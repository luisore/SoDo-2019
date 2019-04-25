#include "Kernel.h"



int main(void) {
	if (inicializar() == EXIT_FAILURE) {
		kernel_exit();
			return EXIT_FAILURE;
	}

	imprimir_config();

	abrir_script("lissandra.txt");
	while(1){

	}

	kernel_exit();
	return EXIT_SUCCESS;
}

void abrir_script(char* path){

	FILE * archivo;
	char * linea = NULL;
	size_t longitud = 0;
	ssize_t leido;
	archivo = fopen(path, "r");

	//TODO: creo un contador de lineas leidas
	//y si las lineas leidas son mayor al contador del programa IP, entonces ejecuto tantas lineas como quantum
	if (archivo == NULL) {
		log_error(kernel_log, "No se puede abrir el Script.");
	}
	while ((leido = getline(&linea, &longitud, archivo)) != -1) {
		log_info(kernel_log, "%s", linea);
		struct_operacion* operacion = parsear_linea(linea);
		kernel_ejecutar(operacion);
	}
	fclose(archivo);
}


void kernel_ejecutar(struct_operacion* operacion){

	switch (operacion->nombre_operacion) {
		case API_SELECT:
			log_info(kernel_log, "SELECT, Tabla: %s, Key: %s\n",(operacion->parametros)[0],
																(operacion->parametros)[1]);
			break;
		case API_INSERT:
			log_info(kernel_log, "INSERT, Tabla: %s, Key: %s, Value: %s, Timestamp: %s\n",	(operacion->parametros)[0],
																							(operacion->parametros)[1],
																							(operacion->parametros)[2],
																							(operacion->parametros)[3]);
			break;
		case API_CREATE:
			log_info(kernel_log,"CREATE, Tabla: %s, Tipo Consistencia: %s, Numero Particiones: %s, Compaction Time: %s\n", 	(operacion->parametros)[0],
																															(operacion->parametros)[1],
																															(operacion->parametros)[2],
																															(operacion->parametros)[3]);
			break;
		case API_DESCRIBE:
			log_info(kernel_log,"DESCRIBE, Tabla: %s\n", (operacion->parametros)[0]);
			break;
		case API_DROP:
			log_info(kernel_log,"DROP, Tabla: %s\n", (operacion->parametros)[0]);
			break;
		case API_JOURNAL:
			log_info(kernel_log,"JOURNAL\n");
			break;
		case API_ADD:
			log_info(kernel_log,"ADD MEMORY , Numero: %s, Criterio: %s\n", 	(operacion->parametros)[0],
																			(operacion->parametros)[1]);
			break;
		case API_RUN:
			log_info(kernel_log,"RUN, <path>: %s\n", (operacion->parametros)[0]);
			break;
		case API_METRICS:
			log_info(kernel_log,"METRICS\n");
			break;
	}

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
