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
#include "config/config.h"
#include <pthread.h>
#include <dirent.h>
pthread_t consola,dump;//,compactador;

//typedef struct{
//	char pathParticion[256];
//	bool esTemporal;
//}ParticionNombre;

//int main(void) {
//	puts("... INICIA LFS ...");
//
//	//inicio log
//	logger = log_create("LFS.log", "LISSANDRA - LFS", true, LOG_LEVEL_TRACE);
//
//	config_cargar("LFS.config");
//	log_info(logger,"se cargo LFS.log \n");
//
//	memtable=list_create();
//
//	imprimir_configuracion();
//
//	pthread_create(&consola,NULL,lfs_consola,NULL);
//	pthread_join(consola,NULL);
////	lfs_consola();//por el momento funciona con el CREATE
//
////	system("rmdir src/punto_de_montaje_FS_LISSANDRA_ejemplo/Tables/tableA");
//
//
//
//
//	log_destroy(logger);
//	list_destroy(memtable);
//	puts("... FIN LFS ...");
//	return EXIT_SUCCESS;
//}
int main() {

	config_cargar("LFS.config");
	imprimir_configuracion();
	struct stat estadoDeArchivo;
		struct dirent* archivo;
		puts("antes de tabla A");
		char* pathDeTabla=obtenerPathDeTabla("unaTablaEtc");
		puts("tablaA");
		DIR* directorio;
		for(directorio=opendir(pathDeTabla);(archivo=readdir(directorio))!=NULL;stat(archivo->d_name,&estadoDeArchivo)){
			puts(archivo->d_name);
			if((strcmp(archivo->d_name,".")!=0)&&(strcmp(archivo->d_name,"..")!=0)){
//			Particion* unaParticion=(Particion*)malloc(sizeof(Particion));
//			if(string_ends_with(archivo->d_name,".tmp")){//es temporal
//							unaParticion->esTemporal=true;
//
//			}
		}
		}
		puts("ahora con la funcion hecha");
		puts(pathDeTabla);
		t_list* lista = obtenerListadoDeSubArchivosCompleto(pathDeTabla,".partition");
		list_iterate(lista,puts);
		list_destroy(lista);

		free(pathDeTabla);


		puts("mostrar particiones de una tabla tableA");
			t_list* particiones = obtenerParticiones("tableA");
		list_iterate(particiones,mostrarParticion);
		list_destroy(particiones);


		puts("inicio de recorrido de bloque");
		recorrerBloque("src/punto_de_montaje_FS_LISSANDRA_ejemplo/Bloques/2.bin");

		puts("describe2---------");
		describe2("tableA");
//		puts("describe1------");
//		describe1();

//		puts("describe1 simulacion ------");
//		describe2("unaTablaEtc");
//		describe2("tableA3");
//		describe2("tableA2");
//		describe2("tableA");

		puts("mostrar listado de archivos de punto de montaje");
		t_list* archivos = obtenerListadoDeNombresDeSubArchivos("src/punto_de_montaje_FS_LISSANDRA_ejemplo/Tables");
		list_iterate(archivos,puts);
		list_destroy(archivos);



		puts("mostrar particiones de una tabla path");
		t_list* particiones_path = obtenerListaDeParticiones_path("tableA");
		list_iterate(particiones_path,puts);
		list_destroy(particiones_path);


		puts("drop en tableA");
		drop("tableA");

		puts("FIN");

	return EXIT_SUCCESS;
}
void recorrerBloque(const char* pathBloque){//ok
	FILE* bloque =fopen(pathBloque,"r+");
	if(bloque==NULL)perror("error al abrir el bloque para recorrer ");
	while(!feof(bloque)){
		RegistroLinea registro;
		registro.value=malloc(lfs.tamanioValue);
//		fscanf(bloque,"%d;%d;%s\n",&registro.timestamp,&registro.key,registro.value);
		registro = obtenerRegistroLinea(bloque);
		printRegistroLinea(&registro);
		free(registro.value);
	}
	fclose(bloque);
}
RegistroLinea obtenerRegistroLinea(FILE* bloque){
	RegistroLinea registro;//=(RegistroLinea*)malloc(sizeof(RegistroLinea));
	registro.value=malloc(lfs.tamanioValue);
//	fseek(bloque,posicionLinea,SEEK_SET);
	fscanf(bloque,"%d;%d;%s\n",&registro.timestamp,&registro.key,registro.value);
//	printf("bytes leidos = %d \n",n );
	return registro;
}
void printRegistroLinea(RegistroLinea* registro){
	printf("Registro linea -> %d;%d;%s\n",registro->timestamp,registro->key,registro->value);
}
void registroLineaDestroy(RegistroLinea* linea){
	free(linea->value);
	free(linea);
}
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
//		strcpy(unaParticion->pathParticion,pathDeParticion);
		memcpy(unaParticion->pathParticion,pathDeParticion,strlen(pathDeParticion)*sizeof(char));
		config_destroy(config);
		return unaParticion;
	}
	t_list* listaDeParticiones = list_map(listaDeParticiones_path,pathToParticion);
	list_destroy(listaDeParticiones_path);
	return listaDeParticiones;
}
void mostrarParticion(Particion* particion){//ok
	if(particion->esTemporal)printf("particion = %s \n	size= %d \n	y es temporal y un bloque es %s\n",particion->pathParticion,particion->size,particion->bloques[0]);
	else printf("particion = %s \n	size= %d \n	y no es temporal y un bloque es %s\n",particion->pathParticion,particion->size,particion->bloques[0]);
}
t_list* obtenerListaDeParticiones_path(const char* nombreDeTabla ){//ok
	t_list* listaDePaths=list_create();
	char* aux = obtenerPathDeTabla(nombreDeTabla);
	t_list* listaDeBin = obtenerListadoDeSubArchivosCompleto(aux,".bin");
	t_list* listaDe_partition=obtenerListadoDeSubArchivosCompleto(aux,".partition");
	t_list* listaDe_tmp = obtenerListadoDeSubArchivosCompleto(aux,".tmp");
	t_list* listaDe_tmpc= obtenerListadoDeSubArchivosCompleto(aux,".tmpc");
	free(aux);
	list_add_all(listaDePaths,listaDeBin);
	list_add_all(listaDePaths,listaDe_tmp);
	list_add_all(listaDePaths,listaDe_tmpc);
	list_add_all(listaDePaths,listaDe_partition);
	list_destroy(listaDeBin);
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
//Persona* buscarPersona(t_list* lista, char* nombre){//para probar
//	bool buscarLaDePersona1(Persona* p){
//			return strcmp(p->nombre,"Jhon")==0;
//		}
//	Persona* encontrada=list_find(lista,buscarLaDePersona1);
//	return encontrada;
//}

//---------
//t_list* obtenerParticiones(const char* nombreDeTabla){
//	t_list* particiones = list_create();
//	struct stat estadoDeArchivo;
//	struct dirent* archivo;
//	char* pathDeTabla=obtenerPathDeTabla(nombreDeTabla);
//
//	DIR* directorio;
//	for(directorio=opendir(pathDeTabla);(archivo=readdir(directorio))!=NULL;){
//		puts("x");
//		stat(archivo->d_name,&estadoDeArchivo);
//		puts(archivo->d_name);
////		if((strcmp(archivo->d_name,".")!=0)&&(strcmp(archivo->d_name,"..")!=0)){
//
//		if(string_ends_with(archivo->d_name,".tmp")){//es temporal
//
//			Particion* unaParticion=(Particion*)malloc(sizeof(Particion));
//			unaParticion->esTemporal=true;
//
//			list_add(particiones,unaParticion);
//
//		}
//	}
//
//	free(pathDeTabla);
//	return particiones;
//}
void lfs_consola(){
	while(1){
		char* linea = readline("LFS@_consola -> ");

		struct_operacion* parametros_lql_leidos = parsear_linea(linea);
		ejecutar_linea_lql(parametros_lql_leidos);
//
		printf(" se leyo la  la sentencia \"%s\" LQL\n", linea);
//		printf("nombre de tabla = %s\n", (parametros_lql_leidos->parametros)[0]);
//		printf("nombre tipo de consistencia = %s\n", (parametros_lql_leidos->parametros)[1]);
//		printf("nombre de particiones  = %s \n", (parametros_lql_leidos->parametros)[2]);
//		printf("tiempo de compactacion = %s \n ", (parametros_lql_leidos->parametros)[3]);
		free(linea);
	}
}
void ejecutar_linea_lql(struct_operacion* parametros_de_linea_lql){
	switch (parametros_de_linea_lql->nombre_operacion) {
		case API_CREATE:
			lfs_create((parametros_de_linea_lql->parametros)[0],(parametros_de_linea_lql->parametros)[1],(parametros_de_linea_lql->parametros)[2],(parametros_de_linea_lql->parametros)[3]);
			break;
		case API_INSERT:
			//insert();
			break;
		case API_SELECT:
//			select1();
//			lfs_select()
			break;
		case API_DESCRIBE:
//			describe1();
//			describe2();
			break;
		case API_DROP:
//			drop();
			break;
		default:
			log_error(logger,"linea LQL leida erronea \n");
			break;
	}
}

