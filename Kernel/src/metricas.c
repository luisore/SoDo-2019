#include "metricas.h"

void metricas_iniciar_colas() {
	latency_SC = list_create();
	latency_SHC = list_create();
	latency_EC = list_create();
}

latency_struct* _metricas_crear_latency(int segundos_ejecutando, api_operacion operacion) {
	latency_struct * latency = malloc(sizeof(latency_struct));
	latency->operacion = operacion;
	latency->tiempo_llegada = time(NULL);
	latency->segundos_ejecutando = segundos_ejecutando;
	return latency;
}

void metricas_agregar_latency(int segundos_ejecutando, api_operacion operacion, char* criterio) {
	latency_struct* latency = _metricas_crear_latency(segundos_ejecutando, operacion);
	if (es_strong_consistency(criterio)) {
		list_add(latency_SC, latency);
	} else if (es_strong_hash_consistency(criterio)) {
		list_add(latency_SHC, latency);
	} else if (es_eventual_consistency(criterio)) {
		list_add(latency_EC, latency);
	}
}

void metricas_imprimir(char* criterio) {

	bool se_ejecuto_en_los_ultimos_30_segundos(latency_struct* latency) {
		return latency->tiempo_llegada > time(NULL) - 30;
	}
	//validamos si existieron request los ultimos 3 segundos
	t_list *last_30s_latencys = list_filter(_metricas_obtener_lista_latency(criterio), (void*) se_ejecuto_en_los_ultimos_30_segundos);
	if(list_is_empty(last_30s_latencys)){
		log_info(kernel_log,"No se hizo ninguna request con el criterio %s en los ultimos 30 segundos.",criterio_obtener_nombre(criterio));
		return;
	}

	t_list *select_latencys, *insert_latencys;
	metricas_struct* metricas = malloc(sizeof(metricas_struct));

	bool es_un_select(latency_struct* latency) {
		return latency->operacion == API_SELECT;
	}
	bool es_un_insert(latency_struct* latency) {
		return latency->operacion == API_INSERT;
	}
	int calcular_suma_metricas(int valor, latency_struct* latency) {
		return valor + latency->segundos_ejecutando;
	}

	select_latencys = list_filter(	last_30s_latencys,	(void*) es_un_select);

	metricas->reads = list_size(select_latencys);

	metricas->read_latency =	(int)list_fold(	select_latencys, 0,	(void*) calcular_suma_metricas)	/ list_size(select_latencys);

	insert_latencys = list_filter(	last_30s_latencys,	(void*) es_un_insert);

	metricas->writes = list_size(insert_latencys);

	metricas->write_latency = 	(int)list_fold(	insert_latencys, 0, (void*) calcular_suma_metricas) / list_size(insert_latencys);

	_metricas_imprimir_log(criterio, metricas);
}

t_list* _metricas_obtener_lista_latency(char* criterio){
	t_list* lista_latency = NULL;
	if (es_strong_consistency(criterio)) {
		lista_latency = latency_SC;
	} else if (es_strong_hash_consistency(criterio)) {
		lista_latency = latency_SHC;
	} else if (es_eventual_consistency(criterio)){
		lista_latency = latency_EC;
	}
	return lista_latency;
}

void _metricas_imprimir_log(char* criterio,metricas_struct* metricas){
	log_info(kernel_log,"METRICAS: Read Latency / 30s del Criterio %s : %d ", criterio_obtener_nombre(criterio), metricas->read_latency);
	log_info(kernel_log,"METRICAS: Write Latency / 30s del Criterio %s : %d ", criterio_obtener_nombre(criterio), metricas->write_latency);
	log_info(kernel_log,"METRICAS: Reads / 30s del Criterio %s : %d ", criterio_obtener_nombre(criterio), metricas->reads);
	log_info(kernel_log,"METRICAS: Writes / 30s del Criterio %s : %d ", criterio_obtener_nombre(criterio), metricas->writes);
}
