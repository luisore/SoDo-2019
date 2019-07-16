/*
 * metadata.h
 *
 *  Created on: 27 nov. 2018
 *      Author: utnso
 */

#ifndef CONFIG_METADATA_H_
#define CONFIG_METADATA_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h> //Commons config archivo
#include <commons/string.h> //Commons string

#include <commons/log.h>
#include <stdbool.h>
#include <unistd.h>

#include <stdarg.h>

#include <commons/txt.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>

//#include "../MDJ.h"
#include "config.h"


//METADATA
struct METADATA{
	int tamanio_de_bloque;//en bytes, ni dice eso el enunciado
	int cantidad_bloques;
	char* MAGIC_NUMBER;//es fifa , puede ser un charArray, charlable, no sirve , es la extension de los archivos d config escriptorio
}lfs_metadata;

void cargar_configuracion_metadata(const char* pathMetadata);
void mostrar_configuracion_metadata();

//METADATA

#endif /* CONFIG_METADATA_H_ */
