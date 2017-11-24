/*
 ============================================================================

 ============================================================================
 Name        : master.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <biblioteca/sockets.h>
#include <commons/string.h>
#include <time.h>
#include <biblioteca/estructuras.h>
/*

Señales extraidas de /usr/bin/include/bits/signum.h



#define SIG_ERR	((__sighandler_t) -1)		 Error return.
#define SIG_DFL	((__sighandler_t) 0)		 Default action.
#define SIG_IGN	((__sighandler_t) 1)		 Ignore signal.

Signals.
#define	SIGHUP		1	 Hangup (POSIX).
#define	SIGINT		2	 Interrupt (ANSI).
#define	SIGQUIT		3	 Quit (POSIX).
#define	SIGILL		4	 Illegal instruction (ANSI).
#define	SIGTRAP		5	 Trace trap (POSIX).
#define	SIGABRT		6	 Abort (ANSI).
#define	SIGIOT		6	 IOT trap (4.2 BSD).
#define	SIGBUS		7	 BUS error (4.2 BSD).
#define	SIGFPE		8	 Floating-point exception (ANSI).
#define	SIGKILL		9	 Kill, unblockable (POSIX).
#define	SIGUSR1		10	 User-defined signal 1 (POSIX).
#define	SIGSEGV		11	 Segmentation violation (ANSI).
#define	SIGUSR2		12	 User-defined signal 2 (POSIX).
#define	SIGPIPE		13	 Broken pipe (POSIX).
#define	SIGALRM		14	 Alarm clock (POSIX).
#define	SIGTERM		15	 Termination (ANSI).
#define	SIGSTKFLT	16	 Stack fault.
#define	SIGCLD		SIGCHLD	 Same as SIGCHLD (System V).
#define	SIGCHLD		17	 Child status has changed (POSIX).
#define	SIGCONT		18	 Continue (POSIX).
#define	SIGSTOP		19	 Stop, unblockable (POSIX).
#define	SIGTSTP		20	 Keyboard stop (POSIX).
#define	SIGTTIN		21	 Background read from tty (POSIX).
#define	SIGTTOU		22	 Background write to tty (POSIX).
#define	SIGURG		23	 Urgent condition on socket (4.2 BSD).
#define	SIGXCPU		24	 CPU limit exceeded (4.2 BSD).
#define	SIGXFSZ		25	 File size limit exceeded (4.2 BSD).
#define	SIGVTALRM	26	 Virtual alarm clock (4.2 BSD).
#define	SIGPROF		27	 Profiling alarm clock (4.2 BSD).
#define	SIGWINCH	28	Window size change (4.3 BSD, Sun).
#define	SIGPOLL		SIGIO	 Pollable event occurred (System V).
#define	SIGIO		29	 I/O now possible (4.2 BSD).
#define	SIGPWR		30	 Power failure restart (System V).
#define SIGSYS		31	 Bad system call.
#define SIGUNUSED	31

*/

int leerConfiguracion();
void mandarRutaInicial(char* ruta);

void gestionarTransformacion();
void mandarDatosTransformacion(t_indicacionTransformacion * indicacion, char* rutasRepetidas[], int tamRepetidas);
bool chequearExistencia(char* array[],int tamanio, char* elemento);

void gestionarReduccionLocal();

void mandarDatosReduccionLocal(t_pedidoReduccionLocal * pedido);


void gestionarReduccionGlobal(t_pedidoReduccionGlobal pedido);


void signal_capturer(int numeroSenial);




void avisarWorker();

void procesarPaquete(t_paquete * unPaquete, int * client_socket);
// Variables globales

t_list * pedidosDeTransformacion ;
t_list * pedidosDeRedcuccionLocal;
int tareasRealizadasEnParalelo;
int tareasTotalesReduccionLocal;
int cantidadDeFallos;
char* rutaScriptTransformador ;
char* rutaScriptReductor ;
char* rutaArchivoParaArrancar ;
char* rutaParaAlmacenarArchivo;
int conexionYama;
int conexionWorker;


// Resultado de operaciones

