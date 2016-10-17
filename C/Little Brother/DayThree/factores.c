#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "auxiliar.h"

#define true  1
#define false 0
#define BUFFER 80

int main(int argc, char* argv[])
{
	
	argv0 = "factores";
	long int * factores = malloc(sizeof(long int));
 	char * trash;
	helpCall(argc, argv,2);
	long int N,i;
	if(factores==NULL){
		Error(EX_OSERR, "No se pudo ubicar la memoria dinámica necesaria.");
	}
	if(argc>=2){
		Error(EX_USAGE, "Numero de argumentos erroneos");
	}
	

	char fd_read[BUFFER];
	while(feof(stdin)==0){
		fscanf(stdin,"%80s",fd_read);
		if(feof(stdin)!=0){
			return 0;
		}
		
	
		for(i=0;i<strlen(fd_read);i++){
			if(fd_read[i] == '\n'){
				fd_read[i]='\0';
			}
		}
		N = strtol(fd_read, &trash,0);
		if(strlen(trash)!=0 || N < 0){
		
			fprintf(stderr,"%s: \"%s\" no es un entero no negativo.\n",argv0,fd_read );
		}
		else{
			factorizar(N,factores);
		}		
		
	}
	
	free(factores);

	return 0;
}
