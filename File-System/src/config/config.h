/*
 * config.h
 *
 *  Created on: 12 abr. 2019
 *      Author: jhon
 */

#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_
#define LEN_MAXIMO 100
#include <commons/config.h>

typedef struct {
	unsigned int tamanioBloque,cantidadDeBloques;
	char magicNumber[3];
}Metadata;
typedef struct {
	unsigned int  cantidadDeBytesOcupado;
}Bitmap;

typedef struct {
	 int puertoEscucha,tamanioValue;
	long retardo,tiempoDump;
	char* puntoDeMontaje;//[LEN_MAXIMO];
//	 Metadata metadata;
//	 Bitmap bitmap;
}LFS;
LFS lfs;

void probando();

void config_cargar(const char* dir_config);//cargar configuracion desde un archivo

void imprimir_configuracion();
#endif /* CONFIG_CONFIG_H_ */
