#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>
#include "filtrar.h"


/* ---------------- PROTOTIPOS ----------------- */ 
/* Esta funcion monta el filtro indicado y busca el simbolo "tratar"
   que debe contener, y aplica dicha funcion "tratar()" para filtrar
   toda la informacion que le llega por su entrada estandar antes
   de enviarla hacia su salida estandar. */
extern void filtrar_con_filtro(char* nombre_filtro);

/* Esta funcion lanza todos los procesos necesarios para ejecutar los filtros.
   Dichos procesos tendran que tener redirigida su entrada y su salida. */
void preparar_filtros(void);

/* Esta funcion recorrera el directorio pasado como argumento y por cada entrada
   que no sea un directorio o cuyo nombre comience por un punto '.' la lee y 
   la escribe por la salida estandar (que seria redirigida al primero de los 
   filtros, si existe). */
void recorrer_directorio(char* nombre_dir);

/* Esta funcion recorre los procesos arrancados para eje1cutar los filtros, 
   esperando a su terminacion y recogiendo su estado de terminacion. */ 
void esperar_terminacion(void);

/* Desarrolle una funcion que permita controlar la temporizacion de la ejecucion
   de los filtros. */ 
extern void preparar_alarma(void);
void infanticidio(int sig);         // Matará a los hijos que no han terminado.

/* ---------------- IMPLEMENTACIONES ----------------- */ 
char** filtros;   /* Lista de nombres de los filtros a aplicar */
int    n_filtros; /* Tama~no de dicha lista */
pid_t* pids;      /* Lista de los PIDs de los procesos que ejecutan los filtros */



/* Funcion principal */
int main(int argc, char* argv[])
{
	/* Chequeo de argumentos */
	if(argc<2) 
	{
	  fprintf(stderr,"Uso: %s directorio [filtro...]\n",argv[0]);
	  exit(1);
	}

	filtros = &(argv[2]);                             /* Lista de filtros a aplicar */
	n_filtros = argc-2;                               /* Numero de filtros a usar */
	pids = (pid_t*)malloc(sizeof(pid_t)*n_filtros);   /* Lista de pids */

	preparar_alarma();
	preparar_filtros();
	recorrer_directorio(argv[1]);
	esperar_terminacion();
	
	return 0;
}


void recorrer_directorio(char* nombre_dir)
{
	DIR* dir ;
	struct dirent* ent;
	struct stat  f_stat;
	char fich[1024];
	char buff[4096];
	int fd;
	/* Abrir el directorio */
	dir = opendir(nombre_dir);
	/* Tratamiento del error. */
        if( dir == NULL){
	  fprintf(stderr,"Error al abrir el directorio '%s'\n",nombre_dir);
	  exit(1);
	}
	

	/* Recorremos las entradas del directorio */
	while((ent=readdir(dir))!=NULL)
	{
		/* Nos saltamos las que comienzan por un punto "." */
		if(ent->d_name[0]=='.')
			continue;

		/* fich debe contener la ruta completa al fichero */
		// sprintf, nos mermite escribier en un 'string', como salida.
		sprintf(fich,"%s/%s",nombre_dir, ent->d_name);
		
		/* Nos saltamos las rutas que sean directorios. */
		if(lstat(fich,&f_stat) < 0 )
		  {
		    fprintf(stderr,"AVISO: No se puede stat el fichero '%s'!\n",fich);
		    exit(1);
		  }
		if(S_ISDIR(f_stat.st_mode))
		  continue;
		if(S_ISLNK(f_stat.st_mode))
		  {
		    fprintf(stderr,"AVISO: No se puede stat el fichero '%s'!\n",fich);
		    continue;
		  }
		  
		/* Abrir el archivo. */
                fd=open(fich, O_RDONLY);
		/* Tratamiento del error. */
                 if( fd<0 ){
		   fprintf(stderr,"AVISO: No se puede abrir el fichero '%s'!\n",fich);
		   continue;
		}

		/* Cuidado con escribir en un pipe sin lectores! */


		/* Emitimos el contenido del archivo por la salida estandar. */
		 int err;
		 while( (err = write(1,buff,read(fd,buff,4096))) > 0)
			continue;
		 if(err<0)
		   {
		     fprintf(stderr,"Error al emitir el fichero '%s'\n",fich);
		     exit(1);
		   }
		/* Cerrar. */
		close(fd);
	}
	/* Cerrar. */
	closedir(dir);
	close(1);

	/* IMPORTANTE:
	 * Para que los lectores del pipe puedan terminar
	 * no deben quedar escritores al otro extremo. */
	// IMPORTANTE
}


