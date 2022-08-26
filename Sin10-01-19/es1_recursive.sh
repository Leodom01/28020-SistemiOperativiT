#!/bin/bash
# calledFile sebDir toMatch numToGet outputfile

counter=0
cd "$1"
for i in * ; do
  if [[ -d $i ]] ; then
    "$0" "`pwd`/$i" "$2" "$3" "$4"
  fi
  if [[ -f "$i" && "$i" = $2* ]] ; then
    counter=`expr $counter + 1`
  fi
done

if [[ $counter -gt "$3" ]] ; then
  echo "`pwd` $counter" >> "$4"
fi
