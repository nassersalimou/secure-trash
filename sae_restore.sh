#!/bin/bash

working_directory=sae_broyeur
if ! pwd | grep "$working_directory" > /dev/null 2>&1 ; then
  echo "Vous n'êtes pas dans le dossier $working_directory !!"
  exit 1
fi

# Initialisation des répertoires et fichiers nécessaires
trash_dir=".sh-trashbox"
trash_file=".sh-trashbox/INDEX"
output_default="./"

if [ "$#" -lt 1 ]; then
  echo "Erreur de paramétrage"
  exit 1
fi

if [ ! -d "$trash_dir" ]; then
  echo "Le dossier $trash_dir est inexistant"
  exit 1
fi

if [ ! -f "$trash_file" ]; then
  echo "Le fichier $trash_file est inexistant"
  exit 1
fi

restore_to_origin=false
output_dir="$output_default"

if [ "$1" == "-r" ]; then
  restore_to_origin=true
  shift
fi

if [ "$1" == "-d" ]; then
  output_dir="$2"
  shift 2
fi

mkdir -p "$output_dir"

for param in "$@"; do
  # Trouver toutes les entrées correspondant au fichier ou ID
  matching_entries=$(grep ":$param:" "$trash_file")

  if [  -z "$matching_entries" ]; then
    echo "Aucune entrée trouvée pour '$param' dans $trash_file"
    exit 1
  fi

  # Trier les résultats par date de suppression (champ 4) et prendre le plus récent
  recent_entry=$(echo "$matching_entries" | sort -t ":" -k4 -n | tail -n 1)

  # Extraire les champs de l'entrée sélectionnée
  recent_id=$(echo "$recent_entry" | cut -d ":" -f1)
  recent_dir=$(echo "$recent_entry" | cut -d ":" -f2)
  recent_filename=$(echo "$recent_entry" | cut -d ":" -f3)

  file_to_restore="$trash_dir/$recent_id"

  if [ -f "$file_to_restore" ]; then
    if [ "$restore_to_origin" = true ]; then
      mkdir -p "$recent_dir" #on recree le repertoire d'origine au cas où , il n'existe plus
      target_dir="$recent_dir"
    else
      target_dir="$output_dir" #dans le cas où , la restauration se fait dans un repertoire spécifié en paramètre.
    fi
  if [ -e "$target_dir/$recent_filename" ]; then
  echo "Le fichier $recent_filename existe déjà dans le répertoire cible : $target_dir"
  echo "Spécifiez un autre dossier ou renommez le fichier avant de relancer la commande."
  exit 1
fi
    mv "$file_to_restore" "$target_dir/$recent_filename" 

 
    sed -i "/^$recent_id:/d" "$trash_file"
    echo "Le fichier $file_to_restore a été restauré dans $target_dir/$recent_filename"
  else
    echo "Le fichier $file_to_restore est inexistant dans notre espace de travail"
  fi
done
