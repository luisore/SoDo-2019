/*
 * FuncionesAuxiliares.c
 *
 *  Created on: 22 abr. 2019
 *      Author: utnso
 */

#ifndef FUNCIONESAUXILIARES_C_
#define FUNCIONESAUXILIARES_C_
#include "FuncionesAuxiliares.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

unsigned long funcionEpoc(){
	return (unsigned long)time(NULL);
}

unsigned long long getCurrentTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	unsigned long resultado;
	resultado = ((unsigned long long) tv.tv_sec) * 1000;
	resultado += ((unsigned long long)tv.tv_usec) / 1000;
	return resultado;
}

void agregarTimeStats (char *memoria,int marco,int tamanioPagina,unsigned long timestats){
	memcpy(memoria+marco*tamanioPagina,&timestats,sizeof(unsigned long));
}

void agregarKey (char *memoria,int marco,int tamanioPagina,uint16_t key){
	memcpy(memoria+marco*tamanioPagina+sizeof(unsigned long),&key,sizeof(uint16_t));
}

void agregarValor (char *memoria,int marco,int tamanioPagina,char *valor){
	memcpy(memoria+marco*tamanioPagina+sizeof(unsigned long)+sizeof(uint16_t),valor,strlen(valor));
}



#endif /* FUNCIONESAUXILIARES_C_ */
