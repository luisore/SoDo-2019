#ifndef CRITERIO_H_
#define CRITERIO_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <commons/collections/list.h>

#include "Kernel.h"

/*no se usa*/
typedef enum {
	SC = 0,
	SHC = 1,
	EC = 2,
} tipo_consistencia;

t_list* criterio_SC;
t_list* criterio_SHC;
t_list* criterio_EC;

void criterio_iniciar_colas();
void criterio_agregar_memoria(char* memoria, char* consistencia);
int criterio_obtener_memoria(char* key,char* consistencia);
bool es_strong_consistency(char* consistencia);
bool es_strong_hash_consistency(char* consistencia);
bool es_eventual_consistency(char* consistencia);
char* criterio_obtener_nombre(char* criterio);

#endif /* CRITERIO_H_ */
