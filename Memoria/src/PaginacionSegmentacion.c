/*
 * PaginacionSegmentacion.c
 *
 *  Created on: 22 abr. 2019
 *      Author: utnso
 */

#include "PaginacionSegmentacion.h"

char *crearMemoria(int cantidadBytes){
	char *memoria;
	memoria=malloc(cantidadBytes);
	memset(memoria,0,cantidadBytes);
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
		list_destroy(segmento->direccionTablaDePaginas);
		list_remove(tablaSegmentos,i);
	}
	list_destroy(tablaSegmentos);
}

int existeTablaEnTablaDeSegmento(char *nombreTabla,t_list *tablaDeSegmentos){
	int encontrado = -1;
	int cantidadSegmentos=list_size(tablaDeSegmentos);
	segmentacion *segmento;
	for(int nodo=0;nodo <cantidadSegmentos ;nodo ++){
		segmento =list_get(tablaDeSegmentos,nodo);
		if(!strncmp(segmento->nombreTabla,nombreTabla, strlen(nombreTabla))){
			puts("algo");
			return nodo;
		}
	}
	return encontrado;
}

int agregarDatOaTabla(char *nombreTabla,t_bitarray *marco,t_bitarray *modificado,int tamanioValue,t_list *tablaDeSegmentos,int cantidadDePaginas){
	int hayPaginaLibre = paginaLibre(marco,cantidadDePaginas);
	//no hay espacio en memoria
	//Falta buscar en algoritmo de reemplazo
	if (hayPaginaLibre<0){
		return -1;
	}
	else{
		bitarray_set_bit(marco,hayPaginaLibre);
	}
	segmentacion *unNuevoSegmento;
	int numeroNodoEnTablaDeSegmentos =existeTablaEnTablaDeSegmento(nombreTabla,tablaDeSegmentos);
	//unNuevoSegmento=nuevoSegmento(nombreTabla,tablaDeSegmentos );
	if(numeroNodoEnTablaDeSegmentos>=0){
		unNuevoSegmento=list_get(tablaDeSegmentos,numeroNodoEnTablaDeSegmentos);
	}
	else{
		unNuevoSegmento=nuevoSegmento(nombreTabla,tablaDeSegmentos );
	}
	crear_NodoDePaginaYagregaTabla(hayPaginaLibre,tamanioValue,unNuevoSegmento->direccionTablaDePaginas);
	return hayPaginaLibre;
}

void crear_NodoDePaginaYagregaTabla(int numeroPagina,int tamanioValue,t_list *tablaPaginas){
	paginacion *unaPagina;
	unaPagina = malloc(sizeof(paginacion));
	unaPagina->numeroPagina = numeroPagina;
	unaPagina->direccionFisica = numeroPagina *(tamanioValue+sizeof(uint16_t)+sizeof(unsigned long));
	agregar_tabla(tablaPaginas,unaPagina);
}

segmentacion* nuevoSegmento(char *nombreTabla,t_list *tablaDeSegmentos ){
	segmentacion *nuevoSegmento;
	nuevoSegmento = malloc (sizeof (segmentacion));
	nuevoSegmento->nombreTabla =  malloc (strlen(nombreTabla)+1);
	strcpy(nuevoSegmento->nombreTabla,nombreTabla);
	nuevoSegmento->direccionTablaDePaginas  = crearTablaDePaginas();
	list_add(tablaDeSegmentos,nuevoSegmento);
	return nuevoSegmento;
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
t_bitarray *crearBitmap(int cantidadDepagina,int diferencia){
		size_t tamanio =cantidadDepagina/8;
		//printf("tamanio %d: \n",tamanio);
		if (diferencia >0){
			tamanio++;
		//	printf("algo %d: \n",tamanio);
		}
		char bmap[tamanio];
		//memset(bmap,0,tamanio);
		bzero(bmap,tamanio);
		//printf("tamanio %d: \n",tamanio);
		bitarray = bitarray_create_with_mode(bmap,tamanio, MSB_FIRST);
		size_t	cantidadDebits= bitarray_get_max_bit (bitarray);
		for (int i=0;i<cantidadDebits;i++){
			//printf("posicion %d valor %d:\n",i,bitarray_test_bit(bitarray,i));
		}
		return bitarray;

}



//Buscar Pagina libre
int paginaLibre(t_bitarray *marco,int cantidadDePaginas){
	size_t	cantidadDebits= cantidadDePaginas;
	int i;
	int libre=-1;
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

void guardarEnMemoria(char *memoria,int marco,int valor,struct_operacion* operacion){
	char *registro;
	int tam_registro = sizeof(unsigned long)+sizeof(int)+valor;
	//registro = malloc(tam_registro);
	memcpy(memoria+marco*tam_registro,(operacion->parametros)[3],sizeof(unsigned long));
	memcpy(memoria+marco*tam_registro+sizeof(unsigned long),(operacion->parametros)[1],sizeof(int));
	memcpy(memoria+marco*tam_registro+sizeof(unsigned long)+sizeof(int),(operacion->parametros)[3],sizeof(unsigned long));

}
