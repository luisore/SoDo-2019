/*
 * File-System.h
 *
 *  Created on: 3 abr. 2019
 *      Author: utnso
 */

#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_
#include "../../biblioteca/biblioteca/bibliotecaDeSockets.h"
#include <commons/config.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "util/util.h"
#include "APIs/api_lissandra.h"
//funcion recibir que comunica con memoria
void recibir_conexion();

int FileSystem_fd;

int  LFS_FD;

//typedef struct {
//	int cantParticionesTemporales;
//	char* nombreDeLaTabla;
//	t_list *_inserts;
//}Insert;
//typedef struct {
//		unsigned long timestamp;
//		unsigned int key;
//		char value[lfs.tamanioValue];
//}RegistroLinea;
//t_list * memtable;


#endif /* FILE_SYSTEM_H_ */
