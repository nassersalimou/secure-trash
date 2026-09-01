#!/bin/bash

trash_dir=".sh-trashbox"
trash_file=".sh-trashbox/INDEX"

working_directory=tp-corbeille
  if ! pwd | grep $working_directory; then
      echo "Vous n'êtes pas dans le dossier $working_directory !!"
      exit 1
  fi

 if [ ! -d "$trash_dir" ];then
  echo "le repertoire $trash_dir n'existe pas"
  exit 1
fi

if [ ! -f "$trash_file" ];then
  echo "le fichier $trash_file n'existe pas "
  exit 1
fi

while IFS=":" read -r id dirname filename date_suppression ; do
 echo "$id:$dirname/$filename:$date_suppression"
  done < "$trash_file"
