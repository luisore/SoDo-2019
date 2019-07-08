/*
 * api_lissandra.c
 *
 *  Created on: 16 may. 2019
 *      Author: jhon
 */
#include "api_lissandra.h"


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
void insert_2(const char* nombre_de_tabla,unsigned int key , const char* value, unsigned long timestamp){
	if(!yaExisteTabla(nombre_de_tabla)){
		fprintf(stderr,"no exixte la tabla \"%s\" \n",nombre_de_tabla);
	}
	else {
		if(hay_datos_a_dumpear())dumpear();//aca en lugar de if , se reemplaza con un semaforo , pues dumpear() es una seccion critica, con un mutex, solo hay un proceso dumpeo
		insertarEnMemtable( nombre_de_tabla, key , value,timestamp);
	}
}
void insertarEnMemtable(const char* nombre_de_tabla,unsigned int key , const char* value,unsigned long timestamp){//la logica  es esta, ok
	Insert* unInsert = buscarTablaEnLaMemtable(nombre_de_tabla);
	RegistroLinea* unRegistro=malloc(sizeof(RegistroLinea));
			unRegistro->key=key;
			unRegistro->value=strdup(value);
			unRegistro->timestamp;
	if(unInsert==NULL){//si es igual a NULL no esta en la memtable
		unInsert =malloc(sizeof(Insert));
		unInsert->nombreDeLaTabla=strdup(nombre_de_tabla);
		unInsert->cantParticionesTemporales=0;
		unInsert->registros=list_create();
		list_add((unInsert->registros),unRegistro);
	}
	else {
		list_add((unInsert->registros),unRegistro);
	}
}
Insert* buscarTablaEnLaMemtable(const char * tabla){
//	Insert* insertConTabla;
	bool existeTablaEnLaMemtable(Insert *unInsert) {//simulo aplicacion parcial
	            			return strcmp(unInsert->nombreDeLaTabla,tabla)==0;//string_equals_ignore_case(unInsert->nombreDeLaTabla, tabla);
	            		}
//	aux_tabla_para_la_memtable=tabla;
	return list_find(memtable,existeTablaEnLaMemtable);// // le puse (void*) por que en los tests lo manejan asi
//	return insertConTabla;
}
//bool existeTablaEnLaMemtable(Insert* unInsert){
//	return strcmp( unInsert->nombreDeLaTabla,aux_tabla_para_la_memtable)==0;//si es =0 son iguales
//}
bool hay_datos_a_dumpear(){
//	bool hay_datos_en_memoria_para_dump=false;
//	hay_datos_en_memoria_para_dump=laMemtableTieneContenido();
//	return laMemtableTieneContenido();
	return list_size(memtable)>0;//hay datos en la memtable
}
//bool laMemtableTieneContenido(){
////	return memtable->elements_count=0;
//	return list_size(memtable)>0;
//}
void dumpear(){
	Insert* unInsert;
	for(int tabla_iesima =0;tabla_iesima<list_size(memtable);tabla_iesima++){
		unInsert = list_get(memtable,tabla_iesima);
		for(int fila_iesima =0;fila_iesima<list_size(unInsert->registros);fila_iesima++){
			insertarRegistrosEnParticionTemporal(unInsert->nombreDeLaTabla,(RegistroLinea*)list_get((unInsert->registros),fila_iesima), unInsert->cantParticionesTemporales);
			unInsert->cantParticionesTemporales++;
		}
	}
}
void insertarRegistrosEnParticionTemporal(const char* tabla,  RegistroLinea* unRegistro,int cantidadDeParticionesTemporales){
//	Metadata_Tabla* metadataDeTabla = obtenerMetadata(tabla);
//	unsigned int particionCorrespondiente = particionSegunKey(unRegistro,metadataDeTabla->PARTITIONS);
	char* path_de_particion_temporal=obtenerPathDeParticionTemporal(cantidadDeParticionesTemporales+1);
	escribirAParticion(path_de_particion_temporal,unRegistro);
	int bloqueGrabado=grabarRegistroABloques(unRegistro);
//	free(metadataDeTabla);
}
void escribirAParticion(const char* path_particion,RegistroLinea* registro){
	size_t longitud_usada=longitudDeRegistroAlFileSystem(registro);


	t_config* config_particion = config_create(path_particion);
	if(config_particion==NULL){//hay que crear la particion
		FILE* part=fopen(path_particion,"w+r");
		fprintf(part,"SIZE=  \n");
		fprintf(part,"BLOCKS=  \n");
		fclose(part);
	}
	config_set_value(config_particion,"SIZE",string_itoa(longitud_usada));
//	config_set_value(config_particion,"BLOCKS",string_itoa(longitud_usada));//falta grabar este campo , ejemplo grabar [1,2,3,4] por ejemplo

	config_destroy(config_particion);
}
size_t longitudDeRegistroAlFileSystem(RegistroLinea* unRegistro){
	size_t longitud;
	char* aux=malloc(sizeof(unRegistro->timestamp)+sizeof(unRegistro->key)+strlen(unRegistro->value)+1000);
	sprintf(aux,"%d;%d;%s\n",unRegistro->timestamp,unRegistro->key,unRegistro->value);
	longitud=strlen(aux);
	free(aux);
	return longitud;
}
char* registroLineaAString(RegistroLinea* registro){
	char* registro_string=malloc(longitudDeRegistroAlFileSystem(registro));
	sprintf(registro_string,"%d;%d;%s\n",registro->timestamp,registro->key,registro->value);
	return registro_string;
}

