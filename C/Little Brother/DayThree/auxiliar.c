/*
** auxiliar.c
*/

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "auxiliar.h"



char * argv0 = "<argv0>";

void Error(int exitval, char * fmt, ...)
{
	va_list ap;
	char buff[80]="¿?";
	char * name = buff;
	char * meaning;

	switch(exitval) {
	case EX_OK:		name="EX_OK";		meaning = "terminación correcta"; break;
	case EX_USAGE:		name="EX_USAGE";	meaning = "uso incorrecto del mandato"; break;
	case EX_DATAERR:	name="EX_DATAERR";	meaning = "datos con formato incorrecto"; break;
	case EX_NOINPUT:	name="EX_NOINPUT";	meaning = "no se puede abrir la entrada"; break;
	case EX_NOUSER:		name="EX_NOUSER";	meaning = "usuario desconocido"; break;
	case EX_NOHOST:		name="EX_NOHOST";	meaning = "nombre de Host desconocido"; break;
	case EX_UNAVAILABLE:	name="EX_UNAVAILABLE";	meaning = "servicio no disponible"; break;
	case EX_SOFTWARE:	name="EX_SOFTWARE";	meaning = "error interno del software"; break;
	case EX_OSERR:		name="EX_OSERR";	meaning = "error en el sistema"; break;
	case EX_OSFILE:		name="EX_OSFILE";	meaning = "ausencia de fichero crítico del sistema"; break;
	case EX_CANTCREAT:	name="EX_CANTCREAT";	meaning = "imposible crear fichero de salida de usuario"; break;
	case EX_IOERR:		name="EX_IOERR";	meaning = "error en entrada/salida"; break;
	case EX_TEMPFAIL:	name="EX_TEMPFAIL";	meaning = "fallo temporal, reintente"; break;
	case EX_PROTOCOL:	name="EX_PROTOCOL";	meaning = "error remoto en protocolo"; break;
	case EX_NOPERM:		name="EX_NOPERM";	meaning = "permiso denegado"; break;
	case EX_CONFIG:		name="EX_CONFIG";	meaning = "configuración errónea"; break;
	default:	sprintf(name, "%d", exitval);	meaning = "«sin significado establecido»";
	}

	fprintf(stderr, "%s: Error(%s), %s. \"%s\"\n", argv0, name, meaning, strerror(errno));

	fprintf(stderr, "%s+ ", argv0);
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");

	exit(exitval);
}
/*
 * helpCall()
 * 	Recive como parametro, el nº de argumentos, y el array que los contiene. 
 * */
void helpCall(int arg_n, char* arg_v[], int control)
{
	char * use_;
	char * use_meaning;
	switch(control){
		case 1 :  use_ = "[ hasta [ desde ]]"; use_meaning = "Genera los números primos comprendidos en el intervalo indicado"; break;
		case 2 :  use_ = ""; use_meaning = "Factoriza los números presentes en la entrada estandar"; break;
		default:  use_ = ""; use_meaning = "<<sin significado establecido>>";
	}
	
	if( (arg_n == 2) && ( !strcmp(arg_v[1],"-h") || !strcmp(arg_v[1],"--help")))
	{
		fprintf(stdout, "%s: Uso: %s %s\n"
				"%s: %s.\n", argv0,argv0, use_, argv0, use_meaning);
		exit(EX_OK);

	 }
}

bool esPrimo(long int N)
{
	bool esPrimo = 0; //0 si es primo, 1 si lo es.
	int i, rs, nc=0;
	
	for(i = 1; i<=N; i++){
		rs = N%i;
		if(rs == 0){
			nc++;
		}
		if(nc>2){ 
			break;
		}
	}
	if(nc==2){
		esPrimo = 1;
	}
	return esPrimo;
}

void factorizar(long int N, long int * factor)
{
	long int i = 2;
	int j=0;
	int nL=0;
	long int num,resto;
	num = N;
	long int * factor_plus = NULL;	
	if(esPrimo(N) ||
	   N== 1 || N== 0)
	{
		fprintf(stdout," %ld\n",N);
		return; 
	}

	while(i<N){
		resto = num%i;
		if(resto == 0 && esPrimo(i)){
			num=num/i;
			nL++;
			factor_plus = realloc(factor,nL*sizeof(long int));
			if(factor_plus==NULL){
				Error(EX_OSERR,"No se pudo ubicar la memoria dinamica necesaria");

			}
			factor=factor_plus;
			factor[j] = i ;
			j++;
			i--;
		}
		i++;
	}
		

	for(i=0; i < j; i++){
	 	fprintf(stdout,"\t%ld", factor[i]);
	}
	fprintf(stdout,"\n");
	return;
}

