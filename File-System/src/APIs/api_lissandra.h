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
//#define EXITSUCCESS 1
//#define EXITFAILURE 0
//#define STRONGCONSISTENCY "SC"
//#define EVENTUALCONSISTENCY "EC"
#include <stdbool.h>

//#include "../File-System.h"
#include "../config/config.h"
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/txt.h>
#include <commons/string.h>
#include <math.h>
#include "../../../biblioteca/biblioteca/parser.h"

typedef struct {
	int cantParticionesTemporales;
	int cantParticionesCompactacion;
	char* nombreDeLaTabla;
//	t_list *_inserts;//esto es del tipo RegistroLinea
	t_list* registros;
}Insert;

typedef struct {
		unsigned long timestamp;
		unsigned int key;
		char* value;
//		char value[lfs.tamanioValue];
}RegistroLinea;
//Insert* memtable_de_inserts;
t_list * memtable;//lista del tipo Insert , el select chequea esto tambien

//char* aux_tabla_para_la_memtable;

//funciones de la API de LFS,para ser consultadas IMPORTANTES estas 7 funciones, son la posta
void select1(const char * nombre_de_tabla, unsigned int key);
	//el timestamp es opcional
void insert_1(const char* nombre_de_tabla,unsigned int key , const char* value);
void insert_2(const char* nombre_de_tabla,unsigned int key , const char* value, unsigned  long long timestamp);
void create(const char* nombre_de_tabla,const char* tipo_consistencia,unsigned int numero_de_particiones,unsigned int tiempo_de_compactacion );
void describe1();
void describe2(const char* nombre_de_tabla);
void drop(const char* nombre_de_tabla);

int buscarParticion(char *tabla , unsigned int key);
//misma funcion de antes pero que solo reciben strings
void lfs_create(const char* nombre_de_tabla,const char* tipo_consistencia,const char*  numero_de_particiones,const char* tiempo_de_compactacion );


bool yaExisteTabla(const char* nombre_de_tabla);

typedef struct {
	char CONSISTENCY[3];//=SC
	unsigned int PARTITIONS;//=3
	unsigned long COMPACTION_TIME;//=60000
}Metadata_Tabla;

//auxiliares
char archivo_path(const char rutaMontaje, const char *rutaArchivo);
void guardar_en_temporal(Insert *insert,RegistroLinea *registro);
char *path_tables();
char *path_bloques();
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
void dumpear();// lo que hay en la memtable, bajar a las particiones .tmp
void insertarRegistrosEnParticionTemporal(const char* tabla,  RegistroLinea* unRegistro,int cantidadDeParticionesTemporales);
size_t longitudDeRegistroAlFileSystem(RegistroLinea* unRegistro);

int grabarRegistroABloques(RegistroLinea* unRegistro);
void escribirRegistroABloque(const char * bloque_path,RegistroLinea* unRegistro);
int particionSegunKey(RegistroLinea* unRegistro,unsigned int cantidad_de_particiones);
char* obtenerPathDelNumeroDeBloque(int numeroDeBloque);
char* obtenerPathDeParticionTemporal(int numeroDeParticionTemporal,char *tabla);
Insert* buscarTablaEnLaMemtable(const char * tabla);

#endif /* APIS_API_LISSANDRA_H_ */