char* obtenerPathDeParticionTemporal(numeroDeParticionTemporal){
	char* pathDeParticion=malloc(strlen(lfs.puntoDeMontaje)+strlen("/Tables/")+40);
	sprintf(pathDeParticion,"%sTables/%d.tmp");
	return pathDeParticion;
}
Metadata_Tabla* obtenerMetadata(const char* nombreTabla){
	Metadata_Tabla *unMetadata=(Metadata_Tabla*)malloc(sizeof(Metadata_Tabla));
	char* aux = obtenerPathDeTabla(nombreTabla);
		char* path_metadata=malloc(strlen(aux)+strlen("/Metadata.metadata")+1);
		sprintf(path_metadata,"%s/Metadata.metadata",aux);
		free(aux);
		t_config* unConfig=config_create(path_metadata);
		free(path_metadata);
			if(unConfig==NULL){
				fprintf(stderr,"No Existe metadata para la  tabla  \"%s\" \n",nombreTabla);
				perror("No Existe metadata para la  tabla");
			}
			else {
				unMetadata->COMPACTION_TIME=config_get_int_value(unConfig,"COMPACTION_TIME");
				strcpy(unMetadata->CONSISTENCY,config_get_string_value(unConfig,"CONSISTENCY"));
				unMetadata->PARTITIONS=config_get_int_value(unConfig,"PARTITIONS");
			}
			config_destroy(unConfig);
	return unMetadata;
}
int  grabarRegistroABloques(RegistroLinea* unRegistro){
	int posicionBloqueLibre = getBloqueLibre_int();
	char* bloqueLibre_path=obtenerPathDelNumeroDeBloque(posicionBloqueLibre);
	escribirRegistroABloque(bloqueLibre_path,unRegistro);
	free(bloqueLibre_path);
	return posicionBloqueLibre;
}
char* obtenerPathDelNumeroDeBloque(int numeroDeBloque){
	char* path_del_bloque=malloc(strlen(lfs.puntoDeMontaje)+strlen("/Bloques")+20);
	sprintf(path_del_bloque,"%sBloques/%d.bin",lfs.puntoDeMontaje,numeroDeBloque);
	return path_del_bloque;
}

