/*
 * PaginacionSegmentacion.h
 *
 *  Created on: 22 abr. 2019
 *      Author: utnso
 */

#ifndef PAGINACIONSEGMENTACION_H_
#define PAGINACIONSEGMENTACION_H_


#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../biblioteca/biblioteca/parser.h"


typedef struct {
	int numeroPagina;
	int direccionFisica;
} paginacion;


typedef struct {
	char* nombreTabla;
	t_list * direccionTablaDePaginas;
} segmentacion;

typedef struct{
	char* nombreTabla;
	char* value;
	uint16_t key;
	unsigned long epoc;
} registroTabla;

int hayPaginaLibre();
void crear_NodoDePaginaYagregaTabla(int numeroPagina,int tamanioValue,t_list *tablaPaginas);
void agregar_tabla(t_list *tablaPaginas,paginacion *unaPagina);
registroTabla *buscarPagina(t_list *tablaPaginas,uint16_t keyDeBusqueda,int *numeroDeNodo,int tam,char *nombreTabla,void *memoria);
segmentacion *nuevoSegmento(char *nombreTabla,t_list *tablaDeSegmentos );
int agregarDatOaMemoria(char *nombreTabla,t_bitarray *marco,t_bitarray *modificado,int tamanioValue,t_list *tablaDeSegmentos,void *memoria);
void destruirBitmap(t_bitarray *bitmap);
void limpiarBitMap(t_bitarray *bitmap);
void liberarPaginacionSementacion(t_list *tablaSegmentos);
int cantidadDePaginas(t_bitarray *bitmap);
int listaLFSvacia(t_list *listaLFS);
void liberarPaginacionSementacion(t_list *tablaSegmentos);
void agregar_TablaDePaginaDeSegmento(segmentacion *segmento);
void agregar_pagina(t_list *tablaPaginas,paginacion *unaPagina);
t_list *crearTablaDePaginas();
char *crearMemoria(int cantidadDebytes);
void liberarMemoria(char **memoria);
int paginaLibre(t_bitarray* marco);
int existeTablaEnTablaDeSegmento(char *nombreTabla,t_list *tablaDeSegmentos);
t_bitarray *crearBitmap(int cantidadDepagina);
void setearPaginaEnOcupada(int numeroDepagina);
t_bitarray *bitarray;
void guardarEnMemoria(char *memoria,int marco,int valor,struct_operacion* operacion);

#endif /* PAGINACIONSEGMENTACION_H_ */
