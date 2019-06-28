#include "metadata.h"


void metadata_imprimir(metadata_tabla* metadata){
	log_info(kernel_log,"Metadata. NOMBRE: %s, CONSISTENCY: %s, PARTITIONS: %u, COMPACTION_TIME: %lu",	metadata->NOMBRE,
																							metadata->CONSISTENCY,
																							metadata->PARTITIONS,
																							metadata->COMPACTION_TIME);
}

metadata_tabla* metadata_crear(char* nombre_tabla,char* consistency,char* partitions,char* compaction_time){

	metadata_tabla* metadata = malloc(sizeof(metadata_tabla));
	metadata->NOMBRE = strdup(nombre_tabla);
	strncpy(metadata->CONSISTENCY,consistency,3);

	char *ptr;//puntero al pedo
	metadata->PARTITIONS = (int)strtol(partitions, &ptr, 10);
	metadata->COMPACTION_TIME = (long)strtol(compaction_time, &ptr, 10);

	return metadata;
}

metadata_tabla* metadata_obtener(char* nombre_tabla){

	bool coincide_nombre_tabla(char * nombre){
		return string_equals_ignore_case(nombre, nombre_tabla);
	}

	return list_find(metadata_tablas,(void *)coincide_nombre_tabla);
}

bool metadata_existe(char* nombre_tabla){

	bool coincide_nombre_tabla(char * nombre){
		return string_equals_ignore_case(nombre, nombre_tabla);
	}

	return list_any_satisfy(metadata_tablas, (void *)coincide_nombre_tabla);
}

void metadata_agregar(metadata_tabla* metadata){
	list_add(metadata_tablas,metadata);
}

void metadata_iniciar(){
	metadata_tablas = list_create();
}
