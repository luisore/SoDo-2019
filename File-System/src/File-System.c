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
#include "log/log.h"
int main(void) {
	probando();
	config_cargar("LFS.config");
//	log_inicio();
	imprimir_configuracion();

	recibir_conexion();
//	log_fin();


	puts("algo");
	return EXIT_SUCCESS;
}


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

