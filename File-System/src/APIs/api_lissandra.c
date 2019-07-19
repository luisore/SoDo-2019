/*
 * api_lissandra.c
 *
 *  Created on: 16 may. 2019
 *      Author: jhon
 */
#include "api_lissandra.h"


void select1(const char * nombre_de_tabla, unsigned int key){
	if (!yaExisteTabla(nombre_de_tabla))perror("SELECT no existe tabla ");
//	Metadata_Tabla* unaMetadata=obtenerMetadata(nombre_de_tabla);
	t_list* listaDeRegistros=list_create();
	list_add_all(listaDeRegistros,buscarRegistrosEnMemtable(nombre_de_tabla,key));
	select_mostrar_lista_de_registros(listaDeRegistros,nombre_de_tabla);
	list_iterate(listaDeRegistros,registroLinea_destroy);
	list_destroy(listaDeRegistros);
}


void insert_1(const char* nombre_de_tabla,unsigned int key , const char* value){
	insert_2(nombre_de_tabla,key,value,lfs_timestamp());
}
void insert_2(const char* nombre_de_tabla,unsigned int key , const char* value, unsigned long long timestamp){
	if(!yaExisteTabla(nombre_de_tabla)){
		fprintf(stderr,"INSERT no existe la tabla \"%s\" ",nombre_de_tabla);//\n
		perror("");
	}
	else {
//		if(hay_datos_a_dumpear())dumpear();//aca en lugar de if , se reemplaza con un semaforo , pues dumpear() es una seccion critica, con un mutex, solo hay un proceso dumpeo
		puts("insert_2() insertando datos");
		insertarEnMemtable( nombre_de_tabla, key , value,timestamp);
	}
}
void create(const char* nombre_de_tabla,const char* tipo_consistencia,unsigned int numero_de_particiones,unsigned int tiempo_de_compactacion ){
	lfs_log_info("crear tabla %s",nombre_de_tabla);
	if(yaExisteTabla(nombre_de_tabla)){

		puts("tabla existente");
	}
	else{
		crearTabla(nombre_de_tabla);
		crearMetadata_v2(nombre_de_tabla,tipo_consistencia,numero_de_particiones,tiempo_de_compactacion);
	    crearParticiones(nombre_de_tabla,numero_de_particiones);
	}

	lfs_log_info("fin crear tabla %s ",nombre_de_tabla);
}
//funciones lfs para que use el File System
void lfs_create(const char* nombre_de_tabla,const char* tipo_consistencia,const char*  numero_de_particiones,const char* tiempo_de_compactacion ){
	create(nombre_de_tabla,tipo_consistencia,atoi(numero_de_particiones),atoi(tiempo_de_compactacion));
}
void lfs_drop(const char* nombre_de_tabla){
	drop(nombre_de_tabla);
}
void lfs_insert1(const char* nombre_de_tabla,const char* key,const char* value ){
	insert_1(nombre_de_tabla,atoi(key),value);
}
void lfs_insert2(const char* nombre_de_tabla,const char* key, const char* value, const char* timestamp ){
	insert_2(nombre_de_tabla,atoi(key),value,atol(timestamp));
}
void lfs_describe(){
	describe1();
}
void lfs_describe2(const char* nombre_de_tabla){
	describe2(nombre_de_tabla);
}
void lfs_select1(const char* nombre_tabla,const char* key){
	select1(nombre_tabla,atoi(key));
}
void dumpear(){
	void my_insertarRegistros(Insert* unInsert){
			unInsert->cantParticionesTemporales++;//cuando hay un dump se baja de la memtable a una particion temporal .tmp, se crea una temporal nueva
			insertarListaDeRegistrosDeTablaANuevaParticionTemporal(unInsert);
	}
	list_iterate(memtable,my_insertarRegistros);
	memtable_reboot();
}
void insertarEnMemtable(const char* nombre_de_tabla,unsigned int key , const char* value,unsigned long long  timestamp){//la logica  es esta, ok
	Insert* insertDeMemtableConTablaCorrespondiente = buscarTablaEnLaMemtable(nombre_de_tabla);
	RegistroLinea* unRegistroAInsertar=(RegistroLinea*)malloc(sizeof(RegistroLinea));
			unRegistroAInsertar->key=key;
			unRegistroAInsertar->value=strdup(value);
			unRegistroAInsertar->timestamp;
			Metadata_Tabla* metadata_=obtenerMetadata(nombre_de_tabla);
			unRegistroAInsertar->particionCorrespondiente=key%metadata_->PARTITIONS;
			free(metadata_);
	if(insertDeMemtableConTablaCorrespondiente==NULL){//si es igual a NULL la tabla no esta en la memtable
		insertDeMemtableConTablaCorrespondiente =malloc(sizeof(Insert));
//		insertDeMemtableConTablaCorrespondiente->nombreDeLaTabla=strdup(nombre_de_tabla);
		strcpy(insertDeMemtableConTablaCorrespondiente->nombreDeLaTabla,nombre_de_tabla);
		insertDeMemtableConTablaCorrespondiente->cantParticionesTemporales=0;
		insertDeMemtableConTablaCorrespondiente->registros=list_create();
		list_add(memtable,insertDeMemtableConTablaCorrespondiente);
//		list_add((insertDeMemtableConTablaCorrespondiente->registros),unRegistro);
//		puts("insertarEnMemtable() creando un nuevo insert para insertar");
//		mostrar_insert(insertDeMemtableConTablaCorrespondiente);
	}
	//lista de registros nula
	if(insertDeMemtableConTablaCorrespondiente->registros==NULL)perror("insertarEnMemtable() error al aniadir registro");
	list_add((insertDeMemtableConTablaCorrespondiente->registros),unRegistroAInsertar);

}
Insert* buscarTablaEnLaMemtable(const char * tabla){
	bool existeTablaEnLaMemtable(Insert *unInsert) {//simulo aplicacion parcial
		return string_equals_ignore_case(unInsert->nombreDeLaTabla,tabla);//strcmp() ,string_equals_ignore_case(unInsert->nombreDeLaTabla, tabla);
	}
	return list_find(memtable,existeTablaEnLaMemtable);// // le puse (void*) por que en los tests lo manejan asi
}
bool hay_datos_a_dumpear(){
	return list_size(memtable)>0;//hay datos en la memtable
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
	void borrarBloqueSegunParticion(Particion* unaParticion){
		puts("aca llego ");
		char* pathDeBloque=malloc(strlen(lfs.puntoDeMontaje)+strlen("/Bloques")+strlen(unaParticion->pathParticion));//la longitud es grande pero necesito una longitud maxima para que no tire error de reserva de memoria
		for(int bloque_i=0;(unaParticion->bloques)[bloque_i]!=NULL;bloque_i++){
			sprintf(pathDeBloque,"%sBloques/%s",lfs.puntoDeMontaje,(unaParticion->bloques)[bloque_i]);
			puts("DROP-> compruebo path del bloque de FS, imprimo el path");
			puts(pathDeBloque);
			FILE* bloque=fopen(pathDeBloque,"w+");
			if(bloque==NULL)perror("DROP error en path del bloque");
			fclose(bloque);
		}
		free(pathDeBloque);
		free_list_of_strings(unaParticion->bloques);
	}
	t_list* listaDeParticiones=obtenerParticiones(nombre_de_tabla);
		char* pathDeTabla = obtenerPathDeTabla(nombre_de_tabla);
		puts(pathDeTabla);
		int remove_=remove(pathDeTabla);
		if(remove_==-1)perror("DROP:error en remove");
		if(remove_==0)perror("DROP:exito remove ");
		if(listaDeParticiones==NULL)perror("DROP error en lista de particiones");
		if(list_size(listaDeParticiones)==0){
			perror("DROP lista de particiones de particiones vacia");
			return;
		}
		free(pathDeTabla);
	list_iterate(listaDeParticiones,borrarBloqueSegunParticion);
	list_destroy(listaDeParticiones);
}
t_list*  buscarRegistrosEnMemtable(const char* tabla,uint16_t key){
	bool buscarTablaDeInsert(Insert* unInsert){
		return string_equals_ignore_case(unInsert->nombreDeLaTabla,tabla);//strcmp(unInsert->nombreDeLaTabla,tabla)==0;
	}
	Insert* insert_conListaDeRegistrosDeTabla=list_find(memtable,buscarTablaDeInsert);
	bool buscarRegistroConKeyDado(RegistroLinea* unRegistro){
		return unRegistro->key==key;
	}
	t_list* listaDeRegistrosDeLaMismaKey=list_filter(insert_conListaDeRegistrosDeTabla->registros,buscarRegistroConKeyDado);
	return listaDeRegistrosDeLaMismaKey;
}
unsigned long long lfs_timestamp(){
	return (unsigned long long)time(NULL);
}

