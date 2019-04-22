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

	pthread_t hilo_consola;
	pthread_t hilo_kernel;
	pthread_t hilo_pool;
	pthread_t hilo_inotify;


	pthread_create(&hilo_consola,NULL,(void*)consola_memoria,NULL);
	pthread_create(&hilo_pool,NULL,(void*)pool,NULL);
	pthread_create(&hilo_kernel,NULL,(void*)kernel,NULL);
	pthread_create(&hilo_inotify,NULL,(void*)inotify,NULL);

	for(;;);
	pthread_join(hilo_consola,NULL);
	pthread_detach(hilo_pool);
	pthread_detach(hilo_kernel);
	pthread_detach(hilo_inotify);
	return EXIT_SUCCESS;
}


void inotify(){
	//Detectar cambios en el archivo configuracion
}

void consola_memoria(){
	log_info(log_Memoria,"Consola en linea");
	char * linea;
	while(1) {
		linea = readline("API_MEMORIA>");
		if(!strncmp(linea, "SELECT ", 7)) {
			//parsear comando select

		}
		if(!strncmp(linea, "INSERT ", 7)){
			//parsear comando insert
		}
		if(!strncmp(linea, "CREATE ", 7)){
			//parsear comando create
		}
		if (!strncmp(linea, "DROP ", 5)){
			//parsear comando drop
		}
		if (!strncmp(linea, "DESCRIBE ", 9)){
			//parsear comando describe
		}
		if (!strncmp(linea, "JOURNAL ", 8)){
			//parsear comando journal
		}
		if (!strncmp(linea, "EXIT", 4)){
			break;
		}
	}
}
void kernel(){
	//Conexion del kernel a memoria
}
void pool(){
 //logica para lo de grooping

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

