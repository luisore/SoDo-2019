
#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include "bibliotecaDeSockets.h"

typedef enum {
	API_SELECT = 1,
	API_INSERT = 2,
	API_CREATE = 3,
	API_DESCRIBE = 4,
	API_DROP = 5,
	API_JOURNAL = 6,
	API_RUN = 7,
	API_ADD = 8,
	API_METRICS = 9,
} api_operacion;


typedef struct {
	api_operacion nombre_operacion;
	char** parametros;
} struct_operacion;


struct_operacion* parsear_linea(char[]);
void free_operacion(struct_operacion* operacion);
int convertinAtipoConsistencia(char *tipoConsistencia);

//otras
void free_list_of_strings(char** list_of_strings);
void* estructura_registro (struct_operacion* operacion,int tipo_operacion);

#endif /* PARSER_H_ */
