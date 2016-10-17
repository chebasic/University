#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "auxiliar.h"

int main(int argc, char* argv[])
{
	argv0 = "primos";
	char* trash;
	char* envMx = getenv("MAX_OUTPUT");
	long int maxOut;
        // Inicialización de la variable maxOut.
        if(strlen(envMx)!=0){
		maxOut = strtol(getenv("MAX_OUTPUT"), &trash,0);
		if(maxOut<0 || strlen(trash)!=0){
			maxOut = 100;
		}
	}
	else
		maxOut = 100;
		
	long int desde, hasta, i;
	int mx=0;



	switch(argc){

		case 1 : 
			desde = 1;
			hasta = 100;
			for(i = desde; i <= hasta; i++){
				if(mx<maxOut){	
					if(esPrimo(i)){
						fprintf(stdout, "\t%ld\n", i);
						mx++;
					}
				}
				else{
					Error(EX_NOPERM, "Se intentó superar el limite de salida establecido por MAX_OUTPUT.");
				}
			}
			break;
		case 2 :
			helpCall(argc,argv,1);
			desde = 1;
			hasta = strtol(argv[1], &trash, 0);

			if(strlen(trash)!=0  || hasta <= 0){
				Error(EX_USAGE, "El parámetro \"hasta\" no es un número positivo. ");
			}
			for(i = desde; i<=hasta; i++){
				if(mx<maxOut){
					if( esPrimo(i)){
						fprintf(stdout,"\t%ld\n", i);
						mx++;
					}
				}
				else{
					Error(EX_NOPERM, "Se intentó superar el límite de salida establecido por MAX_OUTPUT.");
				}
			}
			break;
		case 3 :
			desde = strtol(argv[2], &trash, 0);
			if(strlen(trash)!=0 || desde <= 0){
				Error(EX_USAGE, "El parámetro \"desde\" no es un número positivo.");
			}			
			hasta = strtol(argv[1], &trash, 0);
			if(strlen(trash)!=0 || hasta <= 0){
				Error(EX_USAGE, "El parámetro \"hasta\" no es un número positivo.");
			}
			
			if(desde > hasta){
				Error(EX_USAGE, "El parámetro \"desde\" ha de ser menor o igual que \"hasta\".");
			}
			for(i = desde; i <= hasta; i++){
				if(mx < maxOut){
					if(esPrimo(i)){
						fprintf(stdout,"\t%ld\n",i);
						mx++;
					}
				}
				else{
					Error(EX_NOPERM, "Se intentó superar el límite de salida establecido porMAX_OUTPUT.");
				}
			}
			break;

		default:
			 Error(EX_USAGE, "Uso incorrecto del mandato");
			 break;
	}	



	return 0;
}
