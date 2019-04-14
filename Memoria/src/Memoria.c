/*
 ============================================================================
 Name        : Memoria.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "Memoria.h"


int main(void) {

	log_Memoria = log_create("Memoria.log","Memoria",false,LOG_LEVEL_INFO);

	char *archivo;
	archivo="src/config_memoria.cfg";

	if(validarArchivoConfig(archivo)<0)
	  	return -1;

	cargar_configuracion(archivo);

	return EXIT_SUCCESS;
}

void cargar_configuracion(char *archivo){

	file_system=config_create(archivo);
	config_fileSystem.puerto=config_get_int_value(file_system,"PUERTO");
	config_fileSystem.ip_fs=config_get_int_value(file_system,"IP_FS");
	config_fileSystem.puerto_fs=config_get_int_value(file_system,"PUERTO_FS");
	config_fileSystem.ip_seeds=config_get_array_value(file_system,"IP_SEEDS");
	config_fileSystem.puerto_seeds=config_get_array_value(file_system,"PUERTO_SEEDS");
	config_fileSystem.retardo_mem=config_get_int_value(file_system,"RETARDO_MEM");
	config_fileSystem.retardo_fs=config_get_int_value(file_system,"RETARDO_FS");
	config_fileSystem.tam_mem=config_get_int_value(file_system,"TAM_MEM");
	config_fileSystem.retardo_journal=config_get_int_value(file_system,"RETARDO_JOURNAL");
	config_fileSystem.retardo_gossiping=config_get_int_value(file_system,"RETARDO_GOSSIPING");
	config_fileSystem.memori_number=config_get_int_value(file_system,"MEMORY_NUMBER");

	config_destroy(file_system);

}
