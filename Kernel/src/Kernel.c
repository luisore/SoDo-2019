#include "Kernel.h"

int main(void) {
	if (kernel_inicializar() == EXIT_FAILURE) {
		kernel_exit();
		return EXIT_FAILURE;
	}

	configuracion_imprimir();
	/*inicio las colas de criterios*/
	criterio_iniciar_colas();
	/*inicio la metadata de tablas*/
	metadata_iniciar();

	/* Creo el hilo consola */
	pthread_t thread_consola;
	if (pthread_create(&thread_consola, NULL, (void*) consola_iniciar, NULL)) {
		log_error(kernel_log, "[KERNEL] No pude crear el hilo para la consola");
		kernel_exit();
		exit(EXIT_FAILURE);
	}
	log_info(kernel_log, "[KERNEL] Creo el hilo para la consola");
	pthread_detach(thread_consola);

	/* Creo el hilo planificador */
	pthread_t thread_planificador;
	if (pthread_create(&thread_planificador, NULL, (void*) planificador_iniciar,
			NULL)) {
		log_error(kernel_log, "[KERNEL] No pude crear el hilo planificador");
		exit(EXIT_FAILURE);
	}
	log_info(kernel_log, "[KERNEL]  Creo el hilo planificador");
	pthread_detach(thread_planificador);



	while (1) {
	}

	kernel_exit();
	return EXIT_SUCCESS;
}

void consola_iniciar() {
	log_info(kernel_log,"Bienvenido! Ingrese \"help\" para ver una lista con todos los comandos disponibles \n");
	char* linea;
	while (1) {
		linea = readline(">");
		if (linea)
			add_history(linea);
		if (!strncmp(linea, "exit", 4)) {
			free(linea);
			break;
		}
		if (!strncmp(linea, "help", 5)) {
			consola_imprimir_comandos();
			continue;
		}
		request_struct* request = request_create(API, linea);
		planificador_agregar_request(request);
		free(linea);
	}
}

void consola_imprimir_comandos() {
	printf(
			"\nSELECT [NOMBRE_TABLA] [KEY] \n\t Permite la obtención del valor de una key dentro de una tabla.\n");
	printf(
			"\nINSERT [NOMBRE_TABLA] [KEY] “[VALUE]” [Timestamp] \n\t Permite la creación y/o actualización de una key dentro de una tabla.\n");
	printf(
			"\nCREATE [TABLA] [TIPO_CONSISTENCIA] [NUMERO_PARTICIONES] [COMPACTION_TIME] \n\t Permite la creación de una nueva tabla dentro del file-system.\n");
	printf(
			"\nDESCRIBE\t \n\t Permite obtener la Metadata de todas las tablas que el file-system tenga.\n");
	printf(
			"\nDESCRIBE [NOMBRE_TABLA] \n\t Permite obtener la Metadata de una tabla en particular.\n");
	printf(
			"\nDROP [NOMBRE_TABLA] \n\t Permite la eliminación de una tabla del file system.\n");
	printf(
			"\nJOURNAL \n\t Baja todos los datos de la memoria al file-system.\n");
	printf(
			"\nADD MEMORY [NUMERO] TO [CRITERIO] \n\t Permite asignar una memoria a un criterio en concreto: SC,SHC o EC.\n");
	printf(
			"\nRUN <path> \n\t Permite ejecutar un archivo LQL, siendo <path> la ruta del mismo.\n");
	printf("\nMETRICS \n\t Informa las metricas actuales por consola.\n");
	printf("\nexit \n\t Cierra la consola\n\n");
}

bool kernel_ejecutar_api(api_struct* api) {
	struct_operacion* operacion = parsear_linea(api->lineaLQL);
	kernel_ejecutar(operacion);
	//retornar el status de la operacion
	return true;
}

bool kernel_ejecutar_script(script_struct* script) {

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
		if ((lineas_leidas >= script->contadorlinea) && quantum_restante > 0) {
			struct_operacion* operacion = parsear_linea(linea);

			//si la operacion no fue ejecutada correctamente, finalizo el script
			if(kernel_ejecutar(operacion) != true){return true;}

			free_operacion(operacion);
			script->contadorlinea++;
			quantum_restante--;
		}
		lineas_leidas++;
	}
	fclose(archivo);

	//si ejecuto la ultima linea del script, finalizo el script
	if (lineas_leidas == script->contadorlinea) {return true;}

	return false;
}

