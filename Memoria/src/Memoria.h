/*
 * Memoria.h
 *
 *  Created on: 3 abr. 2019
 *      Author: utnso
 */

#ifndef MEMORIA_H_
#define MEMORIA_H_

#include "../../biblioteca/biblioteca/bibliotecaDeSockets.h"
#include <commons/config.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <stdio.h>
#include <stdlib.h>

t_log* log_Memoria;
t_config * file_system;

char *dir_actual;

typedef struct{
 	int puerto;
 	char* ip_fs;
 	int puerto_fs;
	char **ip_seeds;
	char **puerto_seeds;
	int retardo_mem;
	int retardo_fs;
	int tam_mem;
	int retardo_journal;
	int retardo_gossiping;
	int memori_number;

 }t_config_fileSystem;


int FileSystem_fd;

void directorio_actual();
t_config_fileSystem config_fileSystem;

void cargar_configuracion(char *archivo);
#endif /* MEMORIA_H_ */
