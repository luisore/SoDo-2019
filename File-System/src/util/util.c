/*
 * util.c
 *
 *  Created on: 13 nov. 2018
 *      Author: jhondaniel
 */
#include "util.h"

char* recortarPrimerosCaracteres(char* s, int primerosCaracteres){//ok y  malloquea automaticamente
	char* recorte=NULL;
	if(s==NULL ||  strlen(s)==0){
		perror("la cadena a recortar es es nula o termino de recortar cadena \n");
		return strdup("") ;
	}
	else if(strlen(s)<primerosCaracteres){
		recorte=strndup(s,primerosCaracteres);
		strcpy(s,"");
		return recorte;
	}
	recorte= strndup(s,primerosCaracteres);
	char* aux=strdup(s);
//	if(strlen(s)<primerosCaracteres)perror("Error en recorte de caracteres \n");
	strcpy(s,aux+primerosCaracteres);
	free(aux);
	return recorte;
}
int cantidadDeCaracteres_file(FILE* bloque){

	int i ;
	for(i=0;getc(bloque)!=EOF;i++);
	return i;
}
int cantidadDeCaracteres_path(char* path ){//OK
	FILE * f = fopen(path,"r+");
	int n = cantidadDeCaracteres_file(f);
	fclose(f);
	return n ;
}
bool quedaContenidoParaMapear(char* contenido){return strlen(contenido)>0 && contenido!=NULL;}
void crearBloques(int cantidad){//ok, se puede mejorar con realloc
//	int n = metadata.cantidad_bloques;
	for(int var = 0;var<cantidad;var++){
		char* unPath = malloc(100);
		sprintf(unPath,"%d.bin",var);
		FILE* f = fopen(unPath,"w+");
		txt_close_file(f);
		free(unPath);
	}
}
char* str_concat( const char* from,const char* add){//ok, devuelve algo malloqueado,OK,guarda
	if(from==NULL)return strdup(add);
	else{
		int malloq=(strlen(from)+strlen(add))*sizeof(char);
		char* aux = (char*)malloc(malloq);
		sprintf(aux,"%s%s",from,add);
//		free(from);
		return aux;
	}
}
char*  str_concat_v2(const char* from,const char* add){//ok
	if(strlen(from)==0 || strlen(add)==0 || from==NULL )perror("str_concat_v2 from o add nulos");
	char* s=(char*)malloc((strlen(from)+strlen(add))*sizeof(char));
//	char* aux=from;
	strcat(s,from);
	strcat(s,add);
	return s;
}
void mostrarCaracteres(const char* string){

	printf("%d de len de %s \n ",strlen(string), string);
	for(int i =0;i<strlen(string);i++){
		printf("pos i = %d y char = %c \n", i,string[i]);
	}
}


char* intToString( int n){//OK, atoa y atoi ya existen igual
//	int cantidadDeDigitos;
//	int resto=n*10;
//	for(cantidadDeDigitos=0;resto%10==0;cantidadDeDigitos++)resto=resto/10;

//	char* aux = malloc(cantidadDeDigitos*sizeof(char));
	char* aux = malloc(5);
	sprintf(aux,"%d",n);
	return aux;
}
off_t cantidadDeCaracteresDeFile(const char* pathFile){//usar esto
	off_t cantidad=0;
	struct stat inFoDelInodo;
	stat(pathFile,&inFoDelInodo);
	cantidad=inFoDelInodo.st_size;
	return cantidad;
}
char* fileToString(const char* pathFile){//en revision ,deberia de funcionar
	int cantidad=cantidadDeCaracteresDeFile(pathFile);
	char aux[cantidad];
	char* string =strdup(aux);
	FILE* file = fopen(pathFile,"r+");
	int p = 0;
	for(char letra = getc(file);letra!=EOF;letra=getc(file)){
		string[p]=letra;
		p++;
	}
	fclose(file);
	return string;
}
char * fileToString_v2(const char* pathFile){//ok,
	int cantidad=cantidadDeCaracteresDeFile(pathFile);
	FILE* file = fopen(pathFile,"r+");
		//abrimos la proyeccion del segundo archivo para escritura, de forma compartida
		   char*  memoria2 = (char *)mmap(0, cantidad,PROT_WRITE, MAP_SHARED, fileno(file), 0);
		   char* string =strdup(memoria2);
		    if (memoria2 == MAP_FAILED) {//retorna en memoria la direccion principal de la proyeccion
		        perror("Fallo la proyeccion2");//si fallo devuelve MAP_FAILED
//		        exit(-1);
		    }
		    if (munmap (memoria2, cantidad) == -1) {//cerrar la proyeccion.
		        printf("Error al cerrar la proyeccion \n");
//		        exit(-1);
		    }
	fclose(file);
	return string;
}