void preparar_filtros(void)
{
	int p;
	int fd[2];
	int filtro_len;
	char * filtro_name = "";
	char * extension;
	for (p=n_filtros-1; p >= 0 ; p--)
	{
		/* Tuberia hacia el hijo (que es el proceso que filtra). */
	  if(pipe(fd)<0)
	    {
	      fprintf(stderr,"Error al crear el pipe\n");
	      exit(1);
	    }
		/* Lanzar nuevo proceso */
	  pids[p]=fork();
	    
		switch(pids[p])
		{
		case -1:
			/* Error. Mostrar y terminar. */
		  fprintf(stderr,"Error al crear proceso %d\n",p);
		  exit(1);

		case  0:

			/* Hijo: Redireccion y Ejecuta el filtro. */
		  dup2(fd[0],0);
		  close(fd[0]);
		  close(fd[1]);

		  /* Obtencion de la extension del fichero */
		  filtro_name=filtros[p];
		  filtro_len = strlen(filtro_name);
		  extension  = &(filtro_name[filtro_len-3]);
		  
		  if ( !strcmp(extension, ".so") )	/* El nombre termina en ".so" ? */
		    {	/* SI. Montar biblioteca y utilizar filtro. */
		      filtrar_con_filtro(filtros[p]);
		      exit(0);
		    }
		  else
		    {	/* NO. Ejecutar como mandato estandar. */
		      execlp(filtros[p], filtros[p],NULL );
		      fprintf(stderr,"Error al ejecutar el mandato '%s'\n",filtros[p]);
		      exit(1);
		    }
		  break;
		default:
			/* Padre: Redireccion */
		  dup2(fd[1],1);
		  close(fd[0]);
		  close(fd[1]);
			break;
		}
	}
}


void imprimir_estado(char* filtro, int status)
{
	/* Imprimimos el nombre del filtro y su estado de terminacion */
	if(WIFEXITED(status))
		fprintf(stderr,"%s: %d\n",filtro,WEXITSTATUS(status));
	else
		fprintf(stderr,"%s: senyal %d\n",filtro,WTERMSIG(status));

}


void esperar_terminacion(void)
{
  
    int p;
    int status;
    for(p=0;p<n_filtros;p++)
    {
	/* Espera al proceso pids[p] */
      
      if(waitpid(pids[p],&status,0)<0 )
      {
	fprintf(stderr,"Error al esperar proceso %d\n",pids[p]);
	exit(1);
      }
	/* MUESTRA SU ESTADO. */
      imprimir_estado(filtros[p],status);
    }
}


void infanticidio(int sig)
{
  fprintf(stderr,"AVISO: La alarma ha saltado!\n");

  /* CERRAMOS DESCRIPTOR UNO PARA QUE NO LOS LECTORES DEL PIPE PUEDAN TERMINAR */
  close(1);
  
  int p;

  for(p=0; p<n_filtros;p++)
    {
      if( !kill(pids[p],0) )// Check si existe el pid(filtro aplicandose);
	if(kill(pids[p],9)<0)
	  {
	    fprintf(stderr,"Error al intentar matar proceso %d\n",pids[p]);
	    exit(1);
	  }
    }
}



void preparar_alarma(void)
{
  // ¿Existe FILTRAR_TIMEOUT?
  // Si, ¿Es entero positivo?
  // Si, => SetAlarm(nSec)
  int n;    // n Segundos
  char * trash;
  char * envValue;// Valor variable Filtrar_Timeout
  if( (envValue=getenv("FILTRAR_TIMEOUT")) )
      {
	
	n=strtol(envValue,&trash,10);
	if(!strcmp(trash,"") && n>0)
	  {
	    struct sigaction newAction;

	    memset (&newAction, '\0', sizeof(newAction));
	    
	    newAction.sa_handler = &infanticidio;
	    newAction.sa_flags   = SA_RESTART;
	    sigaction(SIGALRM, &newAction, NULL);
	    alarm(n);
	    fprintf(stderr,"AVISO: La alarma vencera tras %i segundos!\n",n);
	  }else
	  {
	    fprintf(stderr,"Error FILTRAR_TIMEOUT no es entero positivo: '%s'\n",envValue);
	  }
      }
}

void filtrar_con_filtro(char* nombre_filtro){
  char * err;// Capturar Error
  void * dlo;// Apertura del filtro
  
  dlo=dlopen(nombre_filtro,RTLD_LAZY);

  err=dlerror();
   
  if( err != NULL )
    {
      fprintf(stderr,"Error al abrir la biblioteca '%s'\n",nombre_filtro);
      exit(1);
    }

  int (*tratar)(char *, char *, int);
  tratar = dlsym(dlo, "tratar");
  err=dlerror();
  if(err != NULL)
    {
      fprintf(stderr,"Error al buscar el simbolo '%s' en '%s'\n","tratar",nombre_filtro);
      exit(1);
    }
  char  bIN[4096]; //Buffer Entrada donde se almacena lo que se va a filtrar.
  char  bOUT[4096];//Buffer Salida donde se almacena el resultado del filtro.
  int b_leido=0;    //bytes Leidos durante el read.
  while(( b_leido=read(0,bIN,4096)) > 0 )
    {
      b_leido = (*tratar)(bIN,bOUT,b_leido);// Llamamos a la funcion tratar de la biblioteca.
      write(1,bOUT,b_leido);
    }
  
  dlclose(dlo);
}
