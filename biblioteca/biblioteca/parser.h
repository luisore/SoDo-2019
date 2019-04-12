
#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h> //Commons string

typedef enum {
	API_SELECT = 1,
	API_INSERT = 2,
	API_CREATE = 3,
	API_DESCRIBE = 4,
	API_DROP = 5,
	API_JOURNAL = 6,
	API_RUN = 7,
	API_ADD = 8,
} api_operacion;


typedef struct {
	api_operacion nombre_operacion;
	char** parametros;
} struct_operacion;




struct_operacion parsear_linea(char[]);

//otras
void liberarListaDeStrings(char** parametros); //sirve para liberar la memoria de una lista de strings con todas sus boludeces




#endif /* PARSER_H_ */
