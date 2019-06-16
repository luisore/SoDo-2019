#include "Kernel.h"

int main(void) {
	if (kernel_inicializar() == EXIT_FAILURE) {
		kernel_exit();
		return EXIT_FAILURE;
	}

	configuracion_imprimir();

	//abrir_script("lissandra.txt");


	/* Creo el hilo consola */
	pthread_t thread_consola;
	if(pthread_create( &thread_consola, NULL, (void*) consola_iniciar, NULL) ){
		log_error(kernel_log,"[KERNEL] No pude crear el hilo para la consola");
		kernel_exit();
		exit(EXIT_FAILURE);
	}
	log_info(kernel_log, "[KERNEL] Creo el hilo para la consola");
	pthread_detach(thread_consola);


	/* Creo el hilo planificador */
	pthread_t thread_planificador;
	if(pthread_create( &thread_planificador, NULL, (void*) planificador_iniciar, NULL) ){
		log_error(kernel_log,"[KERNEL] No pude crear el hilo planificador");
		exit(EXIT_FAILURE);
	}
	log_info(kernel_log, "[KERNEL]  Creo el hilo planificador");
	pthread_detach(thread_planificador);

	/*
	char str1[20];
	while(scanf("%s", str1) == 1){
		printf("Entered Name: %s\n", str1);
	}
*/
	while(1){}

	kernel_exit();
	return EXIT_SUCCESS;
}

void consola_iniciar() {
	printf("Bienvenido! Ingrese \"ayuda\" para ver una lista con todos los comandos disponibles \n");
	char* linea;
	while (1) {
		linea = readline(">");
		if (linea)
			add_history(linea);

		if (!strncmp(linea, "exit", 4)) {
			free(linea);
			break;
		}
		request_struct* request = request_create(API, linea);
		planificador_agregar_request(request);
		free(linea);
	}
}

bool kernel_ejecutar_api(api_struct* api){
	struct_operacion* operacion = parsear_linea(api->lineaLQL);
	kernel_ejecutar(operacion);
	return true;
}

bool kernel_ejecutar_script(script_struct* script){

	FILE * archivo;
	char * linea = NULL;
	size_t longitud = 0;
	ssize_t leido;
	archivo = fopen(script->path, "r");

	if (archivo == NULL) {
		log_error(kernel_log, "No se puede abrir el Script.");
		return true;
	}

	int lineas_leidas = 0;
	int quantum_restante = kernel->quantum;

	while ((leido = getline(&linea, &longitud, archivo)) != -1) {
		if((lineas_leidas >= script->contadorlinea) && quantum_restante > 0 ){
			struct_operacion* operacion = parsear_linea(linea);
			//TODO: validar que la operacion se ejecuto correctamente
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
		log_info(kernel_log, "[KERNEL] ejecuta: SELECT, Tabla: %s, Key: %s\n",
				(operacion->parametros)[0], (operacion->parametros)[1]);
		break;
	case API_INSERT:
		log_info(kernel_log,
				"[KERNEL] ejecuta: INSERT, Tabla: %s, Key: %s, Value: %s, Timestamp: %s\n",
				(operacion->parametros)[0], (operacion->parametros)[1],
				(operacion->parametros)[2], (operacion->parametros)[3]);
		break;
	case API_CREATE:
		log_info(kernel_log,
				"[KERNEL] ejecuta: CREATE, Tabla: %s, Tipo Consistencia: %s, Numero Particiones: %s, Compaction Time: %s\n",
				(operacion->parametros)[0], (operacion->parametros)[1],
				(operacion->parametros)[2], (operacion->parametros)[3]);
		break;
	case API_DESCRIBE:
		log_info(kernel_log, "[KERNEL] ejecuta: DESCRIBE, Tabla: %s\n",
				(operacion->parametros)[0]);
		break;
	case API_DROP:
		log_info(kernel_log, "[KERNEL] ejecuta: DROP, Tabla: %s\n", (operacion->parametros)[0]);
		break;
	case API_JOURNAL:
		log_info(kernel_log, "[KERNEL] ejecuta: JOURNAL\n");
		break;
	case API_ADD:
		log_info(kernel_log, "[KERNEL] ejecuta: ADD MEMORY , Numero: %s, Criterio: %s\n",
				(operacion->parametros)[0], (operacion->parametros)[1]);
		break;
	case API_RUN:
		log_info(kernel_log, "[KERNEL] ejecuta: RUN, <path>: %s\n", (operacion->parametros)[0]);
		request_struct * request = request_create(SCRIPT,(operacion->parametros)[0]);
		planificador_agregar_request(request);

		break;
	case API_METRICS:
		log_info(kernel_log, "[KERNEL] ejecuta: METRICS\n");
		break;
	}

}

int kernel_inicializar(char* nombre_archivo_log) {
	if (configuracion_crear_logger(KERNEL_LOG) == EXIT_FAILURE)
		return EXIT_FAILURE;

	if (configuracion_cargar(CONFIG_PATH) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	return 0;
}

void kernel_exit() {
	//close(memoria_socket);
	log_destroy(kernel_log);
	configuracion_destruir();
}
