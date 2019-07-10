/*
 * FuncionesAuxiliares.h
 *
 *  Created on: 22 abr. 2019
 *      Author: utnso
 */

#ifndef FUNCIONESAUXILIARES_H_
#define FUNCIONESAUXILIARES_H_

#include <time.h>
#include <stdint.h>
#include <string.h>

unsigned long funcionEpoc();
unsigned long long getCurrentTime() ;
void agregarValor (char *memoria,int marco,int tamanioPagina,char *valor);
void agregarKey (char *memoria,int marco,int tamanioPagina,uint16_t key);
void agregarTimeStats (char *memoria,int marco,int tamanioPagina,unsigned long timestats);
#endif /* FUNCIONESAUXILIARES_H_ */
