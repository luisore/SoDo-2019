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

//funcion recibir que comunica con memoria
void recibir_conexion();
void crear_estructuras();
int FileSystem_fd;

int  LFS_FD;

void lfs_consola();



#endif /* FILE_SYSTEM_H_ */
