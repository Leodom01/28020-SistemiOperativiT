#!/bin/bash

if ! test -d "$3" ; then
  #Se è un file
  if [[ `stat --format=%U "$3"` == `whoami`]] ; then
    tail -n "$2" "$3"
else
  #Se è una directory
  cd "$3"
  for f in * ; do
    "$0" "$1" "$2" "$f"
  done
fi
