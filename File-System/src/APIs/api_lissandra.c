/*
 * api_lissandra.c
 *
 *  Created on: 16 may. 2019
 *      Author: jhon
 */
#include "api_lissandra.h"

#include <commons/config.h>
#include <commons/txt.h>

void select1(const char * nombre_de_tabla, unsigned int key){
//	if yaExisteTabla(nombre_de_tabla);
//	Metadata_Tabla* unaMetadata=obtenerMetadata(nombre_de_tabla);
//	if(hayDatosParaDumpear()){
//
//	}
	return ;

}
	//el timestamp es opcional
void insert_1(const char* nombre_de_tabla,unsigned int key , const char* value){
//	verificar_existencia_de_tabla();
//	Metadata_Tabla *unMetadataDeTabla = obtenerMetadata(nombre_de_tabla);
//
//	return ;
}
void insert_2(const char* nombre_de_tabla,unsigned int key , const char* value, const char * timestamp){
	return ;
}

void create(const char* nombre_de_tabla,const char* tipo_consistencia,unsigned int numero_de_particiones,unsigned int tiempo_de_compactacion ){
	puts("crear tabla");
	if(yaExisteTabla(nombre_de_tabla)){

		puts("tabla existente");
	}
	else{
		crearTabla(nombre_de_tabla);
		crearMetadata_v2(nombre_de_tabla,tipo_consistencia,numero_de_particiones,tiempo_de_compactacion);
	    crearParticiones(nombre_de_tabla,numero_de_particiones);
	}

	puts("fin crear tabla");
}
void crearTabla(const char* nombreDeTabla){//ok

	char* aux_path_de_la_tabla = malloc(strlen(nombreDeTabla)+strlen("Tables/")+strlen(lfs.puntoDeMontaje));
	sprintf(aux_path_de_la_tabla,"%s%s%s",lfs.puntoDeMontaje,"Tables/",nombreDeTabla);
//	mostrarCaracteres(path_aux);
	printf("path de tabla %s es \"%s\" \n",nombreDeTabla,aux_path_de_la_tabla);

	if(mkdir(aux_path_de_la_tabla, S_IRWXU) == -1){
		log_error(logger, "No se pudo crear directorio para tabla  \"%s\" o ya existe \n" ,nombreDeTabla);
		free(aux_path_de_la_tabla);
		return ;
	}
	else {
		free(aux_path_de_la_tabla);
	}
}
//
//
//}
//
void crearMetadata_v2(const char* tabla,const char* tipoConsistencia, unsigned int numeroParticiones,
		unsigned int tiempoCompactacion){ //ok
	char* aux_path=obtenerPathDeTabla(tabla);
	char* path_metadata=malloc(strlen(aux_path)+strlen("/Metadata.metadata")+1); //str_concat(aux_path,"/Metadata.metadata");
	sprintf(path_metadata,"%s/Metadata.metadata",aux_path);
	free(aux_path);
	FILE* unArchivo=fopen(path_metadata,"w+r");
	free(path_metadata);
	fprintf(unArchivo,"CONSISTENCY=%s \n",tipoConsistencia);
	fprintf(unArchivo,"PARTITIONS=%d \n",numeroParticiones );
	fprintf(unArchivo,"COMPACTION_TIME=%d ",tiempoCompactacion);
	fclose(unArchivo);
//	CONSISTENCY=SC
//	PARTITIONS=3
//	COMPACTION_TIME=60000
}
void crearParticiones(const char* tabla, unsigned int numeroDeParticiones){
	char* path_tabla=obtenerPathDeTabla(tabla);
	char* path_particion=malloc(strlen(path_tabla)+15);
	for (int numeroDeParticion = 0; numeroDeParticion < numeroDeParticiones; numeroDeParticion++) {
		sprintf(path_particion,"%s/%d.partition",path_tabla,numeroDeParticion);
		FILE* particion = fopen(path_particion,"w+r");
		fprintf(particion,"SIZE = 0 \n");
		fprintf(particion,"BLOCKS= [] ");
		fclose(particion);
	}
	free(path_tabla);
	free(path_particion);
}



void describe1();
void describe2(const char* nombre_de_tabla);
void drop(const char* nombre_de_tabla){

}

