/*
 ============================================================================
 Name        : File-System.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "File-System.h"
pthread_t consola,dump;//,compactador;

//typedef struct{
//	char pathParticion[256];
//	bool esTemporal;
//}ParticionNombre;

int main(void) {
	puts("... INICIA LFS ...");
	config_cargar("LFS.config");
	log_info(lfs_logger,"se cargo LFS.log \n");
	memtable=list_create();

	imprimir_configuracion();

//	pthread_create(&consola,NULL,lfs_consola,NULL);
//	pthread_join(consola,NULL);

//	lfs_consola();//por el momento funciona con el CREATE

//	system("rmdir src/punto_de_montaje_FS_LISSANDRA_ejemplo/Tables/tableA");
	mostrar_bitarray();
	insert_1("tableA",12,"esto es basura");
	insert_1("tableA",13,"esto es basura2");
	insert_1("tableA",10,"esto es basura3");
	insert_1("tableA",10,"esto es basura4");
	insert_1("tableA",10,"esto es basura5");
	insert_1("tableA",10,"esto es basura6");
	insert_1("tableA",10,"esto es basura7");
	lfs_log_info("Mostrando  memtable \n");
	memtable_mostrar();
	lfs_log_info("dump \n");
	dumpear();
	memtable_mostrar();
	select1("tableA",10);

	log_destroy(lfs_logger);
	list_destroy(memtable);

	puts("... FIN LFS ...");
	return EXIT_SUCCESS;
}

//int main() {
//
//	config_cargar("LFS.config");
//	imprimir_configuracion();
//
//		puts("mostrar particiones de una tabla tableA");
//			t_list* particiones = obtenerParticiones("tableA");
//		list_iterate(particiones,mostrarParticion);
//		list_destroy(particiones);



//		puts("describe2---------");
//		describe2("tableA");

//		puts("describe1------");
//		describe1();

//		puts("describe1 simulacion ------");
//		describe2("unaTablaEtc");
//		describe2("tableA3");
//		describe2("tableA2");
//		describe2("tableA");

//		puts("->>>mostrar listado de tablas");
//		t_list* archivos = obtenerListadoDeNombresDeSubArchivos("src/punto_de_montaje_FS_LISSANDRA_ejemplo/Tables");
//		list_iterate(archivos,puts);
//		list_destroy(archivos);

//		puts("mostrar particiones de una tabla path");
//		t_list* particiones_path = obtenerListaDeParticiones_path("tableA");
//		list_iterate(particiones_path,puts);
//		list_destroy(particiones_path);
//
//		puts("mostrar particiones de una tabla struct ");
//		t_list* particiones_=obtenerParticiones("tableA");
//		list_iterate(particiones_,mostrarParticion);
//		list_destroy(particiones_);

//		puts("drop en tableA3");
//		drop("tableA3");
//		puts("inicio de recorrido de bloque 4 ");
//		recorrerBloque("src/punto_de_montaje_FS_LISSANDRA_ejemplo/Bloques/4.bin");

//		puts("FIN");
//
//	return EXIT_SUCCESS;
//}
void recorrerBloque(const char* pathBloque){//ok
	FILE* bloque =fopen(pathBloque,"r+");
	if(bloque==NULL)perror("error al abrir el bloque para recorrer ");
	while(!feof(bloque)){
		RegistroLinea registro;
		registro.value=malloc(lfs.tamanioValue);
//		fscanf(bloque,"%d;%d;%s\n",&registro.timestamp,&registro.key,registro.value);
		registro = obtenerRegistroLinea(bloque);
		registroLinea_mostrar(&registro);
		free(registro.value);
	}
	fclose(bloque);
}
RegistroLinea obtenerRegistroLinea(FILE* bloque){
	RegistroLinea registro;//=(RegistroLinea*)malloc(sizeof(RegistroLinea));
	registro.value=malloc(lfs.tamanioValue);
//	fseek(bloque,posicionLinea,SEEK_SET);
	fscanf(bloque,"%lu;%d;%s\n",&registro.timestamp,&registro.key,registro.value);
//	printf("bytes leidos = %d \n",n );
	return registro;
}
//void printRegistroLinea(RegistroLinea* registro){
//	printf("Registro linea -> %d;%d;%s\n",registro->timestamp,registro->key,registro->value);
//}

t_list* obtenerListadoDeSubArchivos(const char * pathDirectorio,const char* extension){//ok
	t_list* nombresDeArchivos=list_create();
	struct stat estadoDeArchivo;
		struct dirent* archivo;
		DIR* directorio;
		for(directorio=opendir(pathDirectorio);(archivo=readdir(directorio))!=NULL;stat(archivo->d_name,&estadoDeArchivo)){
			if(string_ends_with(archivo->d_name,extension)){
				list_add(nombresDeArchivos,strdup(archivo->d_name));
			}
		}
		return nombresDeArchivos;
}
t_list* obtenerListadoDeSubArchivosCompleto(const char * pathDirectorio,const char* extension){//ok
	t_list* nombresDeArchivos=list_create();
	struct stat estadoDeArchivo;
		struct dirent* archivo;
		DIR* directorio;
		for(directorio=opendir(pathDirectorio);(archivo=readdir(directorio))!=NULL;stat(archivo->d_name,&estadoDeArchivo)){
			if(directorio==NULL)perror("error en obtener listado de sub archivos ");
			if(string_ends_with(archivo->d_name,extension)){
				char* aux = (char*)malloc(strlen(pathDirectorio)+strlen(archivo->d_name)+2);
				sprintf(aux,"%s%s%s",pathDirectorio,"/",archivo->d_name);
				list_add(nombresDeArchivos,aux);
//				free(aux);
			}
		}
//		bool ordenarPorNombre(const char* unPath,const char* otroPath){
//			bool ordenado = false;
//			for(int i =0;unPath[i]!='\0' || otroPath[i]!='\0';i++){
//				if(unPath[i]<otroPath[i]){
//					ordenado=true;
//					break;
//				}
//			}
//		}
//		list_sort(nombresDeArchivos,ordenarPorNombre);
		return nombresDeArchivos;
}
t_list* obtenerListadoDeNombresDeSubArchivos(const char* pathCarpetaPadre){//ok
	t_list* lista = list_create();
	struct stat estadoDeArchivo;
		struct dirent* archivo;
		DIR* directorio;
		for(directorio=opendir(pathCarpetaPadre);(archivo=readdir(directorio))!=NULL;stat(archivo->d_name,&estadoDeArchivo)){
			if(directorio==NULL)perror("error al obtener listado de subarchivos ");
			if(!string_contains(archivo->d_name,".")){
//				char* aux = (char*)malloc(strlen(pathCarpetaPadre)+strlen(archivo->d_name)+2);
//				sprintf(aux,"%s%s%s",pathCarpetaPadre,"/",archivo->d_name);
				list_add(lista,strdup(archivo->d_name));
//				free(aux);
			}
		}
		return lista;
}
void compactar(const char* nombreDeTabla){
	t_list* particionesTemporales=obtenerParticionesTemporales(nombreDeTabla);
	t_list* particionesNoTemporales=obtenerParticionesNoTemporales(nombreDeTabla);

	list_destroy(particionesTemporales);
	list_destroy(particionesNoTemporales);
}
t_list* obtenerParticiones(const char* nombreDeTabla){//ok
	t_list* listaDeParticiones_path = obtenerListaDeParticiones_path(nombreDeTabla);
	Particion* pathToParticion(char* pathDeParticion){
		Particion* unaParticion=(Particion*)malloc(sizeof(Particion));
		t_config* config = config_create(pathDeParticion);
		unaParticion->esTemporal=false;
		if(string_ends_with(pathDeParticion,".tmp") || string_ends_with(pathDeParticion,".tmpc"))unaParticion->esTemporal=true;
		unaParticion->bloques=config_get_array_value(config,"block");
		unaParticion->size=config_get_int_value(config,"size");
		strcpy(unaParticion->pathParticion,pathDeParticion);
//		memcpy(unaParticion->pathParticion,pathDeParticion,strlen(pathDeParticion)*sizeof(char));
		config_destroy(config);
		return unaParticion;
	}
	t_list* listaDeParticiones = list_map(listaDeParticiones_path,pathToParticion);
	list_destroy(listaDeParticiones_path);
	return listaDeParticiones;
}

t_list* obtenerListaDeParticiones_path(const char* nombreDeTabla ){//ok
	t_list* listaDePaths=list_create();
	char* aux = obtenerPathDeTabla(nombreDeTabla);
	t_list* listaDe_bin = obtenerListadoDeSubArchivosCompleto(aux,".bin");
	t_list* listaDe_partition=obtenerListadoDeSubArchivosCompleto(aux,".partition");
	t_list* listaDe_tmp = obtenerListadoDeSubArchivosCompleto(aux,".tmp");
	t_list* listaDe_tmpc= obtenerListadoDeSubArchivosCompleto(aux,".tmpc");
	free(aux);
	list_add_all(listaDePaths,listaDe_bin);
	list_add_all(listaDePaths,listaDe_tmp);
	list_add_all(listaDePaths,listaDe_tmpc);
	list_add_all(listaDePaths,listaDe_partition);
	list_destroy(listaDe_bin);
	list_destroy(listaDe_tmp);
	list_destroy(listaDe_tmpc);
	list_destroy(listaDe_partition);
	return listaDePaths;
}
t_list* obtenerParticionesTemporales(const char* nombreDeTabla){
	t_list* particionesTemporales=list_create();
	Metadata_Tabla *metadataDeTabla = obtenerMetadata(nombreDeTabla);
	int x = metadataDeTabla->PARTITIONS;
	char* pathDeLaTabla=obtenerPathDeTabla(nombreDeTabla);
	free(pathDeLaTabla);
	return particionesTemporales;
}
t_list* obtenerParticionesNoTemporales(const char* nombreDeTabla){
	t_list* particionesTemporales=list_create();
	return particionesTemporales;
}

//---------
void lfs_consola(){
	while(1){
		char* linea = readline("LFS@_consola -> ");
		struct_operacion* parametros_lql_leidos = parsear_linea(linea);
		ejecutar_linea_lql(parametros_lql_leidos);
		printf(" se leyo la  la sentencia \"%s\" LQL\n", linea);

//		if(string_contains(linea,"DUMP"))dumpear();//dumpeo, dumpaear debe de ser ejecutado cada cierto tiempo por un hilo
//		if(string_contains(linea,"MEMTABLE"))memtable_mostrar();
//		if(string_contains(linea,"FIN"))break;
		free(linea);
	}
}
void ejecutar_linea_lql(struct_operacion* parametros_de_linea_lql){
	size_t cantidadDeParametros;
	for(cantidadDeParametros=0;parametros_de_linea_lql->parametros[cantidadDeParametros]!=NULL;cantidadDeParametros++);
	printf("ejecutar_linea_lql(), cantidad de parametros = %d\n",cantidadDeParametros);
	switch (parametros_de_linea_lql->nombre_operacion) {
		case API_CREATE:
			lfs_create((parametros_de_linea_lql->parametros)[0],
					(parametros_de_linea_lql->parametros)[1],
					(parametros_de_linea_lql->parametros)[2],
					(parametros_de_linea_lql->parametros)[3]);
			break;
		case API_INSERT:
			if(cantidadDeParametros==3)lfs_insert1((parametros_de_linea_lql->parametros)[0],(parametros_de_linea_lql->parametros)[1],(parametros_de_linea_lql->parametros)[2]);
			else lfs_insert2((parametros_de_linea_lql->parametros)[0],(parametros_de_linea_lql->parametros)[1],(parametros_de_linea_lql->parametros)[2],(parametros_de_linea_lql->parametros)[3]);
			break;
		case API_SELECT:
			lfs_select1(parametros_de_linea_lql->parametros[0],
					parametros_de_linea_lql->parametros[1],
					parametros_de_linea_lql->parametros[2]);
			break;
		case API_DESCRIBE:
			if(cantidadDeParametros==0)lfs_describe();
			else lfs_describe2((parametros_de_linea_lql->parametros)[0]);
			break;
		case API_DROP:
			lfs_drop((parametros_de_linea_lql->parametros)[0]);
			break;
		default:
			lfs_log_error("linea LQL leida erronea \n");
			break;
	}
}

