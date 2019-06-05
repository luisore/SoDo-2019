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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "util/util.h"
#include "APIs/api_lissandra.h"
#include <readline/readline.h>
#include <readline/history.h>
//funcion recibir que comunica con memoria
void recibir_conexion();
int FileSystem_fd;

int  LFS_FD;



typedef struct {
	int cantParticionesTemporales;
	char* nombreDeLaTabla;
	t_list *_inserts;//esto es del tipo RegistroLinea
}Insert;
typedef struct {
		unsigned long timestamp;
		unsigned int key;
		char* value;
//		char value[lfs.tamanioValue];
}RegistroLinea;
//Insert* memtable_de_inserts;
t_list * memtable;//lista del tipo Insert , el select chequea esto tambien

//RegistroLinea select_compactar(t_list* registros){
//	for
//	return
//}

void lfs_consola();

#endif /* FILE_SYSTEM_H_ */
