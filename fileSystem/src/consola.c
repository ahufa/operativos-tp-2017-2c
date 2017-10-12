#include "consola.h"

/*------------------------------Consola------------------------------*/
void iniciarConsola() {
	char * linea;

	bool ejecutar = true;

	while (ejecutar) {
		linea = readline(">");

		if (linea) {
			add_history(linea);
		} else {
			free(linea);
			break;
		}

		ejecutarComando(linea, &ejecutar);

		free(linea);
	}

	clear_history();
}

void ejecutarComando(char * linea, bool * ejecutar) {
	//MAN
	if (string_equals_ignore_case(linea, "man")) {
		ejecutarMan();
		return;
	}

	//EXIT
	if (string_equals_ignore_case(linea, "exit")) {
		ejecutarExit(ejecutar);
		return;
	}

	//FORMAT
	if (string_equals_ignore_case(linea, "format")) {
		formatearFilesystem();
		return;
	}

	//ELIMINAR DIRECTORIO
	if (string_starts_with(linea, "rm -d")) {
		eliminarDirectorio(linea);
		return;
	}

	//ELIMINAR BLOQUE
	if (string_starts_with(linea, "rm -b")) {
		eliminarBloque(linea);
		return;
	}

	//ELIMINAR ARCHIVO
	if (string_starts_with(linea, "rm")) {
		eliminarArchivo(linea);
		return;
	}

	//RENOMBRAR ARCHIVO O DIRECTORIO
	if (string_starts_with(linea, "rename")) {
		renombrarArchivo(linea);
		return;
	}

	//MOVER ARCHIVO O DIRECTORIO
	if (string_starts_with(linea, "mv")) {
		moverArchivo(linea);
		return;
	}

	//MOSTRAR CONTENIDO ARCHIVO
	if (string_starts_with(linea, "cat")) {
		mostrarContenidoArchivo(linea);
		return;
	}

	//CREAR DIRECTORIO
	if (string_starts_with(linea, "mkdir")) {
		crearDirectorio(linea);
		return;
	}

	//COPIO ARCHIVO LOCAL AL FILE SYSTEM SIGUIENDO LA INTERFAZ DEL FILE SYSTEM
	if (string_starts_with(linea, "cpfrom")) {
		copiarArchivoLocalAlYamafsInterfaz(linea);
		return;
	}

	//COPIO ARCHIVO LOCAL AL FILE SYSTEM
	if (string_starts_with(linea, "cpto")) {
		copiarArchivoLocalAlYamafs(linea);
		return;
	}

	//CREAR COPIA DE BLOQUE EN NODO
	if (string_starts_with(linea, "cpblock")) {
		crearCopiaBloqueEnNodo(linea);
		return;
	}

	//SOLICITAR HASH DEL ARCHIVO
	if (string_starts_with(linea, "md5")) {
		solicitarHash(linea);
		return;
	}

	//LISTAR ARCHIVOS DE UN DIRECTORIO
	if (string_starts_with(linea, "ls")) {
		listarArchivos(linea);
		return;
	}

	//MOSTRAR INFO DEL ARCHIVO
	if (string_starts_with(linea, "info")) {
		mostrarInfo(linea);
		return;
	}

	//NO RECONOZCO EL COMANDO
	printf("No se ha encontrado el comando %s \n", linea);
}

/*------------------------------Comandos------------------------------*/
void ejecutarMan(){
	printf("NAME \n");
	printf("	consola \n\n");

	printf("SYNOPSIS \n");
	printf("	#include <consola.h> \n\n");

	printf("	void format(void) \n");
	printf("	void rm(char * path_archivo) \n");
	printf("	void rm -d(char * path_directorio) \n");
	printf("	void rm -b(char * path_archivo, char * nro_bloque, char * nro_copia) \n");
	printf("	void rename(char * path_original, char * nombre_final) \n");
	printf("	void mv(char * path_original, char * path_final) \n");
	printf("	void cat(char * path_archivo) \n");
	printf("	void mkdir(char * path_dir) \n");
	printf("	void cpfrom(char * path_archivo_origen, char * directorio_yamafs) \n");
	printf("	void cpto(char * path_archivo_yamafs, char * directorio_filesystem) \n");
	printf("	void cpblock(char * path_archivo, char * nro_bloque, char * id_nodo) \n");
	printf("	void md5(char * path_archivo_yamafs) \n");
	printf("	void ls(char * path_directorio) \n");
	printf("	void info(char * path_archivo) \n");
	printf("	void exit(void) \n\n");

	printf("DESCRIPTION \n");
	printf("	format --> Formatear el Filesystem \n");
	printf("	rm --> Eliminar un Archivo \n");
	printf("	rm -d --> Eliminar un Directorio. Si un directorio a eliminar no se encuentra vacío, la operación debe fallar \n");
	printf("	rm -b --> Eliminar un Bloque. Si el bloque a eliminar fuera la última copia del mismo, se abortara la operación informando lo sucedido \n");
	printf("	rename --> Renombra un Archivo o Directorio \n");
	printf("	mv --> Mueve un Archivo o Directorio \n");
	printf("	cat --> Muestra el contenido del archivo como texto plano \n");
	printf("	mkdir --> Crea un directorio. Si el directorio ya existe, el comando deberá informarlo \n");
	printf("	cpfrom --> Copiar un archivo local al yamafs, siguiendo los lineamientos en la operaciòn Almacenar Archivo, de la Interfaz del FileSystem \n");
	printf("	cpto --> Copiar un archivo local al yamafs \n");
	printf("	cpblock --> Crea una copia de un bloque de un archivo en el nodo dado \n");
	printf("	md5 --> Solicitar el MD5 de un archivo en yamafs \n");
	printf("	ls --> Lista los archivos de un directorio \n");
	printf("	info --> Muestra toda la información del archivo, incluyendo tamaño, bloques, ubicación de los bloques, etc. \n");
	printf("	exit --> Cierra la consola \n\n");
}

