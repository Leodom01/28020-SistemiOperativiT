#!/bin/bash

cd "$1"

echo "`ls | sort | head -$3 `" >> $HOME/fileout.txt

for i in *; do
  if [[ -d "$i" ]]; then "$0" "$1"/"$i" "$2" "$3"; fi
  if [[ -f "$i" ]]; then
    cp "$i" "$2"/"$i"
  fi
done
