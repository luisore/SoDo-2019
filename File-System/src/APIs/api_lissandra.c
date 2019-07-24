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
void insert_2(const char* nombre_de_tabla,unsigned int key , const char* value, unsigned long long timestamp){
	if(!yaExisteTabla(nombre_de_tabla)){
		fprintf(stderr,"no exixte la tabla \"%s\" \n",nombre_de_tabla);
	}
	else {
		if(hay_datos_a_dumpear())dumpear();//aca en lugar de if , se reemplaza con un semaforo , pues dumpear() es una seccion critica, con un mutex, solo hay un proceso dumpeo
		insertarEnMemtable( nombre_de_tabla, key , value,timestamp);
	}
}
void insertarEnMemtable(const char* nombre_de_tabla,unsigned int key , const char* value,unsigned long long timestamp){//la logica  es esta, ok
	Insert* unInsert = buscarTablaEnLaMemtable(nombre_de_tabla);
	RegistroLinea* unRegistro=malloc(sizeof(RegistroLinea));
			unRegistro->key=key;
			printf("value largo %d: " ,strlen(value));
			unRegistro->value=malloc(strlen(value));
			strcpy(unRegistro->value,value);
			unRegistro->timestamp=timestamp;
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
Insert* buscarTablaEnLaMemtable(char * tabla){
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
void insertarRegistrosEnParticionTemporal(const char* tabla,  RegistroLinea* unRegistro,int cantidadDeParticiones){
	Metadata_Tabla* metadataDeTabla = obtenerMetadata(tabla);
	unsigned int particionCorrespondiente = particionSegunKey(unRegistro,metadataDeTabla->PARTITIONS);
	char* path_de_particion_temporal=path_particiones(tabla,particionCorrespondiente);
	printf("particiones de la key %s:\n",path_de_particion_temporal);

	t_config * unaconfig= config_create(path_de_particion_temporal);
	int size=config_get_int_value(unaconfig,"SIZE");
	int cantidadDeBloquesDelaParticion = cantidadDeBloques(config_get_array_value(unaconfig,"BLOCKS"));
	printf("cantidad de bloques %d:\n",cantidadDeBloquesDelaParticion);

	//int cantidadDeBloquesNecesariosParaGrabar=cantidadDeBloquesNecesario();
	char *actualizar = "20";
	config_set_value(unaconfig, "SIZE",actualizar);
	config_save(unaconfig);
	config_destroy(unaconfig);

	//escribirAParticion(path_de_particion_temporal,unRegistro);
	//int bloqueGrabado=grabarRegistroABloques(unRegistro);
//	free(metadataDeTabla);
}
void escribirAParticion(const char* path_particion,RegistroLinea* registro){
	size_t longitud_usada=longitudDeRegistroAlFileSystem(registro);
	if(validarArchivoConfig(path_particion)!=-1 ){
		printf("existe el archivo");
	}
	else{
		printf("no existe el archivo");
	}
	char *path="/home/utnso/LISSANDRA_FS/Tables/materias/Metadata.metadata";
	t_config* particion = config_create(path_particion);
	//config_set_value(particion,"PARTITIONS",string_itoa(longitud_usada));
	//config_set_value(config_particion,"SPARTITIONS",string_itoa(longitud_usada));
	config_save(particion);
	config_destroy(particion);
}
//VER longitud
size_t longitudDeRegistroAlFileSystem(RegistroLinea* unRegistro){
	size_t longitud;
	longitud = sizeof(unRegistro->timestamp)+sizeof(unRegistro->key)+strlen(unRegistro->value)+3;
	return longitud;
}
char* registroLineaAString(RegistroLinea* registro){
	char* registro_string=malloc(longitudDeRegistroAlFileSystem(registro));
	//sprintf(registro_string,"%d;%d;%s\n",registro->timestamp,registro->key,registro->value);
	return registro_string;
}

char* obtenerPathDeParticionTemporal(int particion,char *tabla){
	char* pathDeParticionTempora=malloc(strlen(lfs.puntoDeMontaje)+strlen("/Tables/")+strlen(tabla));
	strcpy(pathDeParticionTempora,lfs.puntoDeMontaje);
	strcat(pathDeParticionTempora,"Tables/");
	strcat(pathDeParticionTempora,tabla);
	strcat(pathDeParticionTempora,"/");
	strcat(pathDeParticionTempora,string_itoa(particion));
	strcat(pathDeParticionTempora,".bin");
	printf("path particion temporal %s:\n",pathDeParticionTempora);
	//sprintf(pathDeParticion,"%sTables/%d.tmp");
	return pathDeParticionTempora;
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
int  grabarRegistroABloques(RegistroLinea* unRegistro){
	int posicionBloqueLibre = getBloqueLibre_int();
	char* bloqueLibre_path=obtenerPathDelNumeroDeBloque(posicionBloqueLibre);
	//escribirRegistroABloque(bloqueLibre_path,unRegistro);

	t_config *config=config_create(bloqueLibre_path);
	int size=config_get_int_value(config,"SIZE");
	config_destroy(config);
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
void create(char* nombre_de_tabla,char* tipo_consistencia,unsigned int numero_de_particiones,unsigned int tiempo_de_compactacion ){
	puts("crear tabla");
	if(yaExisteTabla(nombre_de_tabla)){
		puts("tabla existente");
		//crearParticiones(nombre_de_tabla,numero_de_particiones);
	}
	else{
		crearTabla(nombre_de_tabla);
		crearMetadata_v2(nombre_de_tabla,tipo_consistencia,numero_de_particiones,tiempo_de_compactacion);
	    crearParticiones(nombre_de_tabla,numero_de_particiones);
	    crearHiloCompactacion(nombre_de_tabla,numero_de_particiones,tiempo_de_compactacion);
	}

	puts("fin crear tabla");
}

void lfs_create(const char* nombre_de_tabla,const char* tipo_consistencia,const char*  numero_de_particiones,const char* tiempo_de_compactacion ){
	create(nombre_de_tabla,tipo_consistencia,atoi(numero_de_particiones),atoi(tiempo_de_compactacion));
}

void crearTabla(const char* nombreDeTabla){//ok

	char* aux_path_de_la_tabla = malloc(strlen(nombreDeTabla)+strlen("Tables/")+strlen(lfs.puntoDeMontaje));
	//sprintf(aux_path_de_la_tabla,"%s%s%s",lfs.puntoDeMontaje,"Tables/",nombreDeTabla);
	strcpy(aux_path_de_la_tabla,lfs.puntoDeMontaje);
	strcat(aux_path_de_la_tabla,"Tables/");
	strcat(aux_path_de_la_tabla,nombreDeTabla);
	printf("path %s \n",aux_path_de_la_tabla);
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
	fclose(unArchivo);
	t_config *archivo_MetaData=config_create(path_metadata);
	//config_get_set_value(archivo_MetaData,"BLOCKS");
	char *value ="0";
	config_set_value(archivo_MetaData, "CONSISTENCY",tipoConsistencia);
	config_set_value(archivo_MetaData, "PARTITIONS",string_itoa(numeroParticiones));
	config_set_value(archivo_MetaData, "COMPACTION_TIME",string_itoa(tiempoCompactacion));
	config_save(archivo_MetaData);
	config_destroy(archivo_MetaData);
	//lfsmetadata.magicNumber=string_duplicate(config_get_string_value(archivo_MetaData,"MAGIC_NUMBER"));
	//lfsmetadata.tamanioBloque=config_get_int_value(archivo_MetaData,"BLOCK_SIZE");

	/*free(path_metadata);
	fprintf(unArchivo,"CONSISTENCY=%s \n",tipoConsistencia);
	fprintf(unArchivo,"PARTITIONS=%d \n",numeroParticiones );
	fprintf(unArchivo,"COMPACTION_TIME=%d ",tiempoCompactacion);
	*/



//	CONSISTENCY=SC
//	PARTITIONS=3
//	COMPACTION_TIME=60000
}
void crearParticiones(const char* tabla, unsigned int numeroDeParticiones){
	char* path_tabla=obtenerPathDeTabla(tabla);
	char* path_particion=malloc(strlen(path_tabla)+15);
	for (int numeroDeParticion = 0; numeroDeParticion < numeroDeParticiones; numeroDeParticion++) {
		sprintf(path_particion,"%s/%d.bin",path_tabla,numeroDeParticion);
		FILE* particion = fopen(path_particion,"w+r");
		fclose(particion);
		t_config *archivo = config_create(path_particion);
		char *size=string_itoa(0);
		config_set_value(archivo, "SIZE",size);
		char *vacio = "[]";
		config_set_value(archivo, "BLOCKS",vacio);
		config_save(archivo);
		config_destroy(archivo);
		//fprintf(particion,"SIZE=0\n");


	}
	free(path_tabla);
	free(path_particion);
	pthread_mutex_lock (&mMemtable);
	Insert *agregar_tabla = malloc(sizeof(Insert));
	agregar_tabla->cantParticionesCompactacion = 0;
	agregar_tabla->cantParticionesTemporales = 0;
	agregar_tabla->nombreDeLaTabla = strdup(tabla);
	agregar_tabla->registros = list_create();
	list_add(memtable,agregar_tabla);
	pthread_mutex_unlock (&mMemtable);
}
char *path_tables(){
	char *direccionArchivoMedata=(char *) malloc(1 + strlen(lfs.puntoDeMontaje) + strlen("Tables") +strlen("/Metadata/Metadata.bin"));;
	strcpy(direccionArchivoMedata,lfs.puntoDeMontaje);
	string_append(&direccionArchivoMedata,"Tables");
	return direccionArchivoMedata;
}

char *path_bloques(){
	char *direccionArchivoMedata=(char *) malloc(1 + strlen(lfs.puntoDeMontaje) + strlen("Bloques/"));;
	strcpy(direccionArchivoMedata,lfs.puntoDeMontaje);
	string_append(&direccionArchivoMedata,"Bloques/");
	return direccionArchivoMedata;
}


void guardar_en_temporal(Insert *insert,RegistroLinea *registro){
	char *path_tabla_particion = path_tables();
	string_append(&path_tabla_particion,"/");
	string_append(&path_tabla_particion,insert->nombreDeLaTabla);
	string_append(&path_tabla_particion,"/");
	string_append(&path_tabla_particion,"0.partition");

	printf("path para guardar en particio %s:\n",path_tabla_particion);

	char *path_tabla_bloque = path_bloques();
	string_append(&path_tabla_bloque,"0.bin");

	printf("path para guardar en bloque %s:\n",path_tabla_bloque);


	char *guardar = malloc(sizeof(registro->timestamp)+strlen(";")+sizeof(registro->key)+strlen(";")+sizeof(registro->timestamp));
	int offset=0;
	//offset += sizeof(registro->timestamp);
	//offset += strlen(registro->value);
	//memcpy(guardar,registro->value,strlen(registro->value));
	offset = offset + strlen(";");
	memcpy(guardar+offset,";",strlen(";"));
	//offset += sizeof(registro->key);
	//memcpy(guardar+offset,&registro->key,of(registro->key));
	offset =offset + strlen(";");
	memcpy(guardar+offset,";",strlen(";"));

	offset =offset +  sizeof(registro->value)+1;
	//registro->value[strlen(registro->value)]='\0';
	memcpy(guardar+offset,registro->value,strlen(registro->value));
	offset =offset + strlen("\n");
	memcpy(guardar+offset,"\n",strlen("\n"));

	FILE* particion = fopen(path_tabla_bloque,"wr");
	fwrite(guardar , 1 , sizeof(guardar) , particion );
	//fwrite(particion,guardar);
	int numero_particion  = buscarParticion(insert->nombreDeLaTabla,registro->key);

	fclose(particion);
}

int buscarParticion(char *tabla , unsigned int key)
{
	Metadata_Tabla* tabla_metadata =obtenerMetadata(tabla);
	int particion ;
	particion = key % tabla_metadata->PARTITIONS;
	return particion;
}


void describe1();
void describe2(const char* nombre_de_tabla);
void drop(char* nombre_de_tabla){
	pthread_mutex_lock (&mMemtable);
	//Busca el elemento
	Insert* unInsert = buscarTablaEnLaMemtable(nombre_de_tabla);
	if(unInsert==NULL){
		log_error(logger,"no existe la tabla");
		pthread_mutex_unlock (&mMemtable);
		return;
	}

	bool _eliminarElemento(Insert* insert) {
	                    return string_equals_ignore_case(insert->nombreDeLaTabla, unInsert->nombreDeLaTabla);
	}
	void _eliminarRegistro(RegistroLinea* registro) {
		               free(registro->value);
		               free(registro);
	}

	//Elimina de la memtable
	Insert* remover =list_remove_by_condition(memtable, (void*) _eliminarElemento);
	log_info(logger,"Eliminar de la Memtable: %s",remover->nombreDeLaTabla);
	log_error(logger,"Eliminar de la Memtable: %d",list_size(remover->registros));
	list_destroy_and_destroy_elements(remover->registros, (void*)_eliminarRegistro);
	free(remover);


	pthread_mutex_unlock(&mMemtable);

	bool condicion_busqueda(ejecucion*	ejecutar) {
	                    return string_equals_ignore_case(ejecutar->nombre_tabla, nombre_de_tabla);
	}

	//Busco los mutex de la tabla
	pthread_mutex_lock (&mEjecucion);
	ejecucion*	ejecutar=list_find(lista_ejecucion,condicion_busqueda);
	if(ejecutar ==NULL){
		printf("hay un grave porblema:\n");
	}
	log_info(logger, "se esta ejecutando tabla: %s",ejecutar->nombre_tabla );

	//Inicio de borrado de estructuras
	pthread_mutex_lock (&ejecutar->mtabla);
	log_info(logger, "Iniciando borrado de estructura  de Directorios para tabla: %s",ejecutar->nombre_tabla );
	borrar_estructura(ejecutar->nombre_tabla);
	pthread_mutex_unlock (&ejecutar->mtabla);

	pthread_mutex_unlock(&mEjecucion);



	printf("cantidad memtable %d\n",list_size(memtable));
	pthread_mutex_unlock (&mMemtable);
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

void crearHiloCompactacion(char *nombre_de_tabla,int numero_de_particiones,int tiempo_de_compactacion){
	pthread_t hilo_compactacion;
	struct_create *registro_create = malloc(sizeof(struct_create));
	registro_create->nombreTabla = strdup(nombre_de_tabla);
	registro_create->tiempoCompactacion = tiempo_de_compactacion;
	registro_create->numeroParticiones =  numero_de_particiones;

	pthread_create (&hilo_compactacion, NULL, compactacion, (void*)registro_create) ;
	pthread_detach(hilo_compactacion);

}

void* compactacion(void *registro_create){

	struct_create  *create_compactacion = (struct_create  *) registro_create;
	log_info(logger, "Creando hilo Compactacion para Tabla:%s",create_compactacion->nombreTabla );
	while (1){
		usleep(create_compactacion->tiempoCompactacion*1000);
		log_info(logger, "Iniciando compactacion de la Tabla:%s",create_compactacion->nombreTabla );
		//agregar mutex
		pthread_mutex_lock (&mMemtable);
		Insert* unInsert = buscarTablaEnLaMemtable(create_compactacion->nombreTabla);

		//agregar mutex
		if(unInsert==NULL){
			pthread_mutex_unlock (&mMemtable);
			break;
		}
		pthread_mutex_unlock (&mMemtable);
		log_info(logger, "La tabla aun existe , procediendo con la compactacion de la Tabla: %s",create_compactacion->nombreTabla );
		t_list *lista;
		lista=list_create();
		for(int i=0;i<create_compactacion->numeroParticiones;i++){
			registroCompactacion* registro = malloc(sizeof(registroCompactacion));
			registro->numeroParticion = i;
			registro->numeroParticion = list_create();
			char * pathDeparticion = path_particiones(create_compactacion->nombreTabla,i);
			printf("particiones %s:\n",pathDeparticion);
			free(pathDeparticion);
			list_add(lista,registro);
		}
		log_info(logger, "Compactacion Terminada de la Tabla:%s",create_compactacion->nombreTabla );

		printf("cantidad de particiones %d de la tabla: %s \n:",list_size(lista),unInsert->nombreDeLaTabla);

	}
	log_error(logger, "La tabla %s ya no existe",create_compactacion->nombreTabla );
	//free(create_compactacion->nombreTabla);
	free(create_compactacion);
	return NULL;

}

char *path_particiones(char *nombreTabla,int particion){
	char *path = malloc(strlen(lfs.puntoDeMontaje));
	strcpy(path,lfs.puntoDeMontaje);
	string_append(&path,"Tables/");
	string_append(&path,nombreTabla);
	string_append(&path,"/");
	string_append(&path,string_itoa(particion));
	string_append(&path,".bin");
	return path;
}
void borrar_estructura(char *nombreTabla){
	Metadata_Tabla *unaMetadata = obtenerMetadata(nombreTabla);
	char *pathDeTabla =path_tables();
	DIR *dip;
    struct dirent   *dit;
    printf("\n xxxxxxxxxxxTablaxxxxxxxxx: %s\n",pathDeTabla);
    string_append(&pathDeTabla,"/");
    string_append(&pathDeTabla,nombreTabla);
    printf("\n xxxxxxxxxxxTablaxxxxxxxxx: %s\n",pathDeTabla);
    if ((dip = opendir(pathDeTabla)) == NULL)
    {
              perror("opendir");

    }

    printf("Verificando si hay tablas ya cargadas\n");

    while((dit = readdir(dip)) != NULL){
    	if((strcmp(dit->d_name, ".") != 0) && (strcmp(dit->d_name, "..") != 0) && (strcmp(dit->d_name, "Metadata.metadata") != 0) ){
    				 log_info(logger,"nombre",dit->d_name);
    				 printf("nombre del archivo: %s",dit->d_name);
    				 printf("\n Path: %s\n",dit->d_name);
    				 switch (dit->d_type) {
    				  case DT_DIR:{
    					  log_error(logger,"Es un directorio");
    					  log_info(logger,"Es un directorio",dit->d_name);
    					  break;
    					  //return;
    				  }
    				  case DT_REG:{
    					  log_error(logger,"Es un archivo");
    					  log_info(logger,"Es un archivo",dit->d_name);
    					  char *path_bloque = strdup(pathDeTabla);
    					  string_append(&path_bloque,"/");
    					  string_append(&path_bloque,dit->d_name);
    					  liberar_bloque(path_bloque);
    					  break;
    					  //return;
    				  }

    				 }
    	        	if (dit->d_type  == DT_REG )
    	        	{
    	        		log_error(logger,"Borrar los siguientes archivos");
    	        	}
    	 }
    }

    if (closedir(dip) == -1)
    {
            perror("cerrado directorio");

    }


    /*
	int cantidad_particiones = unaMetadata->PARTITIONS;
	for(int i=0;i<cantidad_particiones;i++){
		char * path = path_particiones(nombreTabla,i);
		free(path);
	}
	*/
}

void liberar_bloque(char *path_bloque){
	t_config * unaconfig= config_create(path_bloque);
	char **estructura_bloque = config_get_array_value(unaconfig,"BLOCKS");
	int cantidadDeBloquesALiberar = cantidadDeBloques(estructura_bloque);
	for(int i=0;i<cantidadDeBloquesALiberar;i++){
		int numeroBloque =atoi(estructura_bloque[i]);
		log_info(logger,"iniciado liberacion de bloque; %d",numeroBloque);
		//liberarDelBitmap
		//Borrar contenido
	}
	config_destroy(unaconfig);
}


void crear_estructura_ejecucion(char *nombreTabla){

	pthread_mutex_lock (&mEjecucion);
	//char * tabla = string_duplicate(nombreTabla);
	ejecucion *ejecutar = malloc(sizeof(ejecucion));
	ejecutar->nombre_tabla = string_duplicate(nombreTabla);
	pthread_mutex_init (&ejecutar->mtabla, NULL);

	list_add(lista_ejecucion,ejecutar);
	printf("cantidad de elementos: %d \n:",list_size(lista_ejecucion));
	pthread_mutex_unlock (&mEjecucion);

}

char *archivo_path(char* rutaMontaje,char *rutaArchivo){

	char*  complete_path = malloc(1 + strlen(rutaMontaje) + strlen(rutaArchivo));
    strcpy(complete_path, rutaMontaje);
    strcat(complete_path, rutaArchivo);
    return complete_path;
}
//
////--------------------------VALIDACIONES INICIO
/*void validarCadenaNoVacia(const char* cadena, const char* mensajeError){
	if(string_is_empty(cadena)){
		printf("Error de cadena%s\n",mensajeError);
		return ;
	}
}
*/
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
	}
	closedir(directorio);
	existe = false;
	return existe;
}

void mostrarMetadata(const char* tabla){ //ok
	//obtener metadata
	Metadata_Tabla *unMetadata=obtenerMetadata(tabla);
	printf("COMPACTION_TIME = %ul  \n",unMetadata->COMPACTION_TIME);
	printf("CONSISTENCY= %s \n",unMetadata->CONSISTENCY);
	printf("PARTITIONS= %d \n",unMetadata->PARTITIONS);
	free(unMetadata);

}


int cantidadDeBloques (char **bloque){
	int i=0;
	while (bloque[i]!=0){
		i++;
	}
	return i;
}

int cantidadDeBloquesNecesario(int espacioAgrabar){
	int i=1;
	while (i*lfs.tamanioValue<espacioAgrabar){
		i++;
	}
	return i;
}

//void metadata_destroy(Metadata_Tabla* unaMetadata){
//	free(unaMetadata->COMPACTION_TIME);
//	free(unaMetadata->CONSISTENCY);
//	free(unaMetadata->PARTITIONS);
//}

//--------------------------EJECUCIONES FIN