bool  yaExisteTabla(const char* nombre_de_tabla){
	//if exit(RES)

	//loggear el error (de que existe la tabla )
	char* path = obtenerPathDeTabla(nombre_de_tabla);
	bool yaExiste=yaExisteCarpeta(path);
	free(path);
	return yaExiste;
}
char*  obtenerPathDeTabla(const char* nombre_de_tabla)
{
	char* path_aux = malloc(strlen(nombre_de_tabla)+strlen("Tables/")+strlen(lfs.puntoDeMontaje));
	sprintf(path_aux,"%s%s%s",lfs.puntoDeMontaje,"Tables/",nombre_de_tabla);
//	mostrarCaracteres(path_aux);
//	printf("path de tabla %s es \"%s\" \n",nombre_de_tabla,path_aux);
	return  path_aux;
}
void crear_directorio_para_tabla(nombre_de_tabla){

}

void crear_archivo_metadata(nombre_de_tabla,tipo_consistencia,numero_de_particiones,tiempo_de_compactacion){

}
void aplicar_retardo(){
//	sleep(lfs.retardo/1000);
//	usleep();
}

char archivo_path(const char rutaMontaje, const char *rutaArchivo){

	char  complete_path = (char ) malloc(1 + strlen(rutaMontaje) + strlen(rutaArchivo));
    strcpy(complete_path, rutaMontaje);
    strcat(complete_path, rutaArchivo);
    return complete_path;
}
//
////--------------------------VALIDACIONES INICIO
void validarCadenaNoVacia(const char cadena, const char mensajeError){
	if(string_is_empty(cadena)){
		printf(mensajeError);
		return ;
	}
}
//
void validarNombreTablaNoVacia(const char* nombreTabla){
	validarCadenaNoVacia(nombreTabla, "El nombre de la tabla no puede ser vacío");
}

void validarTipoConsistencia(const char* tipoConsistencia){
	validarCadenaNoVacia(tipoConsistencia, "El tipo de consistencia no puede ser vacío");
	validarTipoConsitenciaExistente(tipoConsistencia);
}

void validarTipoConsitenciaExistente(const char* tipoConsistencia){
	if(tipoConsistencia != STRONGCONSISTENCY || tipoConsistencia != EVENTUALCONSISTENCY){
		printf("El tipo de consistencia no existe");
	}
}
//
bool yaExisteCarpeta(const char* path_tabla){
	bool existe=false;
	DIR *directorio = opendir(path_tabla);
	if(directorio != NULL){

		existe = true;
		closedir(directorio);
		return true;
//	}
	closedir(directorio);
	existe = false;
	return existe;
}
////--------------------------VALIDACIONES FIN
//
//
////--------------------------EJECUCIONES INICIO


void mostrarMetadata(const char* tabla){ //ok
	//obtener metadata
	Metadata_Tabla *unMetadata=obtenerMetadata(tabla);
//	t_config* unConfig=config_create(path_metadata);
//	if(unConfig==NULL){
//		fprintf(stderr,"No Existe el archivo %s \n",path_metadata);
//		return;
//	}
//	else {
//		unMetadata.COMPACTION_TIME=config_get_int_value(unConfig,"COMPACTION_TIME");
//		unMetadata.CONSISTENCY=strdup(config_get_string_value(unConfig,"CONSISTENCY"));
//		unMetadata.PARTITIONS=config_get_int_value(unConfig,"PARTITIONS");
//	}
//	config_destroy(unConfig);
	//mostrar
	printf("COMPACTION_TIME = %d  \n",unMetadata->COMPACTION_TIME);
	printf("CONSISTENCY= %s \n",unMetadata->COMPACTION_TIME);
	printf("PARTITIONS= %d \n",unMetadata->PARTITIONS);

	free(unMetadata);

}
Metadata_Tabla* obtenerMetadata(const char* nombreTabla){
	Metadata_Tabla *unMetadata=malloc(sizeof(Metadata_Tabla));
	char* aux = obtenerPathDeTabla(nombreTabla);
		char* path_metadata=malloc(strlen(aux)+20);
		sprintf(path_metadata,"%s/Metadata.metadata",aux);
		free(aux);
		t_config* unConfig=config_create(path_metadata);
		free(path_metadata);
			if(unConfig==NULL){
				fprintf(stderr,"No Existe metadata para la  tabla  \"%s\" \n",nombreTabla);
			}
			else {
				unMetadata->COMPACTION_TIME=config_get_int_value(unConfig,"COMPACTION_TIME");
				strcpy(unMetadata->CONSISTENCY,config_get_string_value(unConfig,"CONSISTENCY"));
				unMetadata->PARTITIONS=config_get_int_value(unConfig,"PARTITIONS");
			}
			config_destroy(unConfig);
	return unMetadata;
}

}
//void metadata_destroy(Metadata_Tabla* unaMetadata){
//	free(unaMetadata->COMPACTION_TIME);
//	free(unaMetadata->CONSISTENCY);
//	free(unaMetadata->PARTITIONS);
//}

//--------------------------EJECUCIONES FIN


