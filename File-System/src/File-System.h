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


#include<stdlib.h>
#include "util/utils.h"


void recibir_conexion();

int FileSystem_fd;

int  LFS_FD;

#endif /* FILE_SYSTEM_H_ */
