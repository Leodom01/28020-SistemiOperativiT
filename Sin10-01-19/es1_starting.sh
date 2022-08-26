#!/bin/bash

if [ $# -ne 3 ] ; then echo Sintassi! ; exit; fi
if [[ ! -d $1 ]]; then echo Non è una directory esistente! ; exit; fi
if [[ $3 = *[!0-9]* ]]; then echo N deve essere positivo! ; exit; fi

#Creo file di output
touch esito.out
> esito.out
output_file="`pwd`/esito.out"

#Capisco come chiamare il recursive
if [[ "$0" = /* ]] ; then
  #allora $0 è assoluto
  dir_name=`dirname "$0"`
  recursive_cmd="$dir_name/es1_recursive.sh"
elif [[ "$0" = */* ]] ; then
  #Se non inizia con / ma / è nel mezzo allora è un path relativo
  dir_name=`dirname "$0"`
  recursive_cmd="`pwd`/$dir_name/es1_recursive.sh"
else
  #Se non è ne'realtivo ne'assoluto
  recursive_cmd="es1_recursive.sh"
fi

for sub_dir in `ls -d */`; do
  echo "Chiamo per $sub_dir questo: $recursive_cmd $sub_dir start N"
  "$recursive_cmd" "$sub_dir" "$2" "$3" "$output_file"
done
