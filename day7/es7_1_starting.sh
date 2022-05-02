#!/bin/bash

if [[ "$0" = /* ]] ; then
  #allora $0 è assoluto
  dir_name=`dirname "$0"`
  recursive_cmd="$dir_name/es7_1_recursion.sh"
elif [[ "$0" = */* ]] ; then
  #Se non inizia con / ma / è nel mezzo allora è un path relativo
  dir_name=`dirname "$0"`
  recursive_cmd="`pwd`/$dir_name/es7_1_recursion.sh"
else
  #Se non è ne'realtivo ne'assoluto
  recursive_cmd="es7_1_recursion.sh"
fi

"$recursive_cmd" "$1"
