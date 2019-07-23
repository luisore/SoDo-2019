#ifndef BIBLIOTECADESOCKETS_H_
#define BIBLIOTECADESOCKETS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <fcntl.h>
#include <commons/log.h>

//Estructura para manejar el protocolo
typedef enum{
	HANDSHAKE_LFS,
	VALOR_LFS,
	SELECT,
	INSERT,
	DROP,
	CREATE,
	JOURNAL,
	VERIFICAR_TABLA,
	PEDIR_GOSSIPING,
	ENVIAR_GOSSIPING,

	SELECT_RESULTADO,
	INSERT_TERMINADO,
}t_protocolo;


enum resultado{
	CORRECTO,
	ERROR,
};
enum cosistencia{
	SC,
	EC,
	SHC,
};



typedef struct {
	char* nombreTabla;
	uint16_t key;
} struct_select;

typedef struct {
	char*  nombreTabla;
	int  tipo;
	int numeroParticiones;
	int tiempoCompactacion;
} struct_create;

typedef struct {
	char*  nombreTabla;
	uint16_t key;
	char *valor;
	unsigned long timestats;
} struct_insert;

typedef struct {
	char* nombreTabla;
} struct_tabla;


typedef struct {
	int cantidad;
	t_list *lista;
} struct_journal_tabla;



typedef struct {
	uint16_t key;
	char *valor;
	unsigned long long timestats;;
} struct_registro;

typedef struct {
	int valor;
} valor_tamanio;

typedef struct{
	t_list *lista;
}t_tabla_gossiping;

typedef struct{
	t_list *lista;
}t_lista_enviar;

typedef struct{
	int id_memoria;
	char *ip;
	int puerto;
}t_nodo_tabla_gossiping;

typedef struct{
	char* path;
	int pid;
	int acceso;
}info_archivo;

t_log* log_s;

int crearSocket(int *mySocket);

int conectar(int* mySocket, int puerto, char *ip);

int setearParaEscuchar(int *mySocket, int puerto);

int aceptarConexion(int);

int escuchar(int socket, fd_set *listaDeSockets,
	void *(*funcionParaSocketNuevo)(int, void*), void *parametrosParaSocketNuevo,
	void *(*funcionParaSocketYaConectado)(int, void*),
	void *parametrosParaSocketYaConectado);


//funciones para crear paquetes
struct_select* crear_select(uint16_t key,char *nombreTabla);



void* recibirYDeserializar(int socket,int tipo);
char* recibirYDeserializarString(int socket);
int* recibirYDeserializarEntero(int socket);
uint16_t* recibirYDeserializarUint16(int socket);
unsigned long* recibirYDeserializarUnsignedLong(int socket);

void serializarYEnviar(int socket, int tipoDePaquete, void* package);
void serializarYEnviarString(int socket, char *string);
void serializarYEnviarEntero(int socket, int *numero);
void serializarYEnviarUint16(int socket, uint16_t *numero);
void serializarYEnviarUnsignedLong(int socket, unsigned long *numero);

int enviarTodo(int socketReceptor, void *buffer, int *cantidadTotal);

int validarArchivoConfig(char *archivo);


#endif /* BIBLIOTECADESOCKETS_H_ */
