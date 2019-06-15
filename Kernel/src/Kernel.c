#include "Kernel.h"

int main(void) {
	if (kernel_inicializar() == EXIT_FAILURE) {
		kernel_exit();
		return EXIT_FAILURE;
	}

	configuracion_imprimir();

	//abrir_script("lissandra.txt");

	//logica para leer de consola
	char * linea;
	while (1) {
		linea = readline(">");

		if (linea)
			add_history(linea);

		if (!strncmp(linea, "exit", 4)) {
			free(linea);
			break;
		}
		printf("%s\n", linea);
		request_struct* request = request_create(API,linea);
		planificador_agregar_request(request);

		free(linea);
	}




	kernel_exit();
	return EXIT_SUCCESS;
}


bool kernel_ejecutar_script(script_struct* script){

	FILE * archivo;
	char * linea = NULL;
	size_t longitud = 0;
	ssize_t leido;
	archivo = fopen(script->path, "r");

	if (archivo == NULL) {
		log_error(kernel_log, "No se puede abrir el Script.");
	}

	int lineas_leidas = 0;
	int quantum_restante = kernel->quantum;

	while ((leido = getline(&linea, &longitud, archivo)) != -1) {
		if((lineas_leidas >= script->contadorlinea) && quantum_restante > 0 ){
			log_info(kernel_log, "%s", linea);
			struct_operacion* operacion = parsear_linea(linea);
			kernel_ejecutar(operacion);
			script->contadorlinea++;
			quantum_restante--;
		}
		lineas_leidas++;
	}
	fclose(archivo);

	if(lineas_leidas == script->contadorlinea){
		return true;
	}
	return false;
}



void kernel_ejecutar(struct_operacion* operacion) {

	switch (operacion->nombre_operacion) {
	case API_SELECT:
		log_info(kernel_log, "SELECT, Tabla: %s, Key: %s\n",
				(operacion->parametros)[0], (operacion->parametros)[1]);
		break;
	case API_INSERT:
		log_info(kernel_log,
				"INSERT, Tabla: %s, Key: %s, Value: %s, Timestamp: %s\n",
				(operacion->parametros)[0], (operacion->parametros)[1],
				(operacion->parametros)[2], (operacion->parametros)[3]);
		break;
	case API_CREATE:
		log_info(kernel_log,
				"CREATE, Tabla: %s, Tipo Consistencia: %s, Numero Particiones: %s, Compaction Time: %s\n",
				(operacion->parametros)[0], (operacion->parametros)[1],
				(operacion->parametros)[2], (operacion->parametros)[3]);
		break;
	case API_DESCRIBE:
		log_info(kernel_log, "DESCRIBE, Tabla: %s\n",
				(operacion->parametros)[0]);
		break;
	case API_DROP:
		log_info(kernel_log, "DROP, Tabla: %s\n", (operacion->parametros)[0]);
		break;
	case API_JOURNAL:
		log_info(kernel_log, "JOURNAL\n");
		break;
	case API_ADD:
		log_info(kernel_log, "ADD MEMORY , Numero: %s, Criterio: %s\n",
				(operacion->parametros)[0], (operacion->parametros)[1]);
		break;
	case API_RUN:
		log_info(kernel_log, "RUN, <path>: %s\n", (operacion->parametros)[0]);
		request_struct * request = request_create(SCRIPT,(operacion->parametros)[0]);
		planificador_agregar_request(request);

		break;
	case API_METRICS:
		log_info(kernel_log, "METRICS\n");
		break;
	}

}

int kernel_inicializar(char* nombre_archivo_log) {
	if (configuracion_crear_logger(KERNEL_LOG) == EXIT_FAILURE)
		return EXIT_FAILURE;

	if (configuracion_cargar(CONFIG_PATH) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	planificador_iniciar_colas();

	return 0;
}

void kernel_exit() {
	//close(memoria_socket);
	log_destroy(kernel_log);
	configuracion_destruir();
}
