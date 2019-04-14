#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/log.h>

#define CONFIG_PATH "kernel.config"
#define KERNEL_LOG "kernel.log"

typedef struct {
	char* ip_memoria;
	int puerto_memoria;
	int quantum;
	int multiprocesamiento;
	int metadata_refresh;
	int sleep_ejecucion;
} kernel_config;

kernel_config kernel;
t_log *kernel_log;


int cargar_config(char* path);
int cargar_logger(char* nombre);
void imprimir_config();
void config_struct_destroy();



#endif /* CONFIG_CONFIG_H_ */
