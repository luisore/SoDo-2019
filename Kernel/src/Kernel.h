#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <commons/collections/list.h>
#include "../../biblioteca/biblioteca/bibliotecaDeSockets.h"
#include "../../biblioteca/biblioteca/parser.h"
#include "configuracion.h"//Configuracion y Logger

#include "planificador.h"
#include "request.h"
#include "criterio.h"
#include "metadata.h"
#include "metricas.h"

int memoria_fd;


int kernel_inicializar();
void kernel_exit();
bool kernel_ejecutar(struct_operacion* operacion);
/*retorna true si ejecuto la ultima linea del script*/
bool kernel_ejecutar_script(script_struct* script);
bool kernel_ejecutar_api(api_struct* api);
void kernel_retardo_ejecucion();

void consola_iniciar();
void consola_imprimir_comandos();


#endif /* KERNEL_H_ */
