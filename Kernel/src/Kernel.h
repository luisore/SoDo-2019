#ifndef KERNEL_H_
#define KERNEL_H_

#include "../../biblioteca/biblioteca/bibliotecaDeSockets.h"
#include "../../biblioteca/biblioteca/parser.h"
#include <commons/collections/list.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"//Configuracion y Logger

int inicializar();
void kernel_exit();
void abrir_script(char* path);
void kernel_ejecutar(struct_operacion* operacion);


#endif /* KERNEL_H_ */
