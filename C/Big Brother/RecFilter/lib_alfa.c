#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "filtrar.h"

/* Este filtro deja pasar los caracteres NO alfabeticos. */
/* Devuelve el numero de caracteres que han pasado el filtro. */
int tratar(char* buff_in, char* buff_out, int tam)
{

  int o = 0; 
  int i;
  char c;
	      
  for(i=0; i<tam; i++){
    c = buff_in[i];
    if( c!='\0' && isalpha(c)>0 )
   	continue;
    else
	buff_out[o] = c;
	o++;
  }
  
  return o;
}
