/*
 * PaginacionSegmentacion.c
 *
 *  Created on: 22 abr. 2019
 *      Author: utnso
 */

#include "PaginacionSegmentacion.h"

char *crearMemoria(int cantidadBytes){
	char *memoria;
	memoria=calloc(1,cantidadBytes);
	return memoria;
}

t_list * crearTablaDePaginas(){
	return list_create();
}

t_list * crearTablaDeSegmentos(){
	return list_create();
}

int listaLFSvacia(t_list *listaLFS){
	return list_is_empty(listaLFS);
}

void liberarPaginacionSementacion(t_list *tablaSegmentos){
	int cantidadSegmentos=list_size(tablaSegmentos);
	segmentacion *segmento;
	int cantPaginas;
	for(int i=0;i<cantidadSegmentos;i++){
		segmento =list_get(tablaSegmentos,i);
		cantPaginas=list_size(segmento->direccionTablaDePaginas);
		for(int k=0;k<cantPaginas;k++){
			list_remove(segmento->direccionTablaDePaginas,k);
		}
		list_clean(segmento->direccionTablaDePaginas);
		list_remove(tablaSegmentos,i);
	}
}

void agregar_tabla(t_list *tablaPaginas,paginacion *unaPagina){
	list_add(tablaPaginas,&unaPagina);
}

void crear_TablaDePaginaParaSegmento(segmentacion *segmento){
	segmento->direccionTablaDePaginas=crearTablaDePaginas();
}

registroTabla *buscarPagina(t_list *tablaPaginas,uint16_t keyDeBusqueda,int *numeroDePagina,int tam,char *nombreTabla, void *memoria){
	int cantPaginasDeLaTabla=list_size(tablaPaginas);
	paginacion *paginaDelNodo;
	registroTabla *registroDeLaTabla;
	registroDeLaTabla = malloc(sizeof(registroTabla));
	registroDeLaTabla->nombreTabla = nombreTabla;
	registroDeLaTabla->value = malloc(sizeof(char)*tam);
	int noEncontro = -1;
	numeroDePagina=&noEncontro;
	paginaDelNodo=malloc(sizeof(paginacion));
	int tamanioDePagina;
	tamanioDePagina= tam + sizeof(unsigned long) + sizeof(uint16_t);
	char *value;
	uint16_t key;
	for(int i=0;i<cantPaginasDeLaTabla;i++){
		paginaDelNodo =list_get(tablaPaginas,i);
		int dirkey=paginaDelNodo->direccionFisica + sizeof(unsigned long);
		memcpy(memoria+dirkey,&key,sizeof(uint16_t));
		if(key == keyDeBusqueda){
			registroDeLaTabla->key=key;
			numeroDePagina = &paginaDelNodo->numeroPagina;
			memcpy(memoria+i*(tamanioDePagina),&registroDeLaTabla->epoc,sizeof(unsigned long));
			memcpy(memoria+i*(tamanioDePagina)+sizeof(unsigned long)+sizeof(uint16_t),&registroDeLaTabla->value,tam);
			break;
		}
	}
	return registroDeLaTabla;
}

void liberarMemoria(char **memoria){
	free(memoria);
}

void limpiarBitMap(t_bitarray *bitmap){
	int cantDepaginas=cantidadDePaginas(bitmap);
	for (int i=0;i<cantDepaginas;i++){
		bitarray_clean_bit(bitmap,i);
	}
}
void destruirBitmap(t_bitarray *bitmap){
	bitarray_destroy(bitmap);
}

int cantidadDePaginas(t_bitarray *bitmap){
	return	bitarray_get_max_bit(bitmap);
}

//Para crear una X cantidad de paginas en la memoria
//Esta funcion se va a usar 2 veces
	// 1-para crear el bitmap de paginas en uso
	// 2-para setear las paginas en estado modificado
t_bitarray *crearBitmap(int cantidadDepagina,char *direccionArchivoBitmap){
		char *direccionArchivoBitMap = direccionArchivoBitmap;
		int bitmap = open(direccionArchivoBitMap, O_RDWR);
		struct stat mystat;
		//puts(bitmap);
		if (fstat(bitmap, &mystat) < 0) {
		    //printf("Error al establecer fstat\n");
		    close(bitmap);
		}
	    char *bmap ;
		bmap = mmap(NULL, mystat.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, bitmap, 0);

		if (bmap == MAP_FAILED) {
				//printf("Error al mapear a memoria: %s\n", strerror(errno));

		}

		bitarray = bitarray_create_with_mode(bmap,cantidadDepagina , MSB_FIRST);
		size_t	cantidadDebits= bitarray_get_max_bit (bitarray);
		for (int i=0;i<cantidadDebits;i++){
			//printf("posicion %d valor %d:\n",i,bitarray_test_bit(bitarray,i));
		}
		return bitarray;

}

//Buscar Pagina libre
int paginaLibre(){
	size_t	cantidadDebits= bitarray_get_max_bit (bitarray);
	int i;
	int libre=0;
	for (i=0;i<cantidadDebits;i++){
		if(bitarray_test_bit(bitarray,i)==0){
			libre=i;
			break;
		}
	}
	return libre;
}
//Ocupar pagina

void setearPaginaEnOcupada(int numeroDepagina){
	bitarray_set_bit(bitarray, numeroDepagina);
}

int buscarPaginasContiguas(int cantidadDepaginasContiguas){
	size_t	cantidadDebits= bitarray_get_max_bit (bitarray);
	int i;
	int libre=0;
	int primeraPaginaContiguaLibre;
	for (i=0;i<cantidadDebits;i++){
		if(bitarray_test_bit(bitarray,i)==0){
			if(libre==0){
				primeraPaginaContiguaLibre=i;
			}
			libre++;
		}
		else{
			libre=0;
		}
		if(libre==cantidadDepaginasContiguas){
			break;
		}
	}
	return primeraPaginaContiguaLibre;
}

