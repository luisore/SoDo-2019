/*
 ============================================================================
 Name        : Kernel.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "Kernel.h"



int main(void) {
	if (inicializar() == EXIT_FAILURE) {
		kernel_exit();
			return EXIT_FAILURE;
	}

	imprimir_config();

	while(1){

	}

	kernel_exit();
	return EXIT_SUCCESS;
}

int inicializar(char* nombre_archivo_log) {
	if (cargar_logger(KERNEL_LOG) == EXIT_FAILURE)
		return EXIT_FAILURE;

	if (cargar_config(CONFIG_PATH) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	return 0;
}

void kernel_exit(){
	//close(memoria_socket);
	log_destroy(kernel_log);
	config_struct_destroy();
}
