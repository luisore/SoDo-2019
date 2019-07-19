/*
 * config.c
 *
 *  Created on: 12 abr. 2019
 *      Author: jhon
 */
#include "config.h"
void probando(){
	puts("probando");
}
void config_cargar(const char* dir_config){
	t_config*configuracion=config_create(dir_config);
		 	if(configuracion==NULL){
		 		perror("No se encuentra archivo LFS.CONFIG \n");
//		 		log_error(lfs_log, "No se encuentra archivo LFS.config \n");
		 		return;
			}
	lfs.puertoEscucha=config_get_int_value(configuracion,"PUERTO_ESCUCHA");
		 //		 			strcpy(lfs.puntoDeMontaje,config_get_string_value(configuracion,"PUNTO_DE_MONTAJE"));
	lfs.puntoDeMontaje=string_duplicate(config_get_string_value(configuracion,"PUNTO_MONTAJE"));
	lfs.tamanioValue=config_get_int_value(configuracion,"TAMANIO_VALUE");
	lfs.tiempoDump=config_get_long_value(configuracion,"TIEMPO_DUMP");
	lfs.retardo=config_get_long_value(configuracion,"RETARDO");
	config_destroy(configuracion);

	cargar_configuracion_metadata();
	cargar_configuracion_bitmap();
	//inicio log
	lfs_logger = log_create("LFS.log", "LISSANDRA - LFS", true, LOG_LEVEL_TRACE);
	puts("config_cargar() fin ");
}

void imprimir_configuracion(){
	printf("PUERTO_ESCUCHA = %d \n",lfs.puertoEscucha);
	printf("PUNTO_DE_MONTAJE = %s \n",lfs.puntoDeMontaje);
	printf("TAMANIO_VALUE = %d \n",lfs.tamanioValue);
	printf("TIEMPO_DUMP = %d \n",lfs.tiempoDump);
	printf("RETARDO = %d \n",lfs.retardo);
}

void lfs_log_info(const char* message, ...){
	va_list lista;
	va_start(lista,message);
	log_info(lfs_logger,message,lista);
//	log_info(lfs_log,"\n");
	va_end(lista);
}
void lfs_log_error(const char* message, ...){
	va_list lista;
	va_start(lista,message);
	log_error(lfs_logger,message,lista);
//	log_error(lfs_log,"\n");
	va_end(lista);
}
void lfs_log_warning(const char* message, ...){
	va_list lista;
	va_start(lista,message);
	log_warning(lfs_logger,message,lista);
//	log_warning(lfs_log,"\n");
	va_end(lista);
}

