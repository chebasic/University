#!/bin/bash

########################################################################
# Este fichero pretende ser un ejemplo para que los estudiantes
# elaboren nuevos tests para sus programas.

########################################################################
# Funciones auxiliares

# ejecuta un mandato, retorna el exit status del mandato previamente
# almacenado en la variable de entorno xtatus y almacena la salida
# estándar en el fichero .fd1 y la salida de error en el fichero .fd2
test_run() # Command [args...] [| Command...] [<2>> redirections...]
{
	echo "$@"
        [ -x $1 ] || (echo "No existe el ejecutable $1"; exit 1)
	"$@" > .fd1 2> .fd2
        xtatus=$?
        return $xtatus
}

# Comprueba que xtatus tiene como valor el indicado por el primer
# parámetro
test_status()
{
    if [ $xtatus -eq $1 ]; then
        echo "Valor de terminacion correcto"
    else
        echo "Valor de terminacion incorrecto"
        echo "Se esperaba $1 y pero el valor es $xtatus"
        exit 1
    fi;
    return 0
}

# Comprueba que el número de líneas de la salida estándar o de error
# son los adecudados
test_lines()
{
    typeset -i nlines=`cat .fd$1 | wc -l`
    [ $nlines $2 $3 ] && return 0

    echo "Error en el numero de lineas del descriptor $1"
    echo "Se esperaba $nlines $2 $3"
    exit 1
}

# Comprueba que el contenido de la salida est�ndar o de la salida de
# error son adecuados
test_content()
{
    if sdiff .fd$1 ${2:--}; then
        echo "Contenido correcto"
    else
        echo "El contenido del descriptor $1 NO es el esperado"
        exit 1
    fi;
    return 0
}

########################################################################
# Prueba bocabajo con -h
test_run ./bocabajo -h
test_status 0
test_lines 2 -eq 0
test_lines 1 -ge 2
test_content 1 << \EOF
bocabajo: Uso: bocabajo [ fichero...]
bocabajo: Invierte el orden de las lineas de los ficheros (o de la entrada).
EOF

########################################################################
# Prueba bocabajo con la entrada diez:
cat > diez.txt << EOF
uno
dos
tres
cuatro
cinco
seis
siete
ocho
nueve
diez
EOF
test_run ./bocabajo diez.txt
test_status 0
test_lines 2 -eq 0
test_content 1 << \EOF
diez
nueve
ocho
siete
seis
cinco
cuatro
tres
dos
uno
EOF


########################################################################
#	Prueba bocabajo con Fichero que no existe.
########################################################################

test_run ./bocabajo fich_null
test_status 66
test_lines 2 -eq 2
test_content 2 << \EOF
bocabajo: Error(EX_NOINPUT), no se puede abrir la entrada. "No such file or directory"
bocabajo+ El fichero "fich_null" no pude ser leido
EOF


read -p "Borrar ficheros auxiliares? [y/N] :" y_n

case "$y_n" in
  y|Y) echo "Borrados"
	rm .fd1 .fd2 diez.txt;;
  n|N) echo "almacenados"
	if [ ! -d "/auxFiles" ]
	then
		mkdir auxFiles
	fi		
	mv .fd1 .fd2 diez.txt ./auxFiles;;
    *) echo "invalid";;
esac 
