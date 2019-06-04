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

#include <commons/string.h>
int main(void) {
	puts("... INICIA LFS ...");

	//inicio log
	logger = log_create("LFS.log", "LISSANDRA - LFS", true, LOG_LEVEL_TRACE);

	config_cargar("LFS.config");
	log_info(logger,"se cargo LFS.log \n");



	imprimir_configuracion();

	create("unaTablalalalEtc2","SC",16,7000);//ok

//	recibir_conexion();//recibe conexion de memoria
	//insert_1("EEEE",1,"Holassss");//falta


	log_destroy(logger);


	puts("... FIN LFS ...");
	return EXIT_SUCCESS;
}
//int main() {//ok
//	puts("comenzando");
////	crearTabla("src/punto_de_montaje_FS_LISSANDRA_ejemplo/Tables/TablaDeEjemplo");
////	crearMetadata_v2("src/punto_de_montaje_FS_LISSANDRA_ejemplo/Tables/TablaDeEjemplo/metadata.met","C",4,1000);
////	crearMetadata_v2("unaMetadata2.met","C",4,1000);
//	crearTabla("tablaEjemplo");
//	mostrarMetadata("src/punto_de_montaje_FS_LISSANDRA_ejemplo/Tables/TablaDeEjemplo/metadata.met");
//	puts("fin");
//	return EXIT_SUCCESS;
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

