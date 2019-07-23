#include "parser.h"

static bool _esSELECT(char* linea);
static bool _esINSERT(char* linea);
static bool _esCREATE(char* linea);
static bool _esDESCRIBE(char* linea);
static bool _esDROP(char* linea);
static bool _esJOURNAL(char* linea);
static bool _esRUN(char* linea);
static bool _esADD(char* linea);
static bool _esMETRICS(char* linea);

/*struct_operacion* operacion = parsear_linea("INSERT TABLA1 3 “Mi nombre es Lissandra” 1548421507");
 *
 * RESULTADO:
 * operacion->nombre_operacion = API INSERT;
 * (operacion->parametros)[0] = "TABLA1";
 * (operacion->parametros)[1] = "3";
 * (operacion->parametros)[2] = "Mi nombre es Lissandra";
 * (operacion->parametros)[3] = "154821507";
 * (operacion->parametros)[4] = NULL;
 *
 */
struct_operacion* parsear_linea(char linea[]){
	struct_operacion* operacion = (struct_operacion *)malloc(sizeof(struct_operacion));

	if(_esSELECT(linea)){
		operacion->nombre_operacion = API_SELECT;
		operacion->parametros = string_split(linea + strlen("SELECT "), " ");


	}else if(_esINSERT(linea)){
		operacion->nombre_operacion = API_INSERT;
		operacion->parametros = malloc(5 * sizeof(char *));

		//retorna ["TABLA1", "3", "“Mi nombre es Lissandra” 1548421507", NULL]
		char **aux = string_n_split(linea + strlen("INSERT "),3," ");

		//aux[2] = "“Mi nombre es Lissandra” 1548421507"
		//retorna ["Mi nombre es Lissandra", " 154821507", NULL]
		char *split = "\"";
		char **aux2 = string_split(aux[2] ,split );

		printf("algo %s \n",aux2[0]);
		printf("algo %s \n",aux2[1]+1);
		printf("algo %s \n",aux[0]);
		printf("algo %s \n",aux[1]);

		printf("algo %s \n",aux[2]);

		(operacion->parametros)[0] = strdup(aux[0]);
		(operacion->parametros)[1] = strdup(aux[1]);
		(operacion->parametros)[2] = strdup(aux2[0]);
		(operacion->parametros)[3] = strdup(aux2[1]+1);
		(operacion->parametros)[4] = NULL;

		free_list_of_strings(aux);
		free_list_of_strings(aux2);

	}else if(_esCREATE(linea)){
		operacion->nombre_operacion = API_CREATE;
		operacion->parametros = string_split(linea + strlen("CREATE "), " ");

	}else if(_esDESCRIBE(linea)){
		operacion->nombre_operacion = API_DESCRIBE;
		operacion->parametros = string_split(linea + strlen("DESCRIBE "), " ");

		//chequea si tiene el nombre de la tabla
		if((operacion->parametros)[0] != NULL){
			//elimino el \n al final
			char *next = NULL;
			(operacion->parametros)[0] = strtok_r((operacion->parametros)[0],"\n",&next);
		}

		string_iterate_lines(operacion->parametros, (void*) puts);

	}else if(_esDROP(linea)){
		operacion->nombre_operacion = API_DROP;
		operacion->parametros = string_split(linea + strlen("DROP "), " ");

		//elimino el \n al final
		char *next = NULL;
		(operacion->parametros)[0] = strtok_r((operacion->parametros)[0],"\n",&next);

	}else if(_esJOURNAL(linea)){
		operacion->nombre_operacion = API_JOURNAL;
		operacion->parametros = NULL;

	}else if(_esRUN(linea)){
		operacion->nombre_operacion = API_RUN;
		operacion->parametros = string_split(linea + strlen("RUN "), " ");

	}else if(_esADD(linea)){
		//ADD MEMORY [NÚMERO] TO [CRITERIO]
		operacion->nombre_operacion = API_ADD;
		operacion->parametros = malloc(3 * sizeof(char *));

		//retorna ["[NÚMERO]", "TO", "[CRITERIO]", NULL]
		char **aux = string_split(linea + strlen("ADD MEMORY "), " ");

		//borramos "TO"
		(operacion->parametros)[0]= strdup(aux[0]);
		(operacion->parametros)[1]= strdup(aux[2]);
		(operacion->parametros)[2]= NULL;

		free_list_of_strings(aux);

	}else if(_esMETRICS(linea)){
		operacion->nombre_operacion = API_METRICS;
		operacion->parametros = NULL;
	}

	return operacion;
}



