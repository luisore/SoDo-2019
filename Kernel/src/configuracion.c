#include "configuracion.h"

int configuracion_cargar(char* path) {
	if (path == NULL) {
		log_error(kernel_log, "No se encontró el archivo de configuración");
		return EXIT_FAILURE;
	}
	t_config *config = config_create(path);

	if (config == NULL) {
		log_error(kernel_log, "Error al leer ruta del archivo de configuracion");
		return EXIT_FAILURE;
	}

	kernel = malloc(sizeof(kernel_config));

	log_info(kernel_log, ".:: Cargando configuracion ::.");
	kernel->ip_memoria = malloc(10 * sizeof(char));
	strcpy(kernel->ip_memoria, config_get_string_value(config, "IP_MEMORIA"));
	kernel->puerto_memoria = config_get_int_value(config, "PUERTO_MEMORIA");
	kernel->quantum = config_get_int_value(config, "QUANTUM");
	kernel->multiprocesamiento = config_get_int_value(config, "MULTIPROCESAMIENTO");
	kernel->metadata_refresh = config_get_int_value(config, "METADATA_REFRESH");
	kernel->sleep_ejecucion = config_get_int_value(config, "SLEEP_EJECUCION");
	log_info(kernel_log, ".:: Configuracion Cargada exitosamente ::.");

	config_destroy(config);
	return 0;
}

int configuracion_crear_logger(char* nombre) {
	kernel_log = log_create(nombre, "LISSANDRA - KERNEL", true, LOG_LEVEL_TRACE);
	if (kernel_log == NULL) {
		printf("No se pudo crear el log. Abortando ejecución\n");
		return EXIT_FAILURE;
	}

	return 0;
}

void configuracion_imprimir() {
	log_info(kernel_log, ".:: Imprimiendo configuracion ::.");
	log_info(kernel_log, "IP_MEMORIA: %s", kernel->ip_memoria);
	log_info(kernel_log, "PUERTO_MEMORIA: %d", kernel->puerto_memoria);
	log_info(kernel_log, "QUANTUM: %d", kernel->quantum);
	log_info(kernel_log, "MULTIPROCESAMIENTO: %d", kernel->multiprocesamiento);
	log_info(kernel_log, "METADATA_REFRESH: %d", kernel->metadata_refresh);
	log_info(kernel_log, "SLEEP_EJECUCION: %d", kernel->sleep_ejecucion);
	log_info(kernel_log, ".::::::Fin de Configuracion:::::.");
}

void configuracion_destruir() {
	free(kernel->ip_memoria);
	free(kernel);
}
