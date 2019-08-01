/*
 * conexion.h
 *
 *  Created on: 1 ago. 2019
 *      Author: utnso
 */

#ifndef CONEXION_H_
#define CONEXION_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
int iniciar_servidor(const char* _SERVER_IP,const char* _SERVER_PUERTO);
int  conectar_cliente(int fd_server);
char* recibir_lql(int fd_cliente);

#endif /* CONEXION_H_ */