struct metricas {
	float tiempoTotal;
	float promedioJobs;
	unsigned short int tareasEnParalelo;
	unsigned short int tareasPrincipalesDelJob;
	unsigned short int fallosDelJob;

};

int cont = 0;


int main(int argc, char **argv) {

	pedidosDeTransformacion = list_create();
	pedidosDeRedcuccionLocal = list_create();
	rutaScriptTransformador = argv [1];
	rutaScriptReductor = argv [2];
	rutaArchivoParaArrancar = argv [3];
	rutaParaAlmacenarArchivo = argv [4];

	clock_t inicioPrograma = clock();

	bool finDeMensajes = true;

	conexionYama = leerConfiguracion();

	signal(SIGFPE,signal_capturer);

	enviarMensaje(conexionYama,rutaArchivoParaArrancar);


	while(finDeMensajes){

	gestionarSolicitudes(conexionYama, (void *) procesarPaquete);

	}



	printf("El proceso Master termino en: %d", (clock()-inicioPrograma)*1000/CLOCKS_PER_SEC);

	return EXIT_SUCCESS;


}


int leerConfiguracion(){

	char* ruta = "/home/utnso/workspace/tp-2017-2c-NULL/configuraciones/master.cfg";
	t_config * config = config_create(ruta);

	char * puerto = config_get_string_value(config, "YAMA_PUERTO");
	printf("%s", puerto);
	char * ip = config_get_string_value(config, "YAMA_IP");
    printf("%s", ip);

    int socketYama = conectarCliente(ip, puerto, MASTER);
    char * mensaje = string_new();
    string_append(&mensaje,"Hola puto te re cabio");

    enviarMensaje(socketYama, mensaje);
    while(1){

    }
    return socketYama;


}



void mandarRutaInicial(char* ruta){

}



void gestionarTransformacion(){

	int cantPedidos = list_size(pedidosDeTransformacion);
	int posActual = 0;
	pthread_t hiloTransformar[cantPedidos];
	t_indicacionTransformacion * indicacionesDeTransformacion[cantPedidos];
	char* rutasRepetidas[cantPedidos];
	int numeroRepetidas = 0;
	while(posActual < cantPedidos){

		char* rutasAux[cantPedidos];
		t_pedidoTransformacion * pedido = list_get(pedidosDeTransformacion, posActual);
		t_indicacionTransformacion * indicacion = malloc(sizeof(t_indicacionTransformacion));
		indicacion->bloque = pedido->bloque;
		indicacion->ip = pedido->ip;
		indicacion->bytes = 2; // cómo sé cuántos bytes ocupados hay en un bloque?
		indicacion->rutaArchivoTemporal = pedido->rutaArchivoTemporal;
		*indicacionesDeTransformacion[posActual] = *indicacion;
		rutasAux[posActual] = indicacion->rutaArchivoTemporal;
		if(posActual >= 1)
			{
			if(chequearExistencia(rutasAux,posActual,indicacion->rutaArchivoTemporal))
				{
				rutasRepetidas[numeroRepetidas]=indicacion->rutaArchivoTemporal;
				numeroRepetidas++;
				}
			}
		posActual ++;
	}
	posActual = 0;

	while(posActual <= cantPedidos)
	{
	pthread_create(hiloTransformar[posActual],NULL,(void *) mandarDatosTransformacion, (*indicacionesDeTransformacion[posActual], rutasRepetidas, numeroRepetidas));
	posActual++;
	}
	int i=0;
	for(i;i<cantPedidos; i++)
	{
		pthread_join(hiloTransformar[i],NULL);
	}

	enviarMensaje(conexionYama,"salio todo piola la transformacion");

}


bool chequearExistencia(char* array[],int tamanio, char* elemento){
	int contador = 0;
	bool encontroRepetido = false;
	while(contador<=tamanio){
		if(array[contador]==elemento){
			encontroRepetido = true;
		}
		contador++;
	}
	return encontroRepetido;

}


