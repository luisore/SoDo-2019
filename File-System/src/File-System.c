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

pthread_t consola,dump;//,compactador;

int main(void) {
	pthread_mutex_init (&mMemtable, NULL);
	puts("... INICIA LFS ...");

	//inicio log
	logger = log_create("LFS.log", "LISSANDRA - LFS", true, LOG_LEVEL_TRACE);

	config_cargar("LFS.config");
	log_info(logger,"se cargo LFS.log \n");

	memtable=list_create();

	crear_estructuras();
	leer_tablas();
	imprimir_configuracion();
	//creacionDeArchivoBitmap(path_bitmap(),lfsmetadata.cantidadDeBloques);
	//crearBitmap();

	pthread_create(&consola,NULL,lfs_consola,NULL);
	pthread_create(&dump,NULL,dump_proceso,NULL);
	pthread_join(consola,NULL);
	pthread_join(dump,NULL);
//	lfs_consola();//por el momento funciona con el CREATE

//	system("rmdir src/punto_de_montaje_FS_LISSANDRA_ejemplo/Tables/tableA");




	log_destroy(logger);
	list_destroy(memtable);
	puts("... FIN LFS ...");
	return EXIT_SUCCESS;
}
//Persona* buscarPersona(t_list* lista, char* nombre){//para probar
//	bool buscarLaDePersona1(Persona* p){
//			return strcmp(p->nombre,"Jhon")==0;
//		}
//	Persona* encontrada=list_find(lista,buscarLaDePersona1);
//	return encontrada;
//}

void *dump_proceso(){
	while(1){
		usleep(lfs.tiempoDump*1000);
		/*
		log_info(logger,"iniciando proceso dump");
		pthread_mutex_lock (&mMemtable);
		for(int i=0;i<list_size(memtable);i++){
			printf("cantidad en la memtable %d: " ,list_size(memtable));
			Insert *insert =list_get(memtable,i);
				printf("tabla %s: \n" ,insert->nombreDeLaTabla);
				printf("compactacion largo %d: \n" ,insert->cantParticionesCompactacion);
				printf("particiones %d: \n" ,insert->cantParticionesTemporales);
				printf("registros %d: \n" ,sizeof(insert->registros));
			for(int k=0;k<list_size(insert->registros);k++){
				RegistroLinea *registro =list_get(insert->registros,k);
				//printf("key %d: " ,registro->key);
				//printf("value largo %d: " ,strlen(registro->value));
				//guardar_en_temporal(insert,registro);
				insertarRegistrosEnParticionTemporal(insert->nombreDeLaTabla,registro, insert->cantParticionesTemporales);
				insert->cantParticionesTemporales++;
			}
		}
		pthread_mutex_unlock (&mMemtable);
		*/
	}
	return NULL;
}

