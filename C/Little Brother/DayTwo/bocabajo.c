#include <string.h>
#include <sysexits.h>
#include <stdlib.h>
#include <stdio.h>
#include "auxiliar.h"


#define BUFFER 2048

int cnt_lineas(char* fich);


int main(int argc, char* argv[]){
	
	argv0 = "bocabajo";
	

	if( argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1],"--help")) ){
		fprintf(stdout,"bocabajo: Uso: bocabajo [ fichero...]\n"
		       	       "bocabajo: Invierte el orden de las lineas de los ficheros (o de la entrada).\n");
		exit(EX_OK);
	}
	
	if (argc>1){
		int i;
		for( i = argc -1; i>0; i--){
			FILE *fd_aux;
			fd_aux = fopen(argv[i], "r");
			if(fd_aux == NULL){
				Error(EX_NOINPUT,"El fichero \"%s\" no pude ser leido", argv[i]);
			}
		}	

	
		int j = 0;
		int k = 0;

		for (i = argc-1 ; i>0 ;i--){
			
		      	int n_lineas = cnt_lineas(argv[i]);
			
		      	char **array = (char **)malloc(n_lineas*sizeof(char *)); 
		      	if(array == NULL){
				Error(EX_OSERR, "No se pudo ubicar la memoria dinámica necesaria");
			}	

		      FILE *fd;
		     	fd = fopen(argv[i],"r");
		        			
	                char fd_read[BUFFER];
				
	                while(feof(fd)==0 ){

				fgets(fd_read,BUFFER,fd);
				if (feof(fd)==0){
					array[j]=strdup(fd_read);
				
					j++;
					
				}
		       	}

		              fclose(fd);
		       
			for(k = n_lineas-1; k >= 0; k--){
				fprintf(stdout, "%s", array[k]);
			}
				
				
			
			for(k = 0; k < n_lineas; k++){
				free(array[k]);
			}
			
			free(array);
			j=0;
		
		}//END For
	}//END if(argc>1)
	
	else{
		
		char array[BUFFER];
		int lin = 0 ;
		int j = 0;
		int k = 0;
		char **lineas_plus=NULL;
		
		
		char **lineas=(char **) malloc( sizeof(char *) );
		if(lineas == NULL){
			Error(EX_OSERR, "No se pudo ubicar la memoria dinamica necesaria.");	
		}

		
		while(feof(stdin) == 0 ){
		
			fgets(array,BUFFER,stdin);
			if(feof(stdin) == 0){
				lin++;
				lineas_plus = realloc(lineas, lin*sizeof(char *));
				if(lineas_plus == NULL){
					Error(EX_OSERR, "No se pudo ubicar la memoria dinámica necesaria.");
				}
				lineas = lineas_plus;
				lineas[j]=strdup(array);
				j++;
			}
			
		}
	
		for(k = lin-1 ; k >=0 ; k--){
			printf("%s",lineas[k]);
		}
		
		for(k = 0; k < lin; k++){
			free(lineas[k]);
		}
		//j=0;	
		free(lineas);
	}
	
exit(EX_OK);
}

int cnt_lineas(char* fich){

	FILE *fd;
	int contador=0;
	fd = fopen(fich,"r");	
        if(fd != NULL){
		char fd_read[BUFFER];
		while(feof(fd)==0){
			fgets(fd_read,BUFFER,fd);
			if (feof(fd)==0){
				
				contador++;
			}
	       	}
		rewind(fd);
		fclose(fd);
	}
	return contador;
}
