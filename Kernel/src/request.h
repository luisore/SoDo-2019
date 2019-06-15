#ifndef REQUEST_H_
#define REQUEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef enum {
    SCRIPT = 001,
    API = 002
} tipo_request;

typedef struct {
	int contadorlinea;
	char* path;
} script_struct;

typedef struct {
	char* lineaLQL;
} api_struct;

typedef struct {
	tipo_request tipo_request;
	void* request;
} request_struct;

request_struct * request_create(tipo_request tipo,char * linea_o_path);

#endif /* REQUEST_H_ */
