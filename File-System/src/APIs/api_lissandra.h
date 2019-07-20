/*
 * api_lissandra.h
 *
 *  Created on: 16 may. 2019
 *      Author: jhon
 */

#ifndef APIS_API_LISSANDRA_H_
#define APIS_API_LISSANDRA_H_

#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h> // para a estructura stat que me da la descripcion del archivo
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>

//#include "../File-System.h"
#include "../config/config.h"
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/txt.h>
#include <commons/string.h>
#include <math.h>


#include "../../../biblioteca/biblioteca/bibliotecaDeSockets.h"
#include "../../../biblioteca/biblioteca/parser.h"
#include "../config/metadata.h"
#include "../config/bitmap.h"

typedef struct {
	unsigned int cantParticionesTemporales;
	char nombreDeLaTabla[255];//255 caracteres como maximo tiene un nobre de un archivo
	unsigned int   cantParticiones;
	//registros, esto es del tipo RegistroLinea
	t_list* registros;
}Insert;
typedef struct {
		unsigned long long timestamp;
		uint16_t key;
		char* value;
		unsigned int particionCorrespondiente;
//		char value[lfs.tamanioValue];
}RegistroLinea;
//Insert* memtable_de_inserts;
t_list * memtable;//lista del tipo Insert , el select chequea esto tambien

//-----
typedef struct{
	RegistroLinea registro;
	unsigned int bloque,particion;
}RegistroLinea_compactador;
typedef struct{
	char pathParticion[256];
	bool esTemporal;
	unsigned int size;
	char** bloques;
}Particion;
//char* aux_tabla_para_la_memtable;
typedef struct {
	char CONSISTENCY[3];//=SC
	unsigned int PARTITIONS;//=3
	unsigned long COMPACTION_TIME;//=60000
}Metadata_Tabla;
//funciones de la API de LFS,para ser consultadas IMPORTANTES estas 7 funciones, son la posta
void select1(const char * nombre_de_tabla,unsigned int key);

void insert_1(const char* nombre_de_tabla,unsigned int key , const char* value);
void insert_2(const char* nombre_de_tabla,unsigned int key , const char* value, unsigned  long long timestamp);
void create(const char* nombre_de_tabla,const char* tipo_consistencia,unsigned int numero_de_particiones,unsigned int tiempo_de_compactacion );
void describe1();
void describe2(const char* nombre_de_tabla);
void drop(const char* nombre_de_tabla);

//misma funcion de antes pero que solo reciben strings y las que usa este proceso
void lfs_create(const char* nombre_de_tabla,const char* tipo_consistencia,const char*  numero_de_particiones,const char* tiempo_de_compactacion );
void lfs_drop(const char* nombre_de_tabla);
void lfs_insert1(const char* nombre_de_tabla,const char* key,const char* value );
void lfs_insert2(const char* nombre_de_tabla,const char* key, const char* value, const char* timestamp );
void lfs_describe();
void lfs_describe2(const char* nombre_de_tabla);


unsigned long long lfs_timestamp();
//auxiliares

void select_mostrar_lista_de_registros(const t_list* listaDeRegistros,const char* tabla,unsigned int key);
void registroLinea_mostrar(RegistroLinea* unRegistro);
//revuelve lista del tipo RegistroLinea
t_list*  filtrarRegistrosEnLaMemtable(const char* tabla,uint16_t key);
//devuelve una lista del tipo de RegistroLinea
t_list* buscarRegistrosEnParticiones(const char* tabla,uint16_t key);
//el timestamp es opcional
bool yaExisteTabla(const char* nombre_de_tabla);
char archivo_path(const char rutaMontaje, const char *rutaArchivo);

void mostrarMetadata(const char* nombreTabla);
void crearMetadata_v2(const char* pathTabla,const char* tipoConsistencia, unsigned int numeroParticiones,
		unsigned int tiempoCompactacion);

Metadata_Tabla *obtenerMetadata(const char* nombreTabla);
void crearTabla(const char* nombreDeTabla);//ok
bool yaExisteCarpeta(const char* path_tabla);
char*  obtenerPathDeTabla(const char* nombre_de_tabla);
void crearParticiones(const char* tabla, unsigned int numeroDeParticiones);

void insertarEnMemtable(const char* nombre_de_tabla,unsigned int key , const char* value,unsigned long long timestamp);
bool hay_datos_a_dumpear();
bool laMemtableTieneContenido();

// lo que hay en la memtable, bajar a las particiones .tmp
void dumpear();
//el insert contiene el nombre de la tabla
void memtable_reboot();
void insert_destroy(Insert* unInsert);
void registroLinea_destroy(RegistroLinea* unRegistro);
//el insert contiene el nombre de la tabla y la lista de registros
void insertarListaDeRegistrosDeTablaANuevaParticionTemporal(const Insert* unInsert);

//lista del tipo Bloque_LFS
t_list* calcularBloquesNecesarios(size_t size_);
size_t tamanioDeListaDeRegistros(t_list* listaDeRegistros);

void crearParticionTemporalConRegistros(const char* pathDeParticion,int size,t_list* bloques);


size_t longitudDeRegistroAlFileSystem(const RegistroLinea* unRegistro);

//bloques de tipo BloqueLFS, y registros del tipo RegistroLinea
void escribirRegistrosABloquesFS_v2(const t_list* bloques,const t_list* registros);
int particionSegunKey(RegistroLinea* unRegistro,unsigned int cantidad_de_particiones);
char* obtenerPathDelNumeroDeBloque(int numeroDeBloque);
char* obtenerPathDeParticionTemporal(const char* tabla,unsigned int  numeroDeParticionTemporal);

//char* obtenerPathDeParticionTemporal(numeroDeParticionTemporal);
Insert* buscarTablaEnLaMemtable(const char * tabla);


//auxiliares para imrpimir
void printfMetadata(Metadata_Tabla* metadata, const char* nombre_de_tabla);
void mostrarParticion(Particion* particion);
void memtable_mostrar();
void mostrar_insert(const Insert* unInsert);


#endif /* APIS_API_LISSANDRA_H_ */
