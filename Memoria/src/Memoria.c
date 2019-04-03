/*
 ============================================================================
 Name        : Memoria.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "Memoria.h"


int main(void) {
	puts("!!!Hello World321!!!");
	log_Memoria = log_create("Memoria.log","Memoria",false,LOG_LEVEL_INFO);
	/* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}