void insertarListaDeRegistrosDeTablaANuevaParticionTemporal(const Insert* unInsert){
	puts("insertarListaDeRegistrosDeTablaANuevaParticionTemporal()");
	size_t sizeTotalDeParticion=tamanioDeListaDeRegistros(unInsert->registros);
	printf("insertarListaDeRegistrosDeTablaANuevaParticionTemporal() SIZE = %d\n",sizeTotalDeParticion);
	char* pathDeParticionTemporal=obtenerPathDeParticionTemporal(unInsert->nombreDeLaTabla,unInsert->cantParticionesTemporales);
	t_list* bloquesNecesarios=calcularBloquesNecesarios( sizeTotalDeParticion);
	crearParticion(pathDeParticionTemporal,sizeTotalDeParticion,bloquesNecesarios);
	free(pathDeParticionTemporal);
	escribirRegistrosABloquesFS(bloquesNecesarios,unInsert->registros);
	list_iterate(bloquesNecesarios,bloque_destroy);
	list_destroy(bloquesNecesarios);
	puts("insertarListaDeRegistrosDeTablaANuevaParticionTemporal( ) FIN ");
}
t_list* calcularBloquesNecesarios(size_t size_de_particion){
	t_list* bloques = list_create();
	int cantidadDeBloques=size_de_particion/lfs_metadata.tamanio_de_bloque;
	if(size_de_particion%lfs_metadata.tamanio_de_bloque!=0)cantidadDeBloques++;
	printf("calcularBloquesNecesario() cantidad de bloques=%d, con SIZE de particion %d y tamanio bloque %d\n",cantidadDeBloques,size_de_particion,lfs_metadata.tamanio_de_bloque);
	for(int i=0;i<cantidadDeBloques;i++)list_add(bloques,lfs_obtenerBloqueLibre);

	void bloquesLFS_mostrar(Bloque_LFS* bloque){
		printf("	bloque FS numero:%d y path %s\n",bloque->numero,bloque->path);
	}
	puts("calcularBloquesNecesarios() mostrando bloques calculados ");
	list_iterate(bloques,bloquesLFS_mostrar);
	puts("fin calcular bloques necesarios");
	return bloques;
}
void crearParticion(const char* pathDeParticion,int size, t_list* bloquesObtenidos){
	FILE* particion =fopen(pathDeParticion,"w+");
	fprintf(particion,"SIZE=%d\n",size);
	char* key_bloques = malloc(sizeof(char)*(strlen("[,]")+list_size(bloquesObtenidos)*4));//una longitud maxima
	memset(key_bloques,0,strlen(key_bloques));
	sprintf(key_bloques,"[");
	size_t cant_bloques=list_size(bloquesObtenidos);
	for(int i=0;i<cant_bloques;i++){
		int numero_de_bloque=((Bloque_LFS*)(list_get(bloquesObtenidos,i)))->numero;
		char* aux =string_itoa(numero_de_bloque);
		strcat(key_bloques,aux);
		free(aux);
		if(i+1==cant_bloques)break;//si ese fue el ultimo bloque
		strcat(key_bloques,",");
	}
	strcat(key_bloques,"]");
	printf("crearParticion(),key_bloques=%s\n",key_bloques);
	fprintf(particion,"BLOCKS=%s",key_bloques);
	free(key_bloques);
	fclose(particion);
//	void my_set_bloques(Bloque_LFS* bloque){
//		setear_bloque_ocupado_en_posicion(bloque->numero);
//	}
//	list_iterate(bloques,my_set_bloques);
}
size_t tamanioDeListaDeRegistros(t_list* listaDeRegistros){
	size_t  size_de_particion =0;
	void sumarSize(RegistroLinea* unRegistro){
		 size_de_particion+=longitudDeRegistroAlFileSystem(unRegistro);
	}
	list_iterate(listaDeRegistros,sumarSize);
	puts("tamanioDeListaDeRegistros()");
	return size_de_particion;
}
size_t longitudDeRegistroAlFileSystem(const RegistroLinea* unRegistro){
	size_t longitud;
	char* aux=malloc(sizeof(unRegistro->timestamp)+sizeof(unRegistro->key)+strlen(unRegistro->value)+1000);
	sprintf(aux,"%d;%d;%s\n",unRegistro->timestamp,unRegistro->key,unRegistro->value);
	longitud=strlen(aux);
	free(aux);
	return longitud;
}
char* registroLineaAString(RegistroLinea* registro){
	char* registro_string=malloc(longitudDeRegistroAlFileSystem(registro));
	sprintf(registro_string,"%lu;%d;%s\n",registro->timestamp,registro->key,registro->value);
	return registro_string;
}

