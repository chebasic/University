#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include "message.h"

void Escribir_Puerto (int puerto);

#define BUFFER_SIZE 4076

/* FUNCION MAIN DEL PROGRAMA SERVIDOR */
int main(int argc,char* argv[])
{
    /* El esqueleto de la funcion principal muestra, de forma esquematica la secuencia 
       de operaciones y la correspondiente traza que se sugiere */

  int s_UDP, s_TCP, p_UDP, p_TCP, s_AUX, rec_UDP,file;
  char * status;
  struct sockaddr_in dir_server_UDP;
  struct sockaddr_in dir_server_TCP;
  struct sockaddr_in dir_client; // Socket de comunicación con el cliente.
  UDP_Msg msg;
  socklen_t size;
  char buffer [BUFFER_SIZE];
   
  /* Creacion del socket UDP */
  s_UDP = socket(AF_INET,SOCK_DGRAM, 0);
  status = ( s_UDP>=0 ) ?  "OK" : "ERROR";
  fprintf(stdout, "SERVIDOR: Creacion del socket UDP: %s\n", status);
  if(s_UDP<0)
    exit(1);  // Si no se ha podido crear el socket UDP se finaliza.

  /* Asignacion de la direccion local (del servidor) Puerto UDP*/
// Inicilizado de la Estructura
  bzero((char *)&dir_server_UDP, sizeof(struct sockaddr_in));

  dir_server_UDP.sin_family = AF_INET;
  dir_server_UDP.sin_addr.s_addr = inet_addr(HOST_SERVIDOR);
  dir_server_UDP.sin_port = htons(0);
  
  p_UDP = bind(s_UDP,(struct sockaddr *) &dir_server_UDP,sizeof(struct sockaddr));
  status = (p_UDP>=0) ? "OK" : "ERROR";
  fprintf(stdout,"SERVIDOR: Asignacion del puerto servidor: %s\n", status);
  
  if(p_UDP<0){
    
    close(s_UDP);
    exit(1);
  }
  /* Escribimos el puerto de servicio */
  size = sizeof(dir_server_UDP);
  
  bzero((char *)&dir_server_UDP,sizeof(struct sockaddr_in));
  getsockname(s_UDP, (struct sockaddr *)&dir_server_UDP, &size);
  p_UDP =ntohs((int) dir_server_UDP.sin_port);

  Escribir_Puerto(p_UDP /* Numero de puerto*/);

  /* Creacion del socket TCP de servicio */
  s_TCP = socket(AF_INET, SOCK_STREAM, 0);

  status = (s_TCP>=0) ? "OK" : "ERROR";
  
  //  fprintf(stdout,"SERVIDOR: Creacion del socket TCP: OK|ERROR\n");
  fprintf(stdout,"SERVIDOR: Creacion del socket TCP: %s\n",status);
  if(s_TCP<0)
    exit(1);
  
  /* Asignacion de la direccion local (del servidor) Puerto TCP*/
  bzero((char *)&dir_server_TCP, sizeof(struct sockaddr_in));
  dir_server_TCP.sin_family = AF_INET;
  dir_server_TCP.sin_addr.s_addr = inet_addr(HOST_SERVIDOR);
  dir_server_TCP.sin_port = htons(0);

  p_TCP = bind(s_TCP, (struct sockaddr *)&dir_server_TCP,sizeof(struct sockaddr_in));

  status = (p_TCP>=0) ?  "OK" : "ERROR";

  fprintf(stdout,"SERVIDOR: Asignacion del puerto servidor: %s\n",status);
  
  if(p_TCP<0){
    close(s_TCP);
    exit(1);
  }
 
  bzero((char *)&dir_server_TCP, sizeof(struct sockaddr_in));

  size= sizeof(dir_server_TCP);
  getsockname(s_TCP, (struct sockaddr *) &dir_server_TCP, &size);
  p_TCP = ntohs((int) dir_server_TCP.sin_port);

  
  /* Aceptamos conexiones por el socket */


  // Preparamos structuras del cliente

  bzero((char *)&dir_client, sizeof(struct sockaddr_in));
  bzero(&msg, sizeof(UDP_Msg));

  size = sizeof(dir_client);

  status = ((listen(s_TCP, 0))>=0) ?  "OK" : "ERROR";
  fprintf(stdout,"SERVIDOR: Aceptacion de peticiones: %s\n",status);


  /* Puerto TCP ya disponible */

  fprintf(stdout,"SERVIDOR: Puerto TCP reservado: OK\n");
  socklen_t size_cli = sizeof(dir_client);
  while(1 /* Bucle de procesar peticiones */)
    {
      fprintf(stdout,"SERVIDOR: Esperando mensaje.\n");

      /* Recibo Mensaje */
      rec_UDP = recvfrom(s_UDP, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&dir_client,  &size_cli);
 
      status = ( rec_UDP>=0  ) ? "OK" : "ERROR";
      fprintf(stdout,"SERVIDOR: Mensaje del cliente: %s\n",status);

      if(ntohl(msg.op) == QUIT/* Mensaje QUIT*/){
          msg.op = htonl(OK);    
	  (sendto(s_UDP,(char *)&msg, sizeof(msg),0,(struct sockaddr *)&dir_client, size_cli)>0) ? "OK": "ERROR";
	  fprintf(stdout,"SERVIDOR: QUIT\n");
	  
	  fprintf(stdout,"SERVIDOR: Enviando del resultado [%s]: %s\n", (msg.op >0) ? "OK":"ERROR",status);
          close(s_UDP);
          close(s_TCP);
          break;
      }
      else
      {
          file = open(msg.remoto, O_RDONLY);
	  fprintf(stdout,"SERVIDOR: REQUEST(%s,%s)\n", msg.local, msg.remoto);
          if(file<0){
            if(errno==ENOENT)
                msg.op = htonl(ERROR);
          }
          else msg.op = htonl(OK);
	  msg.puerto = htons(p_TCP);
	  sendto(s_UDP, (char *) &msg, sizeof(msg),0,(struct sockaddr *)&dir_client,size_cli);
	  
	  if (msg.op == ERROR)
	    continue;

	  /* Envio del resultado */
	  
	  s_AUX = accept(s_TCP, (struct sockaddr *)&dir_client, &size_cli);
	  
	  status = ( s_AUX > 0) ?  "OK" : "ERROR";
		  
	  fprintf(stdout,"SERVIDOR: Enviando del resultado [%s]: %s\n",(msg.op >0) ? "OK" : "ERROR", status);
	  

	  while( write(s_AUX, buffer,  read(file,buffer,BUFFER_SIZE))>0 )
	    continue;
	  close(s_AUX);
	  close(file);
	  bzero((char *)&dir_client,sizeof(struct sockaddr_in));
	  size_cli = sizeof(dir_client);
	  bzero(&msg, sizeof(UDP_Msg));
	  
	  /* Esperamos la llegada de una conexion */
	  fprintf(stdout,"SERVIDOR: Llegada de un mensaje: %s\n", "OK");
      }
    }
  
  fprintf(stdout,"SERVIDOR: Finalizado\n");
  exit(0);
}

/* Funcion auxiliar que escribe un numero de puerto en el fichero */
void Escribir_Puerto (int puerto)
    {
    int fd;
    if((fd=creat(FICHERO_PUERTO,0660))>=0)
    {
	write(fd,&puerto,sizeof(int));
	close(fd);
	fprintf(stdout,"SERVIDOR: Puerto guardado en fichero %s: OK\n",FICHERO_PUERTO);
    }
}