void escribirRegistroABloque(const char * bloque_path,RegistroLinea* unRegistro){
	FILE* unBloque = txt_open_for_append(bloque_path);//fopen(bloque_path,"r+w");
	fprintf(unBloque,"%d;%d;%s\n",unRegistro->timestamp,unRegistro->key,unRegistro->value);
	fclose(unBloque);
}
int particionSegunKey(RegistroLinea* unRegistro,unsigned int cantidad_de_particiones){
	return unRegistro->key % cantidad_de_particiones;
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

void lfs_create(const char* nombre_de_tabla,const char* tipo_consistencia,const char*  numero_de_particiones,const char* tiempo_de_compactacion ){
	create(nombre_de_tabla,tipo_consistencia,atoi(numero_de_particiones),atoi(tiempo_de_compactacion));
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
void describe1(){
	char* aux =(char*)malloc(strlen(lfs.puntoDeMontaje)+1+strlen("Tables")+2);//"src/punto_de_montaje_FS_LISSANDRA_ejemplo/Tables"
	sprintf(aux,"%s%s",lfs.puntoDeMontaje,"Tables");
	puts(aux);
	t_list* listaDeNombreDeTablas= obtenerListadoDeNombresDeSubArchivos(aux);
	list_iterate(listaDeNombreDeTablas,puts);
	free(aux);
//	list_map(listaDeNombreDeTablas,describe2);
	list_destroy(listaDeNombreDeTablas);
}
void describe2(const char* nombre_de_tabla){
	Metadata_Tabla* metadata= obtenerMetadata(nombre_de_tabla);
	printfMetadata(metadata,nombre_de_tabla);//esto puede ser enviar metadata a memoria en lugar de imprimir etc  etc
	free(metadata);
}
void printfMetadata(Metadata_Tabla* metadata, const char* nombre_de_tabla){
		printf("tabla:%s,\n	particiones: %d \n	consistencia: %s \n	y tiempo de compactacion: %d \n",nombre_de_tabla,metadata->PARTITIONS,metadata->CONSISTENCY,metadata->COMPACTION_TIME);
}
void drop(const char* nombre_de_tabla){
	t_list* listaDeParticiones=obtenerParticiones(nombre_de_tabla);
	char* pathDeTabla = obtenerPathDeTabla(nombre_de_tabla);
	remove(pathDeTabla);
	free(pathDeTabla);
	list_destroy(listaDeParticiones);
}
//t_list* obtenerListaDeParticiones(const char* nombre_de_tabla){
//	t_list listaDeParticiones = list_create();
//	char* aux = obtenerPathDeTabla(nombre_de_tabla);
//	t_list* nombresDeParticiones = obtenerListadoDeSubArchivosCompleto(aux,".partition");//puede ser .bin tambien
//	Particion* obtenerParticion(const char* pathDeParticion){
//		obtenerParticionesNoTemporales()
//	}
//
//}
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
//void validarNombreTablaNoVacia(const char* nombreTabla){
//	validarCadenaNoVacia(nombreTabla, "El nombre de la tabla no puede ser vacío");
//}
//
//void validarTipoConsistencia(const char* tipoConsistencia){
//	validarCadenaNoVacia(tipoConsistencia, "El tipo de consistencia no puede ser vacío");
//	validarTipoConsitenciaExistente(tipoConsistencia);
//}

//void validarTipoConsitenciaExistente(const char* tipoConsistencia){
//	if(tipoConsistencia != STRONGCONSISTENCY || tipoConsistencia != EVENTUALCONSISTENCY){
//		printf("El tipo de consistencia no existe");
//	}
//}
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


}
//void metadata_destroy(Metadata_Tabla* unaMetadata){
//	free(unaMetadata->COMPACTION_TIME);
//	free(unaMetadata->CONSISTENCY);
//	free(unaMetadata->PARTITIONS);
//}

//--------------------------EJECUCIONES FIN


