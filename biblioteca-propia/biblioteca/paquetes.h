#ifndef SRC_PROCESAMIENTOPAQUETES_H_
#define SRC_PROCESAMIENTOPAQUETES_H_

#include "estructuras.h"
#include "serializacion.h"

/*-------------------------Paquetes-------------------------*/
void	 						enviarPaquetes						(int, t_paquete *);
int 							recibirTamPaquete					(int);
t_paquete * 					recibirPaquete						(int, int);
t_paquete * 					crearPaquete						(void *);
t_paquete *						crearPaqueteError					(int client_socket);
void 							destruirPaquete						(t_paquete *);
void 							mostrarPaquete						(t_paquete *);

/*-------------------------Enviar paquetes-------------------------*/
void 							enviarHandshake						(int, int);
void 							enviarMensaje						(int, char *);
void 							enviarArchivo						(int, char *);
void 							enviarInfoDataNode					(int, char *, int, int);
void 							enviarBloque						(int, char*);
void 							enviarSolicitudLecturaBloque		(int, int);
void 							enviarSolicitudEscrituraBloque		(int, void*,int);
void 							enviarSolicitudTransformacion		(int, t_pedidoTransformacion *);
void 							enviarSolicitudReduccionLocal		(int, t_pedidoReduccionLocal *);
void 							enviarSolicitudReduccionGlobal		(int, t_pedidoReduccionGlobal *);
void 							enviarSolicitudAlmacenadoFinal		(int, t_pedidoAlmacenadoFinal *);
void 							enviarIndicacionTransformacion		(int, t_indicacionTransformacion *);
void 							enviarIndicacionReduccionLocal		(int, t_indicacionReduccionLocal *);
void 							enviarIndicacionReduccionGlobal		(int, t_indicacionReduccionGlobal *);
void 							enviarIndicacionAlmacenadoFinal		(int, t_indicacionAlmacenadoFinal *);

/*-------------------------Recibir paquetes-------------------------*/
void 							recibirMensaje						(t_paquete *);
void 							recibirArchivo						(t_paquete *);
void 							recibirBloque						(t_paquete *);
int 							recibirSolicitudLecturaBloque		(t_paquete *);
t_pedidoEscritura* 				recibirSolicitudEscrituraBloque		(t_paquete *);
t_pedidoTransformacion* 		recibirSolicitudTransformacion		(t_paquete *);
t_pedidoReduccionLocal* 		recibirSolicitudReduccionLocal		(t_paquete *);
t_pedidoReduccionGlobal*		recibirSolicitudReduccionGlobal		(t_paquete *);
t_pedidoAlmacenadoFinal* 		recibirSolicitudAlmacenadoFinal		(t_paquete *);
t_indicacionTransformacion* 	recibirIndicacionTransformacion		(t_paquete *);
t_indicacionReduccionLocal*	 	recibirIndicacionReduccionLocal		(t_paquete *);
t_indicacionReduccionGlobal*	recibirIndicacionReduccionGlobal	(t_paquete *);
t_indicacionAlmacenadoFinal* 	recibirIndicacionAlmacenadoFinal	(t_paquete *);


#endif /* SRC_PROCESAMIENTOPAQUETES_H_ */