void *lfs_consola(){
	while(1){
		char* linea = readline("LFS@_consola -> ");

		struct_operacion* parametros_lql_leidos = parsear_linea(linea);
		ejecutar_linea_lql(parametros_lql_leidos);
//

		printf(" se leyo la  la sentencia \"%s\" LQL\n", linea);
		//printf("nombre de tabla = %s\n", (parametros_lql_leidos->parametros)[0]);
		//printf("nombre tipo de consistencia = %s\n", (parametros_lql_leidos->parametros)[1]);
		//printf("nombre de particiones  = %s \n", (parametros_lql_leidos->parametros)[2]);
		//printf("tiempo de compactacion = %s \n ", (parametros_lql_leidos->parametros)[3]);
		free(linea);
	}
	return NULL;
}
void ejecutar_linea_lql(struct_operacion* parametros_de_linea_lql){
	switch (parametros_de_linea_lql->nombre_operacion) {
		case API_CREATE:
			lfs_create((parametros_de_linea_lql->parametros)[0],(parametros_de_linea_lql->parametros)[1],(parametros_de_linea_lql->parametros)[2],(parametros_de_linea_lql->parametros)[3]);
			break;
		case API_INSERT:{
			struct_insert *insert;
			insert=estructura_registro(parametros_de_linea_lql,INSERT);
			insert_2(insert->nombreTabla,insert->key,insert->valor,insert->timestats);
			//insert();
			break;
			}
		case API_SELECT:
//			select1();
//			lfs_select()
			break;
		case API_DESCRIBE:
//			describe1();
//			describe2();
			break;
		case API_DROP:{
			log_info(logger, "borrarando tabla :%s",(parametros_de_linea_lql->parametros)[0]);
			drop((parametros_de_linea_lql->parametros)[0]);
			break;
		}

		default:
			log_error(logger,"linea LQL leida erronea \n");
			break;
	}
}
//void struct_operacion_destroy(struct_operacion* unaLineaLql){
//	free_char_x2(unaLineaLql->parametros);
//}
void recibir_conexion(){

	crearSocket(&FileSystem_fd);
	setearParaEscuchar(&FileSystem_fd, lfs.puertoEscucha);
	LFS_FD=aceptarConexion(FileSystem_fd);
}
void crear_estructuras(){
	cargar_metadata();
	//Creacion de punto de motnaje
	printf("montaje: %s \n",lfs.puntoDeMontaje);
	if(mkdir(lfs.puntoDeMontaje, S_IRWXU) == -1){
		log_error(logger, "punto de montaje ya creado" );
	}

	//Creacion de directorio Tablas
	printf("montaje: %s \n",lfs.puntoDeMontaje);
	char *tables =  malloc (strlen(lfs.puntoDeMontaje)+ strlen("Tables/"));
	strcpy(tables,lfs.puntoDeMontaje);
	strcat(tables,"Tables/");

	if(mkdir(tables, S_IRWXU) == -1){
			log_error(logger, "directorio tablas ya creado" );

	}
	else{
		//free(tables);
	}


	//Creacion de directorio bloques
	char *bloques =  malloc (strlen(lfs.puntoDeMontaje)+ strlen("Bloques/"));
	strcpy(bloques,lfs.puntoDeMontaje);
	strcat(bloques,"Bloques/");

	if(mkdir(bloques, S_IRWXU) == -1){
		log_error(logger, "path bloques ya creado" );
	}
	creacionDeBloques();

/*
	//Creacion de directorio Bitmap
	if(mkdir(bloques, S_IRWXU) == -1){
			log_error(logger, "path bloques ya creado" );
	}
	else{
		creacionDeBloques();
		//creacionArchivoBitmap();
	}
*/
}


void cargar_metadata(){

	char *direccionArchivoMedata = path_tables();
	string_append(&direccionArchivoMedata,"/Metadata/Metadata.bin");
	printf("direccionArchivoMedata %s \n",direccionArchivoMedata);
	if (validarArchivoConfig(direccionArchivoMedata)<0){
		//log_error(log_lfs,"No se encontro en el file system el archivo metadata");
	}
	leerMetaData();
}

int leerMetaData(){
	char *direccionArchivoMedata=(char *) malloc(1 + strlen(lfs.puntoDeMontaje) +  strlen("Tables") +strlen("/Metadata/Metadata.bin"));;
	//direccionArchivoMedata = config_MDJ.mount_point;
	strcpy(direccionArchivoMedata,lfs.puntoDeMontaje);
	string_append(&direccionArchivoMedata,"Tables");
	string_append(&direccionArchivoMedata,"/Metadata/Metadata.bin");
	t_config *archivo_MetaData;
	archivo_MetaData=config_create(direccionArchivoMedata);
	lfsmetadata.cantidadDeBloques=config_get_int_value(archivo_MetaData,"BLOCKS");
    lfsmetadata.magicNumber=string_duplicate(config_get_string_value(archivo_MetaData,"MAGIC_NUMBER"));
	lfsmetadata.tamanioBloque=config_get_int_value(archivo_MetaData,"BLOCK_SIZE");
	free(direccionArchivoMedata);
	config_destroy(archivo_MetaData);
	return 0;
}


