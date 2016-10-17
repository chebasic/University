#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sysexits.h>
#define BUFFER 2049 


void invertir(char aInvertir[]){

        int  no_Char = strlen(aInvertir);
        char invertido[no_Char+1];
        invertido[no_Char] = '\0';
        int i;
        int j = no_Char-1;
        if(aInvertir[j]=='\n'){
                invertido[j]=aInvertir[j];
                j--;
                for(i=0;i<no_Char-1;i++){
			invertido[i]=aInvertir[j];
			j--;
		}
        }else{
        	for(i=0; i<no_Char;i++){
                	invertido[i]=aInvertir[j];
                	j--;
        	}
	}//END ELSE

        printf("%s", invertido);
}

int main( int argc, char* argv[] ){

        if( argc < 2 ){
                char buffer[BUFFER];
                while(feof(stdin)==0){
			fgets(buffer,BUFFER,stdin);
			if (feof(stdin)==0){
				invertir(buffer);
			}
		
                }

               exit(EX_OK);
        }else{

        int i;
        for(i=1;i < argc;i++){
                FILE *fd;
                fd = fopen(argv[i],"r");
                if(fd != NULL){
                        char fd_read[BUFFER];
                        while(feof(fd)==0){
				fgets(fd_read,BUFFER,fd);
				if (feof(fd)==0){
					invertir(fd_read);
				}
                	}
                        fclose(fd);
                }//END IF

                else{
				
						if(!strcmp( argv[1],"-h") || !strcmp(argv[1],"--help")){

							fprintf(stdout, "delreves: Uso: delreves [ fichero... ]\n"
                                "delreves: Invierte el contenido de las líneas de los ficheros (o de la entrada).\n");
							exit(EX_OK);
						}	
				
					fprintf(stderr,"delreves: Error(EX_NOINPUT), error en la ejecucion del programa. \"Success\"\n");
					fprintf(stderr,"delreves+ El fichero \"%s\" no puede ser leido.\n",argv[i]);
                       	exit(EX_NOINPUT);
                }
        }//END FOR
	}//END ELSE


	exit(EX_OK);
}//END main()


