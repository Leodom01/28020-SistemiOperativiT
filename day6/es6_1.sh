#!/bin/bash
if [ $# -ne 1 ] ; then echo Sintassi! ; exit; fi
echo Inserire il path assoluto di un file esistente e leggibile:
read F
if [[ ! -f $F ]] ; then echo Il file non esiste!; exit; fi
if [[ ! -r $F ]] ; then echo Non ho diritti di lettura!; exit; fi
grep $1 $F | sort -r > $HOME/results_`whoami`.out
