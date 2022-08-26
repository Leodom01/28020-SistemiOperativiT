#!/bin/bash
if [[ $# -ne 3 ]] ; then echo Sintassi!; exit -1 ; fi
if [[ ! "$1" = /* ]] ; then echo Il path deve essere assoluto!; exit -1; fi
if [[ ! -d "$1" ]] ; then echo Devi dare un path!; exit -1; fi
if [[ $2 -lt 0 ]] ; then echo N deve essere positivo!; exit -1; fi
if [[ -f "$3" ]] ; then echo Deve essere una directory l\'ultimo campo!; exit -1; fi

touch $HOME/fileout.txt
> $HOME/fileout.txt

if [[ -d "$3" ]] ; then
  rm -r "$3"
  mkdir "$3"
else
  mkdir "$3"
fi

#Capisco come chiamare il recursive
if [[ "$0" = /* ]] ; then
  #allora $0 è assoluto
  dir_name=`dirname "$0"`
  recursive_cmd="$dir_name/esame_rec.sh"
elif [[ "$0" = */* ]] ; then
  #Se non inizia con / ma / è nel mezzo allora è un path relativo
  dir_name=`dirname "$0"`
  recursive_cmd="`pwd`/$dir_name/esame_rec.sh"
else
  #Se non è ne'realtivo ne'assoluto
  recursive_cmd="esame_rec.sh"
fi

"$recursive_cmd" "$1" "$3" "$2"