void mandarDatosTransformacion(t_indicacionTransformacion * indicacion, char* rutasRepetidas[], int tamRepetidas){

	if(chequearExistencia(rutasRepetidas, tamRepetidas, indicacion->rutaArchivoTemporal))
	{
		pthread_mutex_t mutexArchivo;
		pthread_mutex_lock(&mutexArchivo);
		avisarWorker(rutaArchivoParaArrancar, rutaScriptReductor); //aca iria en realidad la estructura indicacion con la ruta que requiere mutue exclusion
		pthread_mutex_unlock(&mutexArchivo);
	} else {

		avisarWorker(rutaArchivoParaArrancar, rutaScriptReductor);
	}


}

void gestionarReduccionLocal(){

		pthread_t hiloReduLocal;
	while(list_is_empty(pedidosDeRedcuccionLocal)){
		t_pedidoReduccionLocal * pedido = list_remove(pedidosDeTransformacion , list_size(pedidosDeTransformacion));
		pthread_create(hiloReduLocal,NULL,(void *) mandarDatosReduccionLocal, pedido);
		pthread_join(hiloReduLocal, NULL); // No va ni en pedo esto


	}


}


void mandarDatosReduccionLocal(t_pedidoReduccionLocal * pedido){

	pthread_mutex_t mutexArchivo;
	pthread_mutex_lock(&mutexArchivo);
	avisarWorker(rutaArchivoParaArrancar, rutaScriptReductor); // aca iria comunicacion con worker
	pthread_mutex_unlock(&mutexArchivo);

}
void gestionarReduccionGlobal(t_pedidoReduccionGlobal a){
	avisarWorker();
	gestionarSolicitudes(conexionWorker, (void *) procesarPaquete);
}

void avisarWorker(){};


void gestionarAlmacenadoFinal(t_pedidoAlmacenadoFinal pedido){
	avisarWorker();
}

void signal_capturer(int numeroSenial){

	switch (numeroSenial)
	{
		case 8:
			enviarMensaje(conexionYama,"division por 0");
			break;
		case 11:
			enviarMensaje(conexionYama,"Segmentation fault");
			break;
		case 16:
			enviarMensaje(conexionYama,"Stack Overflow");
			break;
		default:
			break;
	}

	return;
}


void procesarPaquete(t_paquete * unPaquete, int * client_socket) { // contesto a *client_socket


	switch (unPaquete->codigoOperacion) {


	case ENVIAR_SOLICITUD_TRANSFORMACION:
		 ;
		t_pedidoTransformacion * indicacionTransformacion = recibirSolicitudTransformacion(unPaquete);
		int continuarRecibiendoPedidosTransformacion = 1; //recibirMensaje(unPaquete); // hay que castear a entero
		list_add(pedidosDeTransformacion,indicacionTransformacion); //leno lista con pedidos
		if (continuarRecibiendoPedidosTransformacion == 0){
			gestionarTransformacion();
		}
		break;// sale para volver a pedir


	case ENVIAR_SOLICITUD_REDUCCION_LOCAL:

		 ;
		t_pedidoReduccionLocal * indicacionReduLocal = recibirSolicitudReduccionLocal(unPaquete);
		int continuarRecibiendoPedidosDeReduLocal = 1; //recibirMensaje(unPaquete); // hay que castear a entero
		list_add(pedidosDeRedcuccionLocal,indicacionReduLocal);
		if(continuarRecibiendoPedidosDeReduLocal == 0){
			gestionarReduccionLocal();
		}
		break;


	case ENVIAR_SOLICITUD_REDUCCION_GLOBAL:
	//	 ;
		// t_pedidoReduccionGlobal * indicacionReduGlobal = recibirSolicitudReduccionGlobal(unPaquete);
	//	 gestionarReduccionGlobal(indicacionReduGlobal);
		 enviarMensaje(conexionYama,"todo okey");
		break;


	case ENVIAR_SOLICITUD_ALMACENADO_FINAL:
	//	 ;
	//	 t_pedidoAlmacenadoFinal * indicacionAlmacenamientoFinal = recibirSolicitudAlmacenadoFinal(unPaquete);
	//	 gestionarAlmacenadoFinal(indicacionAlmacenamientoFinal);
		break;


	case ENVIAR_ERROR:

		break;


	default:
		break;
	}

	destruirPaquete(unPaquete);
}

