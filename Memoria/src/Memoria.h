/*
 * Memoria.h
 *
 *  Created on: 3 abr. 2019
 *      Author: utnso
 */

#ifndef MEMORIA_H_
#define MEMORIA_H_

#include "../../biblioteca/biblioteca/bibliotecaDeSockets.h"
#include "../../biblioteca/biblioteca/parser.h"
//include "../../compartido/bibliotecaDeSockets.h"
//#include "../../compartido/parser.h"

#include <commons/config.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <pthread.h>
#include <stdint.h>
#include "PaginacionSegmentacion.h"
#include "FuncionesAuxiliares.h"

t_log* log_Memoria;
t_config * memoria_config_leer;

char *dir_actual;

int cantidad_de_Paginas;
void consola_memoria();
void gossiping();
void pool();
void inotify();

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

 }t_config_memoria;

 int cantidadBytes;

int tamanio_pagina;
t_list *lista_tabla_gossping;
t_list *lista_segmento;
int FileSystem_fd;
char* memoria;

int valor;
t_bitarray *marcos;
t_bitarray *modificado;
void directorio_actual();
t_config_memoria config_memoria;

void cargar_configuracion(char *archivo);
#endif /* MEMORIA_H_ */
