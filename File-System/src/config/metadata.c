/*
 * metadata.c
 *
 *  Created on: 27 nov. 2018
 *      Author: utnso
 */
#include "metadata.h"


//METADATA
void cargar_configuracion_metadata(const char* pathMetadata){
	t_config *configuracion_cfg_temporal=config_create(pathMetadata);
	if(configuracion_cfg_temporal==NULL)perror("no cargo bien  Metadata.bin\n");
//	(&metadata)->cantidad_bloques=config_get_int_value(configuracion_cfg_temporal,"CANTIDAD_BLOQUES");
	lfs_metadata.cantidad_bloques=config_get_int_value(configuracion_cfg_temporal,"CANTIDAD_BLOQUES");
	lfs_metadata.tamanio_de_bloque=config_get_int_value(configuracion_cfg_temporal,"TAMANIO_BLOQUES");
	config_destroy(configuracion_cfg_temporal);
}
void mostrar_configuracion_metadata(){
	puts("------------leyendo metadata-----------");
	printf("tamanio bloque %d \n", lfs_metadata.tamanio_de_bloque);
	printf("cantidad_bloques %d \n", lfs_metadata.cantidad_bloques);
	puts("------------fin lectura metadata ---------");
}
//METADATA