char* obtenerPathDeParticionTemporal(const char* tabla, unsigned int  numeroDeParticionTemporal){
	char* pathDeParticion=malloc(strlen(lfs.puntoDeMontaje)+strlen("/Tables/")+strlen(tabla)+100);
	sprintf(pathDeParticion,"%sTables/%s/%d.tmp",lfs.puntoDeMontaje,tabla,numeroDeParticionTemporal);
	printf("obtenerPathDeParticionTemporal() path:%s\n",pathDeParticion);
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
//destroy y printfs
void memtable_reboot(){
	list_iterate(memtable,insert_destroy);
	list_destroy(memtable);
	memtable=list_create();
}
void insert_destroy(Insert* unInsert){
	list_iterate(unInsert->registros,registroLinea_destroy);
	list_destroy(unInsert->registros);
//	free(unInsert->nombreDeLaTabla);
//	free(unInsert);
}
void registroLinea_destroy(RegistroLinea* unRegistro){
	free(unRegistro->value);
}
//int  grabarRegistroABloques(RegistroLinea* unRegistro){
//	int posicionBloqueLibre = getBloqueLibre_int();
//	char* bloqueLibre_path=obtenerPathDelNumeroDeBloque(posicionBloqueLibre);
//	escribirRegistrosABloquesFS(bloqueLibre_path,unRegistro);
//	free(bloqueLibre_path);
//	return posicionBloqueLibre;
//}
char* obtenerPathDelNumeroDeBloque(int numeroDeBloque){
	char* path_del_bloque=malloc(strlen(lfs.puntoDeMontaje)+strlen("/Bloques")+20);
	sprintf(path_del_bloque,"%sBloques/%d.bin",lfs.puntoDeMontaje,numeroDeBloque);
	lfs_log_info("obtenerPathDelNumeroDeBloque() , path: %s\n",path_del_bloque);
	return path_del_bloque;
}

void escribirRegistrosABloquesFS(const t_list* listaDeBloques ,const t_list* listaDeRegistros){
	FILE* bloque_actual;
	char* registroRestante=NULL;
	for(int bloque_i=0, registro_i=0;bloque_i<list_size(listaDeBloques);bloque_i++){
		Bloque_LFS* bloque = list_get(listaDeBloques,bloque_i);
		bloque_actual=txt_open_for_append(bloque->path);
		while(registro_i<list_size(listaDeRegistros)){
			RegistroLinea* unRegistro = list_get(listaDeRegistros,registro_i);
			//si hay contenido restante , entonces
			if( registroRestante!=NULL){
						fprintf(bloque_actual,registroRestante);
						free(registroRestante);
						registroRestante=NULL;//para que no tire error en free()
			}
			char* registroAEscribir =registroLineaAString(unRegistro);
			int longitudRestanteParaEscribir=lfs_metadata.tamanio_de_bloque-cantidadDeCaracteres_file(bloque_actual);
			//si no hay espacio en bloque
			if(longitudRestanteParaEscribir<=0){//ok
				lfs_log_error("escribirRegistroABLoque(), longitud negativa o no hay espacio en bloque");
				registroRestante=registroAEscribir;
//				goto alSiguienteBloque;
				break;
			}
			//si el registro %lu;%d;s es mayor que el espacio disponible,hay que recortar el string  registro
			if( !strlen(registroAEscribir)<=longitudRestanteParaEscribir){
				registroRestante=string_substring_from(registroAEscribir,longitudRestanteParaEscribir);
				char* primeros_caracteres_a_escribir=malloc(sizeof(char)*longitudRestanteParaEscribir);
				strncpy(primeros_caracteres_a_escribir,registroAEscribir,longitudRestanteParaEscribir);
				fprintf(bloque_actual,primeros_caracteres_a_escribir);
				free(primeros_caracteres_a_escribir);
//				goto alSiguienteBloque;
				break;
			}
			fprintf(bloque_actual,registroAEscribir);
			free(registroRestante);
			free(registroAEscribir);
			registroRestante=NULL;
			registroAEscribir=NULL;
			registro_i++;
		}
//		alSiguienteBloque:;
		txt_close_file(bloque);
	}
	free(registroRestante);
}
void escribirRegistrosABloquesFS_v2(const t_list* listaDeBloques,const t_list* listaDeRegistros){
	FILE* bloqueActual=NULL;
	char* registroRestanteAEscribir=NULL;
	unsigned int registro_actual=0;
	void my_ocuparBloqueConRegistros(Bloque_LFS* unBLoque){
		bloqueActual=txt_open_for_append(unBLoque->path);
		while(registro_actual<list_size(listaDeRegistros)){
				if( registroRestanteAEscribir!=NULL){//si ya habia string timestamp;key;value por escribir
						fprintf(bloqueActual,registroRestanteAEscribir);
						registroRestanteAEscribir=NULL;//para que no tire error en free() o strlen de NULL
						registro_actual++;
				}
				RegistroLinea* unRegistro=list_get(listaDeRegistros,registro_actual);
				char* registro_para_escribir=registroLineaAString(unRegistro);
				unsigned int longitud_restante_para_escribir_en_bloque=lfs_metadata.tamanio_de_bloque-cantidadDeCaracteresDeFile(unBLoque->path);
				fprintf(bloqueActual,registro_para_escribir);
				registro_actual++;
				if(longitud_restante_para_escribir_en_bloque==0)break;
				//si se pasa de tamanio de bloque, recortar
				if(!longitud_restante_para_escribir_en_bloque>=strlen(registro_para_escribir)){
					ftruncate(fileno(bloqueActual),lfs_metadata.tamanio_de_bloque);
					registroRestanteAEscribir=string_substring_from(registro_para_escribir,longitud_restante_para_escribir_en_bloque);
					registro_actual--;
				}
		}
		txt_close_file(bloqueActual);
	}

	list_iterate(listaDeBloques,my_ocuparBloqueConRegistros);
}

int particionSegunKey(RegistroLinea* unRegistro,unsigned int cantidad_de_particiones){
	return unRegistro->key % cantidad_de_particiones;
}


void crearTabla(const char* nombreDeTabla){//ok
	char* aux_path_de_la_tabla = malloc(strlen(nombreDeTabla)+strlen("Tables/")+strlen(lfs.puntoDeMontaje));
	sprintf(aux_path_de_la_tabla,"%s%s%s",lfs.puntoDeMontaje,"Tables/",nombreDeTabla);
//	printf("path de tabla %s es \"%s\" \n",nombreDeTabla,aux_path_de_la_tabla);
	if(mkdir(aux_path_de_la_tabla, S_IRWXU) == -1){
		lfs_log_error("No se pudo crear directorio para tabla  \"%s\" o ya existe \n" ,nombreDeTabla);
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

bool  yaExisteTabla(const char* nombre_de_tabla){
	char* path = obtenerPathDeTabla(nombre_de_tabla);
	bool yaExiste=yaExisteCarpeta(path);
	free(path);
	return yaExiste;
}
char*  obtenerPathDeTabla(const char* nombre_de_tabla){
	char* path_aux = malloc(strlen(nombre_de_tabla)+strlen("Tables/")+strlen(lfs.puntoDeMontaje));
	sprintf(path_aux,"%s%s%s",lfs.puntoDeMontaje,"Tables/",nombre_de_tabla);
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
//void validarCadenaNoVacia(const char cadena, const char mensajeError){
//	if(string_is_empty(cadena)){
//		printf(mensajeError);
//		return ;
//	}
//}

bool yaExisteCarpeta(const char* path_tabla){
	bool existe=false;
	DIR *directorio = opendir(path_tabla);
	if(directorio != NULL){
		existe = true;
		closedir(directorio);
		return true;
	}
	lfs_log_info("ya existe la carpeta %s ",path_tabla);
	closedir(directorio);
	existe = false;
	return existe;
}
void mostrarMetadata(const char* tabla){ //ok
	//obtener metadata
	Metadata_Tabla *unMetadata=obtenerMetadata(tabla);
	printf("COMPACTION_TIME = %d  \n",unMetadata->COMPACTION_TIME);
	printf("CONSISTENCY= %s \n",unMetadata->CONSISTENCY);
	printf("PARTITIONS= %d \n",unMetadata->PARTITIONS);
	free(unMetadata);
}


void mostrarParticion(Particion* particion){//ok
	if(particion->esTemporal)printf("particion = %s \n	size= %d \n	y es temporal y un bloque es %s\n",particion->pathParticion,particion->size,particion->bloques[0]);
	else printf("particion = %s \n	size= %d \n	y no es temporal y un bloque es %s\n",particion->pathParticion,particion->size,particion->bloques[0]);
}
void memtable_mostrar(){//ok
	list_iterate(memtable,mostrar_insert);
}
void mostrar_insert(const Insert* unInsert){//ok
		printf("MOSTRANDO MEMTABLE para TABLA:%s y PARTICIONES TEMPORALES:%d \n",unInsert->nombreDeLaTabla,unInsert->cantParticionesTemporales);
		if(list_size(unInsert->registros)==0){
			perror("memtableMostrar() no hay registros");
			return;
		}
		list_iterate(unInsert->registros,registroLinea_mostrar);
}
void select_mostrar_lista_de_registros(const t_list* listaDeRegistros,const char* tabla){//ok
	printf("SELECT mostrarListaDeRegistros() de tabla %s  \n", tabla);
	list_iterate(listaDeRegistros,registroLinea_mostrar);
}
void registroLinea_mostrar(RegistroLinea* unRegistro){
		printf("	timestamp;key;value-> %lu;%d;%s\n",unRegistro->timestamp,unRegistro->key,unRegistro->value);
}
//--------------------------EJECUCIONES FIN
