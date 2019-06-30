#ifndef METRICAS_H_
#define METRICAS_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <commons/string.h>
#include <commons/collections/list.h>

#include "Kernel.h"
#include "criterio.h"

typedef struct{
	api_operacion operacion;
	time_t tiempo_llegada;
	int segundos_ejecutando;
} latency_struct;

typedef struct{
	int read_latency;
	int write_latency;
	int reads;
	int writes;
} metricas_struct;


t_list* latency_SC;
t_list* latency_SHC;
t_list* latency_EC;


latency_struct* _metricas_crear_latency(int segundos_ejecutando, api_operacion operacion);
t_list* _metricas_obtener_lista_latency(char* criterio);
void _metricas_imprimir_log(char* criterio, metricas_struct* metricas);


void metricas_iniciar_colas();
void metricas_agregar_latency(int tiempo, api_operacion operacion, char* criterio);
void metricas_imprimir(char* criterio);

#endif /* METRICAS_H_ */
