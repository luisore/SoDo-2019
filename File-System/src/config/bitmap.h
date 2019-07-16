/*
 * bitmap.h
 *
 *  Created on: 15 nov. 2018
 *      Author: jhondaniel
 */

#ifndef FILE_SYSTEM_BITMAP_H_
#define FILE_SYSTEM_BITMAP_H_
#include <string.h>
#include <stdio.h>
#include <commons/bitarray.h>
#include <commons/txt.h>
#include <stdbool.h>

#include <stdlib.h>
#include "metadata.h"

typedef struct{
	char* path;
	unsigned int numero;
}Bloque_LFS;

t_bitarray* bitarray_;


Bloque_LFS* lfs_obtenerBloqueLibre();
char* bitmap_obtener_path();
t_list* bloquesToList(const char* pathFile);//Ok



void setear_bloque_ocupado_en_posicion(off_t pos);
bool testear_bloque_libre_en_posicion(int pos);
void mostrar_bitarray();
void cargar_configuracion_bitmap();


//la posicion del bloque que devuelve se reserva en el bitmap
int getBloqueLibre_int();//obtiene el proximo bloque libre ,OK

//auxiliares
char* bloquesToString(const char* pathFile);
void mostrarLista(t_list* listaDeBloques_);
char* recortarString(const char* stream, off_t desde, off_t size);
//size_t getCantidadDeBloquesOcupadosSegunPath(const char* pathFile);//OK



//BITMAP end

//void cargar_configuracion_bitmap(char bitmap_array[], int cantidadDeBytes);
//
//void setear_bloque_ocupado_en_posicion(off_t pos);//ok
//
//void setear_bloque_libre_en_posicion(off_t pos);//ok
//
//bool testear_bloque_libre_en_posicion(int pos);
//void mostrar_bitarray();
//void setBloqueActuaLleno();//agregar un 1 al bitmap.bin
//
//void getBloqueLibre_path();
//bool estaLibreElBloqueActual(FILE* bloqueActual, int tamanioDeBloque);

#endif /* FILE_SYSTEM_BITMAP_H_ */
