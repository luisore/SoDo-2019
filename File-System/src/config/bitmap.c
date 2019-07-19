/*
 * bitmap.c
 *
 *  Created on: 15 nov. 2018
 *      Author: jhondaniel
 */
#include "bitmap.h"



//BITMAP begin
void cargar_configuracion_bitmap(){
	bitarray_=NULL;
	char* bitmap_array=malloc(sizeof(char)*(lfs_metadata.cantidad_bloques)/8);//strdup(bits);
	memset(bitmap_array,0b00000000,sizeof(char)*(lfs_metadata.cantidad_bloques)/8);
//	char* aux_path_bitmap=bitmap_obtener_path();
//	char* bits=fileToString_v2(aux_path_bitmap);
//	free(bits);//free para probar
//	free(aux_path_bitmap);

	bitarray_ = bitarray_create_with_mode(bitmap_array,lfs_metadata.cantidad_bloques/8,MSB_FIRST);// LSB_FIRST);
//	for(int i=0;i<bitarray_get_max_bit(bitarray_);i++)setear_bloque_ocupado_en_posicion(i);//setea todos los bloques, OK
//	mostrar_bitarray();

}
char* bitmap_obtener_path(){
	char* path=malloc(sizeof(char)*(strlen(lfs.puntoDeMontaje)+strlen("Metadata/")+strlen("Bitmap.bin")+2));
	sprintf(path,"%sMetadata/Bitmap.bin",lfs.puntoDeMontaje);
	printf("bitmap_obtener_path() , path:%s\n",path);
	return path;
}
void setear_bloque_ocupado_en_posicion(off_t pos){//ok
	bitarray_set_bit(bitarray_,pos);
}
void setear_bloque_libre_en_posicion(off_t pos){//ok
	bitarray_clean_bit(bitarray_,pos);
}
bool testear_bloque_libre_en_posicion(int pos){
	return bitarray_test_bit(bitarray_,(off_t)(pos));
}
void mostrar_bitarray(){
	for(int k =0;k<(bitarray_get_max_bit(bitarray_));k++)printf("test bit posicion, es  %d en posicion %d \n", bitarray_test_bit(bitarray_,k),k);
}
void setBloqueActuaLleno(unsigned int bloqueActual_int){//agregar un 1 al bitmap.bin
	bitarray_set_bit(bitarray_,bloqueActual_int);
}
Bloque_LFS* lfs_obtenerBloqueLibre(){
	puts("lfs_obtenerBloqueLibre() begin ");
	Bloque_LFS* unBloqueLibre=malloc(sizeof(Bloque_LFS));
	unBloqueLibre->numero=getBloqueLibre_int();
	unBloqueLibre->path=(char*)malloc(strlen(lfs.puntoDeMontaje)+strlen("/bloques/")+strlen("xxxxxx.bin"));//solo un tamanio maximo para longitud de path
	sprintf(unBloqueLibre->path,"%s%s%d.bin",lfs.puntoDeMontaje,"Bloques/",unBloqueLibre->numero);//ok
	FILE* f_aux= fopen(unBloqueLibre->path,"w+");//txt_open_for_append(path_bloque); SI LO ABRO COMO "W" SE BORRA EL CONTENIDO
	if(f_aux==NULL)perror("lfs_obtenerBloqueLibre()");
	txt_close_file(f_aux);
	printf("lfs_obtenerBloqueLibre() bloque %d y path %s", unBloqueLibre->numero,unBloqueLibre->path);
	return unBloqueLibre;
}
void bloque_destroy(Bloque_LFS* bloque){
	free(bloque->path);
//	free(bloque);
}

unsigned int getBloqueLibre_int(){//obtiene el proximo bloque libre ,OK
	unsigned int  bloque_i;
	for( bloque_i =0;testear_bloque_libre_en_posicion(bloque_i);bloque_i++);//hasta un bloque lbre,OK

	if(bloque_i>lfs_metadata.cantidad_bloques){
		perror("cantidad insuficiente de espacio o bloques ");
	}
	setear_bloque_ocupado_en_posicion(bloque_i);
	return bloque_i;
}
char* bloquesToString(const char* pathFile){//ok
//	puts("begin bloquesToString()");
	char* bloques=NULL;
	char* path_aux = malloc((5024)*sizeof(char));
	sprintf(path_aux,"%s%s%s",lfs.puntoDeMontaje,"Archivos/",pathFile);
//	puts(path_aux);
	t_list* listaDeBloques =  bloquesToList(path_aux);
//	mostrarLista(listaDeBloques);
	for (int var = 0; var < list_size(listaDeBloques); var++) {
		sprintf(path_aux,"%s%s%d.bin",lfs.puntoDeMontaje,"Bloques/",list_get(listaDeBloques,var));
		bloques=str_concat(bloques,fileToString_v2(path_aux));
	}
	free(path_aux);
	list_destroy(listaDeBloques);
//	puts("end bloquesToString()");
	return bloques;

}
t_list* bloquesToList(const char* pathFile){//Ok
	t_config* scriptGuardado=config_create(pathFile);
		if(scriptGuardado==NULL)fprintf(stderr,"-> bloquesToList() , no existe path:%s\n",pathFile);
			char** listaDeBloques=config_get_array_value(scriptGuardado,"BLOQUES");
			config_destroy(scriptGuardado);
			t_list* listaDeBloques_ = list_create();
			for(int aux=0;listaDeBloques[aux]!=NULL;aux++){
				int unBloque = atoi(listaDeBloques[aux]);
				list_add(listaDeBloques_,unBloque);
			}
	string_trim(listaDeBloques);//free() para char**
	return listaDeBloques_;
}
void mostrarLista(t_list* listaDeBloques_){
	puts("--inicio, mostrando lista de  bloques ");
	for(int p =0; p<list_size(listaDeBloques_);p++){
				puts(intToString(list_get(listaDeBloques_,p)));
			}
	printf("--fin muestra de lista de bloques, total bloques : %d\n",list_size(listaDeBloques_));

}
char* recortarString(const char* stream, off_t desde, off_t size){
	char* parcial = string_substring(stream,(off_t)desde,(off_t)size);
	return parcial;
}
//size_t getCantidadDeBloquesOcupadosSegunPath(const char* pathFile){//OK
//	size_t cantidad=0;
//	t_list* listaDeBloques=bloquesToList(pathFile);
//	cantidad=list_size(listaDeBloques);
//	list_create(listaDeBloques);
//	return cantidad;
//}
//BITMAP end