bool kernel_ejecutar(struct_operacion* operacion) {

	//estado ejecucion es true si la ejecucion fue correcta o false si no lo fue
	bool estado_ejecucion = true;
	metadata_tabla* metadata;
	int memoria;

	switch (operacion->nombre_operacion) {
	case API_SELECT:
		log_info(kernel_log, "[KERNEL] ejecuta: SELECT, Tabla: %s, Key: %s\n",
				(operacion->parametros)[0], (operacion->parametros)[1]);

		//obtenemos la metadata de la tabla, si no existe no hacemos la operacion
		if ((metadata = metadata_obtener((operacion->parametros)[0])) == NULL) {
			log_error(kernel_log,
					"[KERNEL] No se pudo obtener la metadata de la Tabla: %s\n",
					(operacion->parametros)[0]);
			estado_ejecucion = false;
			break;
		}

		log_info(kernel_log,"Metadata Encontrada: ");
		metadata_imprimir(metadata);

		//obtenemos la memoria a consultar a partir del criterio
		memoria = criterio_obtener_memoria((operacion->parametros)[1],
				metadata->CONSISTENCY);

		//TODO: cada memoria va a tener por archivo de configuracion su numero de memoria

		break;

	case API_INSERT:
		log_info(kernel_log,
				"[KERNEL] ejecuta: INSERT, Tabla: %s, Key: %s, Value: %s, Timestamp: %s\n",
				(operacion->parametros)[0], (operacion->parametros)[1],
				(operacion->parametros)[2], (operacion->parametros)[3]);


		//obtenemos la metadata de la tabla, si no existe no hacemos la operacion
		if ((metadata = metadata_obtener((operacion->parametros)[0])) == NULL) {
			log_error(kernel_log,
					"[KERNEL] No se pudo obtener la metadata de la Tabla: %s\n",
					(operacion->parametros)[0]);
			estado_ejecucion = false;
			break;
		}

		log_info(kernel_log,"Metadata Encontrada: ");
		metadata_imprimir(metadata);

		//obtenemos la memoria a consultar a partir del criterio
		memoria = criterio_obtener_memoria((operacion->parametros)[1],
				metadata->CONSISTENCY);

		break;

	case API_CREATE:
		log_info(kernel_log,
				"[KERNEL] ejecuta: CREATE, Tabla: %s, Tipo Consistencia: %s, Numero Particiones: %s, Compaction Time: %s\n",
				(operacion->parametros)[0], (operacion->parametros)[1],
				(operacion->parametros)[2], (operacion->parametros)[3]);

		metadata = metadata_crear((operacion->parametros)[0],
				(operacion->parametros)[1], (operacion->parametros)[2],
				(operacion->parametros)[3]);

		metadata_agregar(metadata);

		log_info(kernel_log,"Se creo la metadata.\n");
		metadata_imprimir(metadata);

		break;

	case API_DESCRIBE:
		log_info(kernel_log, "[KERNEL] ejecuta: DESCRIBE, Tabla: %s\n",
				(operacion->parametros)[0]);

		break;

	case API_DROP:
		log_info(kernel_log, "[KERNEL] ejecuta: DROP, Tabla: %s\n",
				(operacion->parametros)[0]);

		//obtenemos la metadata de la tabla, si no existe no hacemos la operacion
		if ((metadata = metadata_obtener((operacion->parametros)[0])) == NULL) {
			log_error(kernel_log,
					"[KERNEL] No se pudo obtener la metadata de la Tabla: %s\n",
					(operacion->parametros)[0]);
			estado_ejecucion = false;
			break;
		}

		log_info(kernel_log,"Metadata Encontrada: ");
		metadata_imprimir(metadata);

		break;

	case API_JOURNAL:
		log_info(kernel_log, "[KERNEL] ejecuta: JOURNAL\n");

		break;

	case API_ADD:
		log_info(kernel_log,
				"[KERNEL] ejecuta: ADD MEMORY , Numero: %s, Criterio: %s\n",
				(operacion->parametros)[0], (operacion->parametros)[1]);

		criterio_agregar_memoria((operacion->parametros)[0],
				(operacion->parametros)[1]);

		break;

	case API_RUN:
		log_info(kernel_log, "[KERNEL] ejecuta: RUN, <path>: %s\n",
				(operacion->parametros)[0]);

		request_struct* request = request_create(SCRIPT,
				(operacion->parametros)[0]);
		planificador_agregar_request(request);

		break;

	case API_METRICS:
		log_info(kernel_log, "[KERNEL] ejecuta: METRICS\n");

		break;
	default:
		break;
	}
	kernel_retardo_ejecucion();
	return estado_ejecucion;
}

void kernel_retardo_ejecucion(){
	//log_info(kernel_log,"Sleep ejecucion %d",kernel->sleep_ejecucion / 1000);
	sleep(kernel->sleep_ejecucion / 1000);
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
