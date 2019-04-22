/*
 * PaginacionSegmentacion.h
 *
 *  Created on: 22 abr. 2019
 *      Author: utnso
 */

#ifndef PAGINACIONSEGMENTACION_H_
#define PAGINACIONSEGMENTACION_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/bitarray.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>


char *crearMemoria(int cantidadDebytes);
void liberarMemoria(char **memoria);
int paginaLibre();
t_bitarray *crearBitmap(int cantidadDepagina,char *direccionArchivoBitmap);
void setearPaginaEnOcupada(int numeroDepagina);
t_bitarray *bitarray;

#endif /* PAGINACIONSEGMENTACION_H_ */
