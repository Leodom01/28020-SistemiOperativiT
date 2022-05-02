#!/bin/bash

#Controllo dei parametri
if ! [[ "$#" == "3" ]] ; then
  echo Sintassi!
  exit
elif test $2 -lt 0 ; then
  echo R deve essere positivo!
  exit
elif ! test -d $3 ; then
  echo dir deve essere una directory esistente!
  exit
fi

#Genero stringa per chiamare file ricorsivo
if [[ "$0" = /* ]] ; then
  #allora $0 è assoluto
  dir_name=`dirname "$0"`
  recursive_cmd="$dir_name/es7_2_recursive.sh"
elif [[ "$0" = */* ]] ; then
  #Se non inizia con / ma / è nel mezzo allora è un path relativo
  dir_name=`dirname "$0"`
  recursive_cmd="`pwd`/$dir_name/es7_2_recursive.sh"
else
  #Se non è ne'realtivo ne'assoluto
  recursive_cmd="es7_2_recursive.sh"
fi

"$recursive_cmd" "$1" "$2" "$3"
