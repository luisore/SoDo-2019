#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

#include "../../biblioteca/biblioteca/parser.h"
#include <commons/collections/list.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <commons/collections/queue.h>

#include "request.h"
#include "Kernel.h"



t_list* cola_new;
t_list* cola_ready;
t_list* cola_exec;
t_list* cola_exit;


void planificador_agregar_request(request_struct *request_struct);
void planificador_ejecutar_requests();
void planificador_iniciar_colas();
void planificador_iniciar();


#endif /* PLANIFICADOR_H_ */

