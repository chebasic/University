#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "auxiliar.h"

void print_sec(double _hasta, double _desde, double _paso);

int main(int argc, char *argv[]){
	argv0 =  "secuencia";	
	double maxvalue = 100.0;

	
	if(argc>4){
		Error(EX_USAGE,"%s","Número erróneo de argumentos");
	}
	
	/* Comprobamos que existe la variable de entorno MAX_OUTPUT, que es igual 100.
		e inicializamos. */
	
	
	if(((getenv("MAX_OUTPUT")!=NULL) && (strtod(getenv("MAX_OUTPUT"),NULL)>=0)) && strtod(getenv("MAX_OUTPUT"),NULL)-(int)strtod(getenv("MAX_OUTPUT"),NULL)==0){
		maxvalue = strtod(getenv("MAX_OUTPUT"),NULL);
	
	}
	
	// Impresión por defecto desde 1 hasta 10.
	if(argc < 2){
		
		print_sec(10.0, 1.0, 1.0);
		return 0;
	}

	else{

		/*
			Comando de ayuda del programa, -h  y --help
		*/
		
		if ( !strcmp(argv[1],"--help" ) || !strcmp(argv[1],"-h")  ){
	
			fprintf(stdout,"secuencia: Uso: secuencia [hasta [desde [paso]]]\n"
				       "secuencia: Genera la secuencia en el intervalo y paso indicados.");
	
			return 0;	
		}
	
		
		if(argc == 4){
			char *errH, *errD, *errP;
			double hasta, desde, paso;

			hasta = strtod(argv[1], &errH);
			if(strlen(errH) != 0){
				Error(EX_USAGE, "%s", "El parametro Hasta no es valido.");
			}
		
			desde = strtod(argv[2], &errD);
			if(strlen(errD) != 0){
				Error(EX_USAGE, "%s", "El parametro Desde no es valido.");
			}

			paso = strtod(argv[3], &errP);
			if(strlen(errP) != 0){

				Error(EX_USAGE, "%s", "El parametro Paso no es valido.");
			}
			
			if(paso == 0.0){
				Error(EX_USAGE, "%s", "Uso incorrecto del mandato.");
			}
			
			/*
				Comprobamos si el orden para imprimir es correcto.desde 1, hasta 10 con un paso de -1 no es posible.
			*/
			
			if( ( (desde < hasta) && (paso < 0) ) || ( (desde > hasta) && (paso > 0) ) ){
			
				Error(EX_USAGE, "%s", "Uso Incorrecto del mandato.");
			}
			
			print_sec(hasta, desde, paso);
	
			return 0;
		}
	

		if(argc == 3){
			char *errH, *errD;
			double hasta, desde, paso;
			paso = 1;
		
			hasta = strtod(argv[1], &errH);
			if(strlen(errH) != 0){
				Error(EX_USAGE, "%s", "El parametro Hasta no es valido.");
			}
	
			desde = strtod(argv[2], &errD);
			if(strlen(errD) != 0){
				Error(EX_USAGE, "%s", "El parametro Desde no es valido.");
			}
			
			
			if( ( (desde < hasta) && (paso < 0) ) || ( (desde > hasta) && (paso > 0) ) ){
			
				Error(EX_USAGE, "%s", "Uso Incorrecto del mandato");
			}
			
			print_sec(hasta, desde, paso);
			
			return 0;
		}
	
		if(argc == 2){
			char *errH;
			double hasta, desde, paso;
			desde = 1;
			paso = 1;
		
			hasta = strtod(argv[1], &errH);
			if(strlen(errH) != 0){
				Error(EX_USAGE, "%s", "El parametro Hasta no es valido.\n");
			}
			
			if( ( (desde < hasta) && (paso < 0) ) || ( (desde > hasta) && (paso > 0) ) ){
			
				Error(EX_USAGE, "%s", "Uso Incorrecto del mandato");
			}
		
			print_sec(hasta, desde, paso);
		
			return 0;
		}
 	
	}

	/*
	 * argv[1] Fin intervalo
	 * argv[2] Principio intervalo
	 * argv[3] paso
	*/

return 0; 
	
	
}//END MAIN

void print_sec(double _hasta, double _desde, double _paso){
	
	double aux = _desde;
	double maxvalue = strtod(getenv("MAX_OUTPUT"), NULL);
	
	
	
	/*
		Evaluamos si la secuencia va a ser descendente o ascendente.
	*/
	
	if( (_desde<_hasta) && (_paso>0) ){
	
		while( aux <=  _hasta){
			fprintf(stdout, "\t%g\n", aux);
			
			if( ( aux == maxvalue ) && (aux != _hasta) ){
				Error(EX_NOPERM,"%s", "Se ha intentado exceder el límite.");
			}
			aux = aux + _paso;
			

		}
	
	}
	
	if( (_desde > _hasta) && (_paso<0) ){
	
		while( aux >= _hasta){
			fprintf(stdout, "\t%g\n", aux);
			
			if( ( aux == maxvalue ) && (aux != _hasta) ){
				Error(EX_NOPERM,"%s", "Se ha intentado exceder el límite.");
			}
			
			aux = aux + _paso;
		
		}
	
	
	}

	
	

}
