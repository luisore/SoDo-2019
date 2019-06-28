#ifndef METADATA_H_
#define METADATA_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <commons/collections/list.h>

#include "Kernel.h"

typedef struct {
	char* NOMBRE;
	char CONSISTENCY[4];//=SC
	unsigned int PARTITIONS;//=3
	unsigned long COMPACTION_TIME;//=60000
}metadata_tabla;

t_list* metadata_tablas;

void metadata_iniciar();
metadata_tabla* metadata_crear(char* nombre_tabla,char* consistency,char* partitions,char* compaction_time);
void metadata_agregar(metadata_tabla* metadata);

/*retorna la metadata de la tabla, y si no la encuentra retorna NULL*/
metadata_tabla* metadata_obtener(char* nombre_tabla);
bool metadata_existe(char* nombre_tabla);

void metadata_imprimir(metadata_tabla* metadata);


#endif /* METADATA_H_ */
