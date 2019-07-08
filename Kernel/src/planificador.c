#include "planificador.h"

void planificador_iniciar(){
	planificador_iniciar_colas();
	while(1){
		if (!list_is_empty(cola_ready)) {
			//log_info(kernel_log,"Ejecuto quantum %d\n",kernel->quantum);
			//log_info(kernel_log,"tamanio cola ready antes: %d \n",list_size(cola_ready));
			planificador_ejecutar_requests();
			//log_info(kernel_log,"tamanio cola ready despues: %d \n",list_size(cola_ready));
		}
	}

}

void planificador_agregar_request(request_struct *request_struct) {
	//mutex
	list_add(cola_ready, request_struct);
	//mutex
}

void planificador_ejecutar_requests() {

	//mutex ready
	//mutex exec
	list_add(cola_exec, list_remove(cola_ready, 0));
	//mutex exec
	//mutex ready

	request_struct* request_a_ejecutar = (request_struct *) list_get(cola_exec, 0);
	switch (request_a_ejecutar->tipo_request) {
	case SCRIPT:
		/*si ejecuto el script por completo, mando el request a la cola exit*/
		if(kernel_ejecutar_script(request_a_ejecutar->request)){

			//mutex exec
			//mutex exit
			list_add(cola_exit, list_remove(cola_exec, 0));
			//mutex exit
			//mutex exec

		}else{

			//mutex exec
			//mutex ready
			list_add(cola_ready,list_remove(cola_exec,0));
			//mutex ready
			//mutex exec
		}
		break;
	case API:
		kernel_ejecutar_api(request_a_ejecutar->request);

		//mutex exec
		//mutex exit
		list_add(cola_exit, list_remove(cola_exec, 0));
		//mutex exit
		//mutex exec

		break;
	default:
		break;
	}



}

void planificador_iniciar_colas() {
	cola_new = list_create();
	cola_ready = list_create();
	cola_exec = list_create();
	cola_exit = list_create();
}
