#!/bin/bash

if ! test -d "$1" ; then
  echo `pwd`/$1
else
  cd "$1"
  for f in * ; do
    "$0" "$f"
  done
fi
