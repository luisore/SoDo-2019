/*
 * conexion.h
 *
 *  Created on: 1 ago. 2019
 *      Author: utnso
 */

#ifndef CONEXION_H_
#define CONEXION_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <readline/readline.h>
#include <readline/history.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
int crear_conexion(char *ip, char* puerto);
void enviar_lql(char*mensaje, int fd_socket);


#endif /* CONEXION_H_ */
