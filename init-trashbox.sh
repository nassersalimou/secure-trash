#!/bin/bash

dossier=".sh-trashbox"

if [ ! -d "$dossier" ];then
 mkdir "$dossier"
 echo "le dossier $dossier n'existait pas , mais il a été crée"
 elif [ -d "$dossier" ];then
 echo "le dossier existait déjà"
fi
 echo "1" > "$dossier/ID"
 touch "$dossier/INDEX"
