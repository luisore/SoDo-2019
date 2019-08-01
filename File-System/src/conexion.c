/*
 * conexion.c
 *
 *  Created on: 1 ago. 2019
 *      Author: utnso
 */


#include "conexion.h"
//int main(void) {
//	int server = iniciar_servidor("127.0.0.1","4445");
//	int cliente = conectar_cliente(server);
//
//	while(1){
//		char* buffer = recibir_lql(cliente);
//		puts(buffer);
//		free(buffer);
//	}
//	return EXIT_SUCCESS;
//
//}
int iniciar_servidor(const char* _SERVER_IP,const char* _SERVER_PUERTO){// parece ok
	int socket_servidor;
    struct addrinfo hints, *servinfo, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(_SERVER_IP, _SERVER_PUERTO, &hints, &servinfo);
    for (p=servinfo; p != NULL; p = p->ai_next)
    {
        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;
        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
        	perror("Fallo el bind");
        	close(socket_servidor);
            continue;
        }
        break;
    }
	listen(socket_servidor, SOMAXCONN);
    freeaddrinfo(servinfo);
    puts("inicia servidor");
    puts("Estoy escuchando");

    return socket_servidor;
}
int  conectar_cliente(int fd_server){
	struct sockaddr_in direcionCliente;
		    	unsigned int tamanioDireccion;
		    	int cliente = accept(fd_server,(void*)(&direcionCliente),&tamanioDireccion);
		    	printf("Recibi una conexion, fd de cliente es  %d !!\n",cliente);
		    	char mensaje[] = "Hola Cliente",otroMensaje[]="como va";
		    	send(cliente,mensaje,sizeof(mensaje),0);
		    	send(cliente,otroMensaje,sizeof(otroMensaje),0);
	return cliente;
}
char* recibir_lql(int fd_cliente){
	char* buffer =NULL;
	int size_mensaje=-1;
			int bytesRecibidos=recv(fd_cliente,&size_mensaje,sizeof(int),0);
			if(bytesRecibidos<0 || size_mensaje==-1){
				perror("error al recibir size del mensaje o se desconecto");
//				return EXIT_FAILURE;
			}
			buffer=malloc(size_mensaje);
			bytesRecibidos=recv(fd_cliente,buffer,size_mensaje,0);
			if (bytesRecibidos<0)perror("Nada recibido");
			if(bytesRecibidos==0)perror("mensaje recibido vacio");
	//		buffer[bytesRecibidos]='\0';
			printf("Llegaron %d bytes como  : %s y longitud %d \n",bytesRecibidos,buffer,size_mensaje);
	return buffer;
}
