#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void transposer(const char *contenu, int k) {
    int longueur = strlen(contenu);
    int rows = (longueur + k - 1) / k; // Calcul du nombre de lignes nécessaires

    // Création des fichiers pour chaque colonne
    for (int col = 0; col < k; col++) {
        char filename[20];
        snprintf(filename, sizeof(filename), "fichier%d", col + 1);
        FILE *file = fopen(filename, "w");
        if (file == NULL) {
            perror("Erreur d'ouverture du fichier");
            exit(EXIT_FAILURE);
        }

        // Remplissage de chaque colonne
        for (int row = 0; row < rows; row++) {
            int index = row * k + col; // Calcul de l'index dans le contenu
            if (index < longueur) {
                fputc(contenu[index], file); // Écrire le caractère dans le fichier
            }
        }
        fclose(file);
        printf("Colonne %d enregistrée dans %s\n", col + 1, filename);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <fichier_source> <indice_k>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *fichier_source = argv[1];
    int k = atoi(argv[2]);  // Convertir l'indice k en entier

    if (k <= 0) {
        fprintf(stderr, "L'indice k doit être un entier positif.\n");
        return EXIT_FAILURE;
    }

    FILE *file = fopen(fichier_source, "r");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier source");
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_END);
    long taille = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *contenu = malloc(taille + 1);
    if (contenu == NULL) {
        perror("Erreur d'allocation pour le contenu");
        fclose(file);
        return EXIT_FAILURE;
    }
    fread(contenu, 1, taille, file);
    contenu[taille] = '\0';
    fclose(file);

    transposer(contenu, k); // Appel à la fonction de transposition avec l'indice k

    free(contenu);
    return EXIT_SUCCESS;
}