#!/bin/bash


working_directory=sae_broyeur
  if ! pwd | grep $working_directory > /dev/null 2>&1  ; then
      echo "Vous n'êtes pas dans le dossier $working_directory !!"
      exit 1
  fi

 trash_dir=".sh-trashbox"


   # Vérifier l'existence du dossier de corbeille
  if [ ! -d "$trash_dir" ]; then
    echo "Le dossier de corbeille $trash_dir n'existe pas. Veuillez exécuter init-trashbox.sh d'abord."
     exit 1
  fi


  # Vérifier qu'au moins un fichier est passé en paramètre
     if [ $# -lt 1 ];then
       echo "Erreur de parametrage"
            exit 1
    fi

  for fichier in "$@";do
   if [ -d "$fichier" ];then 
    for i in "$fichier"/*;do
     "$0" "$i" #appel recursif  pour  tous les fichiers ou dossier qu'il contient
    done
    continue       
  fi  

  if [ ! -f  "$fichier" ];then #verifier que le fichier a un contenu
   echo "le fichier n'est pas un fichier regulier ou n'existe pas dans notre espace de travail"
   exit 1
   fi


FILENAME=$(basename "$fichier")
DIRNAME=$(dirname "$fichier")

numero=$(cat "$trash_dir/ID")

 mv "$fichier" "$trash_dir/$numero"

annee=$(date +%Y)
mois=$(date +%m)
jour=$(date +%d)
heure=$(date +%H)
minutes=$(date +%M)
secondes=$(date +%S)

echo "$numero:$DIRNAME:$FILENAME:$annee$mois$jour$heure$minutes$secondes" >> "$trash_dir/INDEX"
numero=$((numero + 1))
echo "$numero" > "$trash_dir/ID"
echo "Le fichier $fichier a été supprimé et déplacé vers la corbeille avec succès."
done







