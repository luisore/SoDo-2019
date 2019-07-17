/*
 * File-System.h
 *
 *  Created on: 3 abr. 2019
 *      Author: utnso
 */

#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_
#include "../../biblioteca/biblioteca/bibliotecaDeSockets.h"
#include "../../biblioteca/biblioteca/parser.h"

#include <commons/config.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "util/util.h"
#include "APIs/api_lissandra.h"
#include <readline/readline.h>
#include <readline/history.h>
#include "config/bitmap.h"
#include "config/metadata.h"
#include "config/config.h"
#include <pthread.h>
#include <dirent.h>


//funcion recibir que comunica con memoria
void recibir_conexion();
int FileSystem_fd;

int  LFS_FD;

void lfs_consola();
void compactar();


t_list* obtenerParticionesTemporales(const char* nombreDeTabla);
t_list* obtenerParticionesNoTemporales(const char* nombreDeTabla);
t_list* obtenerListadoDeSubArchivos(const char * pathDirectorio,const char* extension);
t_list* obtenerListadoDeSubArchivosCompleto(const char * pathDirectorio,const char* extension);
RegistroLinea obtenerRegistroLinea(FILE* bloque);
t_list* obtenerListadoDeNombresDeSubArchivos(const char* pathCarpetaPadre);
t_list* obtenerListaDeParticiones_path(const char* nombreDeTabla );//ok
t_list* obtenerParticiones(const char* nombreDeTabla);
unsigned long long lfs_timestamp();
#endif /* FILE_SYSTEM_H_ */