void ejecutarExit(bool * ejecutar) {
	printf("Se cerro la consola \n");
	*ejecutar = false;
}

void formatearFilesystem() {
	printf("Me piden formatear el file system \n");
}

void eliminarArchivo(char * linea) {
	char * path_archivo = obtenerParametro(linea, 1);

	printf("Me llego el path para eliminar: %s \n", path_archivo);

	//Libero memoria
	free(path_archivo);
}

void eliminarDirectorio(char * linea) {
	char * path_directorio = obtenerParametro(linea, 2);

	printf("Me llego el path_directorio para eliminar: %s \n", path_directorio);

	//Libero memoria
	free(path_directorio);
}

void eliminarBloque(char * linea) {
	char * path_directorio = obtenerParametro(linea, 2);
	char * nro_bloque = obtenerParametro(linea, 3);
	char * nro_copia = obtenerParametro(linea, 4);

	printf("Me llego el path_directorio para eliminar: %s \n", path_directorio);
	printf("Me llego el nro_bloque para eliminar: %s \n", nro_bloque);
	printf("Me llego el nro_copia para eliminar: %s \n", nro_copia);

	//Libero memoria
	free(path_directorio);
	free(nro_bloque);
	free(nro_copia);
}

void renombrarArchivo(char * linea) {
	char * path_original = obtenerParametro(linea, 1);
	char * nombre_final = obtenerParametro(linea, 2);

	printf(
			"Me llego el path_directorio para renombrar: %s y su nombre_final es: %s \n",
			path_original, nombre_final);

	//Libero memoria
	free(path_original);
	free(nombre_final);
}

void moverArchivo(char * linea) {
	char * path_original = obtenerParametro(linea, 1);
	char * path_final = obtenerParametro(linea, 2);

	printf(
			"Me llego el path_directorio para mover: %s y su path_final es: %s \n",
			path_original, path_final);

	//Libero memoria
	free(path_original);
	free(path_final);

}

void mostrarContenidoArchivo(char * linea) {
	char * path_archivo = obtenerParametro(linea, 1);

	printf("Me llego el path_archivo: %s y muestro su contenido \n",
			path_archivo);

	//Libero memoria
	free(path_archivo);
}

void crearDirectorio(char * linea) {
	char * path_dir = obtenerParametro(linea, 1);

	printf("Creo el directorio: %s \n", path_dir);

	//Libero memoria
	free(path_dir);
}

void copiarArchivoLocalAlYamafsInterfaz(char * linea) {
	char * path_archivo_origen = obtenerParametro(linea, 1);
	char * directorio_yamafs = obtenerParametro(linea, 2);

	printf("Me llego el path_archivo_origen: %s \n", path_archivo_origen);
	printf("Lo copio segun interfaz en: %s \n", directorio_yamafs);

	//Libero memoria
	free(path_archivo_origen);
	free(directorio_yamafs);
}

void copiarArchivoLocalAlYamafs(char * linea) {
	char * path_archivo_origen = obtenerParametro(linea, 1);
	char * directorio_filesystem = obtenerParametro(linea, 2);

	printf("Me llego el path_archivo_origen: %s \n", path_archivo_origen);
	printf("Lo copio en: %s \n", directorio_filesystem);

	//Libero memoria
	free(path_archivo_origen);
	free(directorio_filesystem);
}

void crearCopiaBloqueEnNodo(char * linea) {
	char * path_archivo = obtenerParametro(linea, 1);
	char * nro_bloque = obtenerParametro(linea, 2);
	char * id_nodo = obtenerParametro(linea, 3);

	printf("Me llego el path_archivo para copiar: %s \n", path_archivo);
	printf("Me llego el nro_bloque para copiar: %s \n", nro_bloque);
	printf("Me llego el id_nodo en donde copiar: %s \n", id_nodo);

	//Libero memoria
	free(path_archivo);
	free(nro_bloque);
	free(id_nodo);
}

void solicitarHash(char * linea) {
	char * path_archivo_yamafs = obtenerParametro(linea, 1);

	printf("Me llego el path_archivo_yamafs: %s para sacar hash \n",
			path_archivo_yamafs);

	//Libero memoria
	free(path_archivo_yamafs);
}

void listarArchivos(char * linea) {
	char * path_directorio = obtenerParametro(linea, 1);

	printf("Me llego el path_directorio: %s para listar los archivos \n",
			path_directorio);

	//Libero memoria
	free(path_directorio);
}

void mostrarInfo(char * linea) {
	char * path_archivo = obtenerParametro(linea, 1);

	printf("Me llego el path_directorio: %s para mostrar su informacion \n",
			path_archivo);

	//Libero memoria
	free(path_archivo);
}

/*------------------------------Auxiliares------------------------------*/
char * obtenerParametro(char * linea, int parametro) {
	char** substrings = string_split(linea, " ");

	int tamPath = string_length(substrings[parametro]);

	char * path = malloc(tamPath + 1);

	memcpy(path, substrings[parametro], tamPath);

	destruirSubstring(substrings);

	path[tamPath] = '\0';

	return path;
}

void destruirSubstring(char ** sub) {
	int i;
	for (i = 0; sub[i] != NULL; ++i) {
		free(sub[i]);
	}
	free(sub[i]);
	free(sub);
}