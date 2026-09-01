# Secure Trash

Corbeille en ligne de commande réalisée en Bash et C. Elle permet de déplacer des fichiers dans une corbeille locale, de conserver leurs métadonnées et de les restaurer.

## Fonctionnalités

- initialisation d'une corbeille cachée ;
- suppression logique de fichiers et dossiers ;
- indexation du chemin d'origine et de la date ;
- affichage du contenu de la corbeille ;
- restauration au chemin d'origine ou dans un dossier choisi ;
- expérimentation de fragmentation et de transposition en C.

## Compilation

```bash
make
```

## Utilisation

Les scripts d'origine vérifient qu'ils sont lancés depuis un dossier nommé `sae_broyeur` :

```bash
mkdir sae_broyeur
cp init-trashbox.sh sae_delete.sh sae_restore.sh sae_trashbox_ls.sh transpose_file transpose_mdp sae_broyeur/
cd sae_broyeur
./init-trashbox.sh
./sae_delete.sh chemin/vers/fichier
./sae_trashbox_ls.sh
./sae_restore.sh nom_du_fichier
```

## Avertissement

Projet universitaire expérimental. Testez-le uniquement sur des copies de fichiers : les scripts utilisent `mv` et modifient l'index de la corbeille.
