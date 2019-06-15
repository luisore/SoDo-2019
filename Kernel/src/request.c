#include "request.h"

request_struct * request_create(tipo_request tipo,char * linea_o_path){
	request_struct * request = malloc(sizeof(request_struct));
	request->tipo_request = tipo;

	script_struct * script = NULL;
	api_struct * api = NULL;
	switch (tipo) {
		case SCRIPT:
			script = malloc(sizeof(script_struct));
			script->contadorlinea = 0;
			script->path = strdup(linea_o_path);

			request->request = script;
			break;
		case API:
			api = malloc(sizeof(api_struct));
			api->lineaLQL = strdup(linea_o_path);

			request->request = api;
			break;
		default:
			break;
	}
	return request;
}