void creacionDeBloques(){

	log_error(logger, "creacion bloques" );
	printf("bloque path: %s:",lfs.puntoDeMontaje);
	char* path_bloque=malloc(strlen(lfs.puntoDeMontaje)+strlen("Bloques/"));
	strcpy(path_bloque,lfs.puntoDeMontaje);
	strcat(path_bloque,"Bloques/");

	for (int numeroDeBloque = 0; numeroDeBloque < lfsmetadata.cantidadDeBloques; numeroDeBloque++) {
		//sprintf(path_bloque,"/%d.bin",numeroDeBloque);
		char *bloque=string_itoa(numeroDeBloque);
		char *numberBloque = malloc (strlen(path_bloque)+strlen(bloque)+strlen(".bin"));
		//strcpy(numberBloque,"\"");
		strcpy(numberBloque,path_bloque);
		strcat(numberBloque,bloque);
		strcat(numberBloque,".bin");
		//strcat(numberBloque,"\"");
		//printf("path %s:\n",numberBloque);
		FILE* particion = fopen(numberBloque,"w");
		fclose(particion);
	}
	free(path_bloque);
}

char *path_bitmap(){
	char *direccionArchivoBitMap=(char *) malloc(1 + strlen(lfs.puntoDeMontaje) + strlen("Tables/Metadata/Bitmap.bin"));
	strcpy(direccionArchivoBitMap,lfs.puntoDeMontaje);
	string_append(&direccionArchivoBitMap,"Tables/Metadata/Bitmap.bin");
	//puts(direccionArchivoBitMap);
	return direccionArchivoBitMap;

}

void leer_tablas(){
    DIR *dip;
    struct dirent   *dit;
    char *path_tablas= malloc(strlen(lfs.puntoDeMontaje)+strlen("Tables/"));
    strcpy(path_tablas,lfs.puntoDeMontaje);
    strcat(path_tablas,"Tables/");
    if ((dip = opendir(path_tablas)) == NULL)
    {
              perror("opendir");

    }

    printf("Verificando si hay tablas ya cargadas\n");

    while((dit = readdir(dip)) != NULL){
        if((strcmp(dit->d_name, ".") != 0) && (strcmp(dit->d_name, "..") != 0)){

        	if (dit->d_type &  DT_DIR )
        	{
        		if(strcmp(dit->d_name,"Metadata")){
        			  printf("es un directorio %s \n",dit->d_name);
        			  log_info(logger, "directorio ya creado y agregado a la memtabla: %s",dit->d_name );
        			  Insert *agregar_tabla = malloc(sizeof(Insert));
        			  agregar_tabla->cantParticionesCompactacion = 0;
        			  agregar_tabla->cantParticionesTemporales = 0;
        			  agregar_tabla->nombreDeLaTabla = strdup(dit->d_name);
        			  agregar_tabla->registros = list_create();
        			  list_add(memtable,agregar_tabla);
        			  log_info(logger, "Agregado a la memtabla: %s",dit->d_name );

        			  struct_create *registro_compactacion = malloc(sizeof(struct_create));
        			  Metadata_Tabla *unMetadata=obtenerMetadata(dit->d_name);
        			  registro_compactacion->nombreTabla = strdup(dit->d_name);
        			  registro_compactacion->numeroParticiones= unMetadata->PARTITIONS;
					  registro_compactacion->tiempoCompactacion =unMetadata->COMPACTION_TIME;

					  crearHiloCompactacion(dit->d_name,unMetadata->PARTITIONS,unMetadata->COMPACTION_TIME);
        			  //compactacion(registro_compactacion);
        			  //free(unMetadata);

        		}

        	}
        	//printf("%s\n", dit->d_name);
        }

    }
            if (closedir(dip) == -1)
            {
                    perror("cerrado directorio");

            }

            printf("\ndirectorio esta ahora cerrado\n");


}
