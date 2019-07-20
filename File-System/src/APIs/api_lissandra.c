/*
 * api_lissandra.c
 *
 *  Created on: 16 may. 2019
 *      Author: jhon
 */
#include "api_lissandra.h"


void select1(const char * nombre_de_tabla, unsigned int key){//hasta ahora OK hasta la memtable
	if (!yaExisteTabla(nombre_de_tabla))perror("SELECT no existe tabla ");
//	Metadata_Tabla* unaMetadata=obtenerMetadata(nombre_de_tabla);
	t_list* select_listaDeRegistros=filtrarRegistrosEnLaMemtable(nombre_de_tabla,key);
//	t_list* select_listaDeRegistrosDeParticiones=obtenerRegistrosEnParticiones(nombre_de_tabla,key);
	bool my_maximo_timestamp(RegistroLinea* unRegistro,RegistroLinea* otroRegistro){//ok
		return unRegistro->timestamp<otroRegistro->timestamp;
	}
//	list_add_all(select_listaDeRegistros,select_listaDeRegistrosDeParticiones);
	list_sort(select_listaDeRegistros,my_maximo_timestamp);
	RegistroLinea* registroConMayorTimestamp = (RegistroLinea*)list_get(select_listaDeRegistros,0);
	puts("SELECT : ");
	registroLinea_mostrar(registroConMayorTimestamp);
//	list_iterate(select_listaDeRegistros,registroLinea_destroy);
//	list_destroy(select_listaDeRegistros);
}
t_list* obtenerRegistrosEnParticiones(const char* tabla,unsigned int key){
	t_list* listaDeParticiones=obtenerParticiones(tabla);
	t_list* listaDeListaDeRegistros=list_map(listaDeParticiones,particionToListaDeRegistros);
	void particionDestroy(Particion* p){
		free(p->pathParticion);
	}
	list_iterate(listaDeParticiones,particionDestroy);
	list_destroy(listaDeParticiones);

	t_list* listaDeRegistros=list_create();
	void my_juntarLista(void* listaDeRegistros_){
		if(listaDeRegistros_==NULL)return ;
		t_list* listaDeRegistros__=(t_list*)listaDeRegistros_;
		list_add_all(listaDeRegistros,listaDeRegistros_);
	}
	list_iterate(listaDeListaDeRegistros,my_juntarLista);
	bool buscarRegistroConKeyDado(RegistroLinea* unRegistro){
			return unRegistro->key==key;
	}
	t_list* listaFiltrada=list_filter(listaDeRegistros,buscarRegistroConKeyDado);
	list_iterate(listaDeRegistros,registroLinea_destroy);
	list_destroy(listaDeRegistros);
	return listaFiltrada;
}
t_list* particionToListaDeRegistros(Particion* particion){//pendiente
	if(particion->size==0)return NULL;
	char* contenido_de_particion=(char*)malloc(particion->size);//*sizeof(char));
//	memset(contenido_de_particion,0,particion->size);
	for(int i=0;particion->bloques[i]!=NULL;i++){
		char* pathBLoqueFS = obtenerPathDelNumeroDeBloqueFS(atoi(particion->bloques[i]));
		//file to string y luego append al char* contenido
		char* contenidoDeBloque=fileToString(pathBLoqueFS);
		strcpy(contenido_de_particion,contenidoDeBloque);
		free(pathBLoqueFS);
		free(contenidoDeBloque);
	}
	t_list* listaDeRegistro=particionStringToRegistro(contenido_de_particion);
	return listaDeRegistro;
}
t_list* particionStringToRegistro(const char* contenido){
	t_list* lista_r=list_create();
	char** registros_=string_split(contenido,"\n");
	for(int i=0;registros_[i]!=NULL;i++){
		RegistroLinea* registroLinea=stringToRegistro(registros_);
		if(registroLinea!=NULL)list_add(lista_r,registroLinea);
		free(registros_);
		printf("particionStringToRegistro(),registro obtenido \"%lu;%d;%s\"\n",registroLinea->timestamp,registroLinea->key,registroLinea->value);
	}
	return lista_r;
}
RegistroLinea* stringToRegistro(const char* registro_){
	if(strlen(registro_)<3){
		return NULL;
	}
	else {
		RegistroLinea* registroLinea = (RegistroLinea*)(malloc(sizeof(RegistroLinea)));
		registroLinea->value=(char*)malloc(lfs.tamanioValue);
		sscanf(registro_,"%lu;%d;%s",registroLinea->timestamp,registroLinea->key,registroLinea->value);
		return registroLinea;
	}
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
//		puts("insert_2() insertando datos");
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
//	printf("insertarEnMemtable() insert encontrado,tabla %s y particiones %d\n",insertDeMemtableConTablaCorrespondiente->nombreDeLaTabla,insertDeMemtableConTablaCorrespondiente->cantParticiones);
	if(insertDeMemtableConTablaCorrespondiente->registros==NULL)perror("insertarEnMemtable() error al aniadir registro");
	list_add((insertDeMemtableConTablaCorrespondiente->registros),unRegistroAInsertar);
	printf("insertarEnMemtable() %s -> %lu;%d;%s\n",nombre_de_tabla,unRegistroAInsertar->timestamp,unRegistroAInsertar->key,unRegistroAInsertar->value);
}
Insert* buscarTablaEnLaMemtable(const char * tabla){
	bool existeTablaEnLaMemtable(Insert *unInsert) {//simulo aplicacion parcial
		return strcmp(tabla,unInsert->nombreDeLaTabla)==0;//strcmp() ,string_equals_ignore_case(unInsert->nombreDeLaTabla, tabla);
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
t_list*  filtrarRegistrosEnLaMemtable(const char* tabla,uint16_t key){//ok
	bool my_buscarTablaDeInsert(Insert* unInsert){
		return string_equals_ignore_case(unInsert->nombreDeLaTabla,tabla);//strcmp(unInsert->nombreDeLaTabla,tabla)==0;
	}
	Insert* insert_conListaDeRegistrosDeTabla=list_find(memtable,my_buscarTablaDeInsert);
	printf("filtrarRegistrosEnLaMemtable() tabla %s y particiones temporales %d \n",insert_conListaDeRegistrosDeTabla->nombreDeLaTabla,insert_conListaDeRegistrosDeTabla->cantParticionesTemporales);
	bool buscarRegistroConKeyDado(RegistroLinea* unRegistro){
		return unRegistro->key==key;
	}
	return list_filter(insert_conListaDeRegistrosDeTabla->registros,buscarRegistroConKeyDado);
}
unsigned long long lfs_timestamp(){
	return (unsigned long long)time(NULL);
}

void insertarListaDeRegistrosDeTablaANuevaParticionTemporal(const Insert* unInsert){
	puts("insertarListaDeRegistrosDeTablaANuevaParticionTemporal()");
	size_t sizeTotalDeParticion=tamanioDeListaDeRegistros(unInsert->registros);
	printf("insertarListaDeRegistrosDeTablaANuevaParticionTemporal() SIZE = %d\n",sizeTotalDeParticion);
	char* pathDeParticionTemporal=obtenerPathDeParticionTemporal(unInsert->nombreDeLaTabla,unInsert->cantParticionesTemporales);
	t_list* bloquesNecesarios=calcularBloquesNecesarios(sizeTotalDeParticion);
	crearParticion(pathDeParticionTemporal,sizeTotalDeParticion,bloquesNecesarios);
	free(pathDeParticionTemporal);
	escribirRegistrosABloquesFS_v2(bloquesNecesarios,unInsert->registros);
	list_iterate(bloquesNecesarios,bloque_destroy);
	list_destroy(bloquesNecesarios);
	puts("insertarListaDeRegistrosDeTablaANuevaParticionTemporal( ) FIN ");
}
t_list* calcularBloquesNecesarios(size_t size_de_particion){//ok
	t_list* bloques = list_create();
	int cantidadDeBloques=size_de_particion/lfs_metadata.tamanio_de_bloque;
	if(size_de_particion%lfs_metadata.tamanio_de_bloque!=0)cantidadDeBloques++;
//	printf("calcularBloquesNecesarios() cantidad de bloques=%d, con SIZE de particion %d y tamanio bloque %d\n",cantidadDeBloques,size_de_particion,lfs_metadata.tamanio_de_bloque);
	for(int i=0;i<cantidadDeBloques;i++){
		Bloque_LFS* unBloque=lfs_obtenerBloqueLibre();
		list_add(bloques,unBloque);
//		printf("-----Bloque libre %d,path %s\n",unBloque->numero,unBloque->path);
	}

//	void bloquesLFS_mostrar(Bloque_LFS* bloque){
//		printf("	bloque FS numero:%d y path %s\n",bloque->numero,bloque->path);
//	}
	if(list_is_empty(bloques))perror("calcularBloquesNecesarios(), no hay bloques ");
//	puts("calcularBloquesNecesarios() mostrando bloques calculados ");
//	list_iterate(bloques,bloquesLFS_mostrar);
//	puts("fin calcular bloques necesarios");
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
	puts("crearParticion() fin ");
}
size_t tamanioDeListaDeRegistros(t_list* listaDeRegistros){//ok
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
char* obtenerPathDelNumeroDeBloqueFS(int numeroDeBloque){
	char* path_del_bloque=malloc(strlen(lfs.puntoDeMontaje)+strlen("/Bloques")+20);
	sprintf(path_del_bloque,"%sBloques/%d.bin",lfs.puntoDeMontaje,numeroDeBloque);
	printf("obtenerPathDelNumeroDeBloque() , path: %s\n",path_del_bloque);
	return path_del_bloque;
}

void escribirRegistrosABloquesFS_v2(const t_list* listaDeBloques,const t_list* listaDeRegistros){//ok
	unsigned int registro_actual=0;
	char* registroRestanteAEscribir=NULL;
	void my_ocuparBloqueConRegistros(Bloque_LFS* unBLoque){
		FILE* bloqueActual=fopen(unBLoque->path,"w");//txt_open_for_append(unBLoque->path);7
		if(bloqueActual==NULL)perror("escribirRegistrosABloquesFS_v2(), no existe el path ");
		fseek(bloqueActual,0,SEEK_SET);
		printf("my_ocuparBloqueConRegistros() path %s\n",unBLoque->path);
		while(registro_actual<list_size(listaDeRegistros) && lfs_metadata.tamanio_de_bloque>ftell(bloqueActual) ){
				if( registroRestanteAEscribir!=NULL){//si ya habia string timestamp;key;value por escribir
					if(strlen(registroRestanteAEscribir)>0){ //strlen de NULL falla
//						if(lfs_metadata.tamanio_de_bloque<ftell(bloqueActual))break;//goto proximo_bloque;
						fprintf(bloqueActual,registroRestanteAEscribir);
						printf("my_ocuparBloqueConRegistros() string a escribir \"%s\" y cantidad de caracteres file %d  \n",registroRestanteAEscribir,ftell(bloqueActual));
						free(registroRestanteAEscribir);
						registroRestanteAEscribir=NULL;//para que no tire error en free() o strlen de NULL
//						registro_actual++;
					}
				}
				unsigned int longitud_restante_para_escribir_en_bloque=lfs_metadata.tamanio_de_bloque-ftell(bloqueActual);//cantidadDeCaracteresDeFile(unBLoque->path);
				if(longitud_restante_para_escribir_en_bloque<=0)break;// goto proximo_bloque;//break;
				RegistroLinea* unRegistro=(RegistroLinea*)list_get(listaDeRegistros,registro_actual);
				char* registro_string=registroLineaAString(unRegistro);
				char* registroAEscribir =string_substring_until(registro_string,longitud_restante_para_escribir_en_bloque); //string_substring_until(unRegistroAux,longitud_restante_para_escribir_en_bloque);
//				registroRestanteAEscribir=string_substring_from(registro_string,longitud_restante_para_escribir_en_bloque);
				registro_actual++;
				registroRestanteAEscribir=NULL;
				//si se pasa de tamanio de bloque, recortar
				if(strlen(registro_string)>longitud_restante_para_escribir_en_bloque){
					registroRestanteAEscribir=string_substring_from(registro_string,longitud_restante_para_escribir_en_bloque);
					registro_actual--;
				}
				free(registro_string);
				fprintf(bloqueActual,registroAEscribir);
				printf("my_ocuparBloqueConRegistros() string a escribir \"%s\" y cantidad de caracteres file %d  \n",registroAEscribir,ftell(bloqueActual));
				free(registroAEscribir);
//				if(longitud_restante_para_escribir_en_bloque<=0)break;// goto proximo_bloque;//break;
		}
//		proximo_bloque:;
		ftruncate(fileno(bloqueActual),lfs_metadata.tamanio_de_bloque);
		txt_close_file(bloqueActual);
//		bloqueActual=NULL;
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
void select_mostrar_lista_de_registros(const t_list* listaDeRegistros,const char* tabla, unsigned int key ){//ok
	printf("SELECT mostrarListaDeRegistros() de tabla %s y key %d  \n", tabla,key);
	list_iterate(listaDeRegistros,registroLinea_mostrar);
}
void registroLinea_mostrar(RegistroLinea* unRegistro){
		printf("	timestamp;key;value-> %lu;%d;%s\n",unRegistro->timestamp,unRegistro->key,unRegistro->value);
}
//--------------------------EJECUCIONES FIN
