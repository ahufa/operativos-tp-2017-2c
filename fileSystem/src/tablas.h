#ifndef TABLAS_H_
#define TABLAS_H_

#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include <biblioteca/sockets.h>

#define RUTA_METADATA "/home/utnso/Escritorio/"

/*---------	---------------Estructuras-------------------------*/
//Tabla de sockets
typedef struct {
	int socket;
	char * nombre;
} t_tabla_sockets;

//Tabla de bitMaps
typedef struct {
	char * nombre;
	t_config * configTablaBitmap;
} t_tabla_bitMaps;

//Tabla de nodos
typedef struct {
	int tamanio;
	int libres;
	t_list * nomNodos; //Lista de nombres de los nodos
	t_list * infoDeNodo; //Lista con cada nodo en particular
} t_tabla_nodo;

//Registro tabla de Directorios
typedef struct {
	int index;
	char nombre[255];
	int padre;
} t_directory;

//Registro tabla de Tareas
typedef struct {
	char * nomNodo;
	int bloque;
} t_tarea;

/*------------------------Variables globales-------------------------*/
t_list * tablaSockets;
t_tabla_nodo * tablaNodos;
t_list * tablaDirectorios;
t_list * tablaTareas;

t_config * configTablaDirectorios;
t_config * configTablaNodo;

bool bitMapDirectorio[100];

/*-------------------------Tabla de directorios-------------------------*/
void 				crearTablaDirectorios			(char *);
void 				crearTablaDirectorioSegunArchivo(char *);
void 				crearArchivoTablaDirectorios	(char *);
void 				agregarDirectorioTabla			(t_directory *, char *);
void 				eliminarDirectorioTabla			(char *, int);
void 				modificarDirectorioTabla		(t_directory *, char *, int);

/*-------------------------Tabla de archivos-------------------------*/
t_config *			crearArchivoTablaArchivo		(char *, char *, char *, int);
void 				agregarRegistroTablaArchivos	(char *, int,int, int, t_config *);
void 				guardoBytesPorBloque			(int, int, t_config *);
char ** 			buscarBloque					(t_config *, int, int);

/*-------------------------Tabla de nodos-------------------------*/
void	 			crearTablaNodos					(char *);
void 				crearArchivoTablaNodos			(char *);
void 				crearTablaNodosSegunArchivo		(char *);
void 				agregarNodoTablaNodos			(t_nodo_info *);
void 				eliminarNodoTablaNodos			(char *);
void 				liberarBloqueTablaNodos			(char *, int);
void 				persistirTablaNodos				(void);

/*-------------------------Tabla de sockets-------------------------*/
void 				crearTablaSockets				(void);
void 				agregarNodoTablaSockets			(char *, int);
char * 				eliminarNodoTablaSockets		(int);
int 				buscarSocketPorNombre			(char *);
char * 				buscarNombrePorSocket			(int);
void 				modificarNodoTablaSockets		(char *, int);

/*-------------------------Tabla de Bitmap-------------------------*/
void 				crearArchivoTablaBitmap			(t_nodo_info *);
int 				buscarBloqueLibre				(t_config *);
void 				liberarBloquebitMap				(char *, int);

/*-------------------------Funciones auxiliares-------------------------*/
char * 				armarRegistroDirectorio			(char *, int);
int 				buscarIndexLibre				(void);
int 				obtenerIndex					(char *);
void 				llenarBitmap					(void);
void 				destruirSubstring				(char **);

#endif /* TABLAS_H_ */