static bool _esSELECT(char* linea){
	//SELECT [NOMBRE_TABLA] [KEY]
	return string_starts_with(linea, "SELECT ");
}

static bool _esINSERT(char* linea){
	//INSERT [NOMBRE_TABLA] [KEY] “[VALUE]” [Timestamp]
	//Ejemplos:
	//INSERT TABLA1 3 “Mi nombre es Lissandra” 1548421507
	return string_starts_with(linea, "INSERT ");
}

static bool _esCREATE(char* linea){
	//CREATE [TABLA] [TIPO_CONSISTENCIA] [NUMERO_PARTICIONES] [COMPACTION_TIME]
	return string_starts_with(linea, "CREATE ");
}

static bool _esDESCRIBE(char* linea){
	//DESCRIBE [NOMBRE_TABLA]
	return string_starts_with(linea, "DESCRIBE");
}

static bool _esDROP(char* linea){
	//DROP [NOMBRE_TABLA]
	return string_starts_with(linea, "DROP ");
}

static bool _esJOURNAL(char* linea){
	//JOURNAL
	return string_starts_with(linea, "JOURNAL");
}

static bool _esRUN(char* linea){
	//RUN <path>
	return string_starts_with(linea, "RUN ");
}

static bool _esADD(char* linea){
	//ADD MEMORY [NUMERO] TO [CRITERIO]
	return string_starts_with(linea, "ADD MEMORY ");
}

static bool _esMETRICS(char* linea){
	//METRICS
	return string_starts_with(linea, "METRICS");
}

void free_list_of_strings(char** list_of_strings) {
	string_iterate_lines(list_of_strings, (void*) free);
	free(list_of_strings);
}

void free_operacion(struct_operacion* operacion){
	if((operacion->parametros) != NULL)
		free_list_of_strings(operacion->parametros);
	free(operacion);
}

void* estructura_registro (struct_operacion* operacion,int tipo_operacion){
	switch (tipo_operacion) {
		case SELECT:{
			struct_select *select = malloc(sizeof(struct_select));
			select->key=strtol((operacion->parametros)[1],NULL,16);
			select->nombreTabla=strdup((operacion->parametros)[0]);
			return select;
		}
		case INSERT:{
			struct_insert *insert = malloc(sizeof(struct_insert));
			insert->nombreTabla=strdup((operacion->parametros)[0]);
			insert->key=strtol((operacion->parametros)[1],NULL,16);
			insert->valor=strdup((operacion->parametros)[2]);
			insert->timestats= atoi((operacion->parametros)[3]);
			return insert;

		}
		case CREATE:{
			struct_create *create = malloc(sizeof(struct_create));
			create->nombreTabla=strdup((operacion->parametros)[0]);
			create->tipo= atoi((operacion->parametros)[1]);
			create->numeroParticiones=atoi((operacion->parametros)[2]);
			create->tiempoCompactacion=atoi((operacion->parametros)[3]);
			return create;
		}

		default:
			return NULL;


	}
}


int convertinAtipoConsistencia(char *tipoConsistencia){
	if(!strncmp(tipoConsistencia, "SHC", 3)){
		return SHC;
	}
	else if(!strncmp(tipoConsistencia, "EC", 3)){
		return EC;
	}
	else if(!strncmp(tipoConsistencia, "SC", 3)){
		return SC;
	}
	else {
		return -1;
	}
}


