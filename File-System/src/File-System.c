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
	puts("... INICIA LFS ...");

	//inicio log
	logger = log_create("LFS.log", "LISSANDRA - LFS", true, LOG_LEVEL_TRACE);

	config_cargar("LFS.config");
	log_info(logger,"se cargo LFS.log \n");

	memtable=list_create();

	imprimir_configuracion();

	pthread_create(&consola,NULL,lfs_consola,NULL);
	pthread_join(consola,NULL);
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
//void struct_operacion_destroy(struct_operacion* unaLineaLql){
//	free_char_x2(unaLineaLql->parametros);
//}
void recibir_conexion(){

	crearSocket(&FileSystem_fd);
	setearParaEscuchar(&FileSystem_fd, lfs.puertoEscucha);
	LFS_FD=aceptarConexion(FileSystem_fd);
}
//int main(void) {
//	void iterator(char* value)
//		{
//			printf("%s\n", value);
//		}
//
//		logger = log_create("LFS.log", "Servidor", 1, LOG_LEVEL_DEBUG);
//
//		int server_fd = iniciar_servidor();
//		log_info(logger, "Servidor listo para recibir al cliente");
//		int cliente_fd = esperar_cliente(server_fd);
//
//		t_list* lista;
////		while(1)
////		{
////			int cod_op = recibir_operacion(cliente_fd);
////			switch(cod_op)
////			{
////			case MENSAJE:
////				recibir_mensaje(cliente_fd);
////				break;
////			case PAQUETE:
////				lista = recibir_paquete(cliente_fd);
////				printf("Me llegaron los siguientes valores:\n");
////				list_iterate(lista, (void*) iterator);
////				break;
////			case -1:
////				log_error(logger, "el cliente se desconecto. Terminando servidor");
////				return EXIT_FAILURE;
////			default:
////				log_warning(logger, "Operacion desconocida. No quieras meter la pata");
////				break;
////			}
////		}
//		char* s;
//		while(1){
//			 s = recibir_mensaje_v2(cliente_fd);
//			puts(s);
//		}
//		free(s);
//		return EXIT_SUCCESS;
//}

