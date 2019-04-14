#include "parser.h"

static bool _esSELECT(char* linea);
static bool _esINSERT(char* linea);
static bool _esCREATE(char* linea);
static bool _esDESCRIBE(char* linea);
static bool _esDROP(char* linea);
static bool _esJOURNAL(char* linea);
static bool _esRUN(char* linea);
static bool _esADD(char* linea);

struct_operacion parsear_linea(char linea[]){
	struct_operacion operacion;

	if(_esSELECT(linea)){
		operacion.nombre_operacion = API_SELECT;
		operacion.parametros = string_split(linea + strlen("SELECT "), " ");

	}else if(_esINSERT(linea)){
		operacion.nombre_operacion = API_INSERT;
		operacion.parametros = string_split(linea + strlen("INSERT "), " ");

	}else if(_esCREATE(linea)){
		operacion.nombre_operacion = API_CREATE;
		operacion.parametros = string_split(linea + strlen("CREATE "), " ");

	}else if(_esDESCRIBE(linea)){
		operacion.nombre_operacion = API_DESCRIBE;
		operacion.parametros = string_split(linea + strlen("DESCRIBE "), " ");

	}else if(_esDROP(linea)){
		operacion.nombre_operacion = API_DROP;
		operacion.parametros = string_split(linea + strlen("DROP "), " ");

	}else if(_esJOURNAL(linea)){
		operacion.nombre_operacion = API_JOURNAL;
		operacion.parametros = NULL;

	}else if(_esRUN(linea)){
		operacion.nombre_operacion = API_RUN;
		operacion.parametros = string_split(linea + strlen("RUN "), " ");

	}else if(_esADD(linea)){
		operacion.nombre_operacion = API_ADD;
		operacion.parametros = string_split(linea + strlen("ADD "), " ");
	}

	return operacion;
}



static bool _esSELECT(char* linea){
	//SELECT [NOMBRE_TABLA] [KEY]
	return string_starts_with(linea, "SELECT ");
}

static bool _esINSERT(char* linea){
	//TODO: fix INSERT [NOMBRE_TABLA] [KEY] “[VALUE]” [Timestamp]
	//Ejemplos:
	//			INSERT TABLA1 3 “Mi nombre es Lissandra” 1548421507
	return string_starts_with(linea, "INSERT ");
}

static bool _esCREATE(char* linea){
	//CREATE [TABLA] [TIPO_CONSISTENCIA] [NUMERO_PARTICIONES] [COMPACTION_TIME]
	return string_starts_with(linea, "CREATE ");
}

static bool _esDESCRIBE(char* linea){
	//DESCRIBE [NOMBRE_TABLA]
	return string_starts_with(linea, "DESCRIBE ");
}

static bool _esDROP(char* linea){
	//DROP [NOMBRE_TABLA]
	return string_starts_with(linea, "DROP ");
}

static bool _esJOURNAL(char* linea){
	//JOURNAL
	return string_starts_with(linea, "JOURNAL ");
}

static bool _esRUN(char* linea){
	//RUN <path>
	return string_starts_with(linea, "RUN ");
}

static bool _esADD(char* linea){
	//ADD MEMORY [NÚMERO] TO [CRITERIO]
	return string_starts_with(linea, "ADD ");
}


void liberarListaDeStrings(char** operation) {
	string_iterate_lines(operation, (void*) free);
	free(operation);
}




