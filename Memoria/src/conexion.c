/*
 * conexion.c
 *
 *  Created on: 1 ago. 2019
 *      Author: utnso
 */
#include "conexion.h"
//int main(void) {
//	int socket_cliente = crear_conexion("127.0.0.1","4445");
//	for (;;) {
//
//		char* mensaje=readline("-> ");
//		enviar_lql(mensaje,socket_cliente);
//		free(mensaje);
//	}
//	return EXIT_SUCCESS;
//}

int crear_conexion(char *ip, char* puerto){
	struct addrinfo hints;
	struct addrinfo *server_info;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(ip, puerto, &hints, &server_info);
	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1){
		perror("error en crear conexion");
		return -1;
	}
//		printf("error");
	freeaddrinfo(server_info);
	if(socket_cliente==-1)return -1;
	return socket_cliente;
}
void enviar_lql(char*mensaje, int fd_socket){
	int size_del_mensaje = strlen(mensaje);
	send(fd_socket,&size_del_mensaje,sizeof(int),0);
	send(fd_socket,mensaje,strlen(mensaje),0);
}

