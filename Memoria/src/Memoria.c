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
#include <dirent.h>

int main(void) {

	log_Memoria = log_create("Memoria.log","Memoria",false,LOG_LEVEL_INFO);

	char *archivo;
	archivo="src/config_memoria.cfg";

	directorio_actual();
	if(validarArchivoConfig(archivo)<0)
	  	puts("hola");
		return -1;
	puts("0");
	cargar_configuracion(archivo);
    puts("1");
	//Me conecto al file system
	crearSocket(&FileSystem_fd);

	//Se conecta al file system
	if(conectar(&FileSystem_fd,config_fileSystem.puerto_fs,config_fileSystem.ip_fs)!=0){
		  puts( "error");
			exit(1);
	}
	else{
		puts("estableci conexion");
	}


	return EXIT_SUCCESS;
}

void cargar_configuracion(char *archivo){

	file_system=config_create(archivo);
	config_fileSystem.puerto=config_get_int_value(file_system,"PUERTO");
	config_fileSystem.ip_fs=string_duplicate(config_get_string_value(file_system,"IP_FS"));
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

void directorio_actual(){
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {

			dir_actual= malloc(strlen(cwd)+1);
		    memcpy(dir_actual,cwd,strlen(cwd));
		    dir_actual[strlen(cwd)]='\0';
	}
	else
	{
	       perror("getcwd() error");
	}
	puts(dir_actual);
}

