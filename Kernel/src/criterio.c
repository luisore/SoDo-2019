#include "criterio.h"


void criterio_agregar_memoria(char* memoria, char* consistencia){

	char *ptr;

	int* nro_memoria = malloc(sizeof(int));
	*nro_memoria = (int)strtol(memoria, &ptr, 10);

	if (es_strong_consistency(consistencia))
	{
		if(list_is_empty(criterio_SC)){
			list_add(criterio_SC,nro_memoria);
		}else{
			list_remove(criterio_SC, 0);
			list_add(criterio_SC,nro_memoria);
		}
	}
	else if (es_strong_hash_consistency(consistencia))
	{
		list_add(criterio_SHC, nro_memoria);
	}
	else if (es_eventual_consistency(consistencia))
	{
		list_add(criterio_EC, nro_memoria);
	}else{
		log_error(kernel_log, "no se reconoce la consistencia %s\n",consistencia);
	}

	log_info(kernel_log, "la memoria %d, agregada al criterio %s",*nro_memoria,consistencia);

}

int criterio_obtener_memoria(char* key,char* consistencia){
	char *ptr;
	int nro_key = (int)strtol(key, &ptr, 10);

	int* nro_memoria;

	if (es_strong_consistency(consistencia))
	{
		nro_memoria = list_get(criterio_SC,0);
	}
	else if (es_strong_hash_consistency(consistencia))
	{
		nro_memoria = list_get(criterio_SHC, nro_key % list_size(criterio_SHC));
	}
	else if (es_eventual_consistency(consistencia))
	{
		nro_memoria = list_get(criterio_SHC, (rand() % 100) % list_size(criterio_SHC));
	}else{
		log_error(kernel_log, "no se reconoce la consistencia %s\n",consistencia);
	}
	return *nro_memoria;
}


void criterio_iniciar_colas(){
	criterio_SC = list_create();
	criterio_SHC = list_create();
	criterio_EC = list_create();
}

bool es_strong_consistency(char* consistencia){
	return string_starts_with(consistencia, "SC");
}

bool es_strong_hash_consistency(char* consistencia){
	return string_starts_with(consistencia, "SHC");
}

bool es_eventual_consistency(char* consistencia){
	return string_starts_with(consistencia, "EC");
}

