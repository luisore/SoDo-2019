#include "planificador.h"

void planificador_iniciar(){
	planificador_iniciar_colas();
	while(1){
		while(!list_is_empty(cola_ready)){
			puts("planificando requests");
			planificador_ejecutar_requests();
		}
	}

}

void planificador_agregar_request(request_struct *request_struct) {

	list_add(cola_ready, request_struct);

}

void planificador_ejecutar_requests() {

	list_add(cola_exec, list_remove(cola_ready, 0));

	request_struct* request_a_ejecutar = (request_struct *) list_get(cola_exec, 0);
	switch (request_a_ejecutar->tipo_request) {
	case SCRIPT:
		if(kernel_ejecutar_script(request_a_ejecutar->request)){//si ejecuto el script por completo, mando el request a la cola exit
			list_add(cola_exit, list_remove(cola_exec, 0));
		}
		list_add(cola_ready,list_remove(cola_exec,0));
		break;
	case API:
		kernel_ejecutar_api(request_a_ejecutar->request);
		list_add(cola_exit, list_remove(cola_exec, 0));
		break;
	default:
		break;
	}



}

void planificador_iniciar_colas() {
	puts("colas_iniciadas");
	cola_new = list_create();
	cola_ready = list_create();
	cola_exec = list_create();
	cola_exit = list_create();
}
