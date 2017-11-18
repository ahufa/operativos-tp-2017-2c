#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <biblioteca/sockets.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "consola.h"
#include "tablas.h"

/*---------	---------------Estructuras-------------------------*/
#define PUERTO_FILESYSTEM "3200"

/*------------------------Variables globales-------------------------*/
t_log* logFileSystem;

/*------------------------Procesamiento paquetes-------------------------*/
void 				procesarPaquete					(t_paquete *, int *);
void 				recibirHandshake				(t_paquete *, int *);
void 				recibirInfoNodo					(t_paquete *, int);
void 				recibirError					(t_paquete *);

/*-------------------------Almacenar archivo-------------------------*/
void				almacenarArchivo				(char *, char *, char *, int);
void * 				dividirBloqueArchivoBinario		(void *, int *);
void * 				dividirBloqueArchivoTexto		(void *, int *);
char * 				buscarNodoMenosCargado			(void);
int 				buscarBloqueAEscribir			(char *);

/*-------------------------Funciones auxiliares-------------------------*/
void 				iniciarServidor					(char*);
void 				armarRegistroTablaArchivos		(char **, char **, char *, int, int, int);

#endif /* FILESYSTEM_H_ */
