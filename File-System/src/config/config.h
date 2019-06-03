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
#include <commons/log.h>

t_log * logger ; // variable del archivo de loggeo de hitos

//typedef struct {
//	unsigned int tamanioBloque,cantidadDeBloques;
//	char magicNumber[3];
//}Metadata_LFS;
//typedef struct {
//	unsigned int  cantidadDeBytesOcupado;
//}Bitmap;

typedef struct {
	 int puertoEscucha,tamanioValue;
	long retardo,tiempoDump;
	char* puntoDeMontaje;//[LEN_MAXIMO];
//	 Metadata_LFS metadata;
//	 Bitmap bitmap;
}LFS;
LFS lfs;//variable global que contiene los valores de LFS.config

void probando();

void config_cargar(const char* dir_config);//cargar configuracion desde un archivo

void imprimir_configuracion();//imprime la configuracion en pantalla



#endif /* CONFIG_CONFIG_H_ */
