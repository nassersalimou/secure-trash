#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char lettre;
    int index;
} Colonne;

int comparer_colonnes(const void *a, const void *b) {
    Colonne *col1 = (Colonne *)a;
    Colonne *col2 = (Colonne *)b;
    if (col1->lettre == col2->lettre) {
        return col1->index - col2->index; // Priorité à l'ordre initial pour doublons
    }
    return col1->lettre - col2->lettre; // Ordre alphabétique
}

void generer_ordre(const char *mot_de_passe, int *ordre) {
    int k = strlen(mot_de_passe);
    Colonne colonnes[k];

    for (int i = 0; i < k; i++) {
        colonnes[i].lettre = mot_de_passe[i];
        colonnes[i].index = i;
    }

    qsort(colonnes, k, sizeof(Colonne), comparer_colonnes);

    for (int i = 0; i < k; i++) {
        ordre[i] = colonnes[i].index;
    }
}

void generer_ordre_inverse(int *ordre, int *ordre_inverse, int taille) {
    for (int i = 0; i < taille; i++) {
        ordre_inverse[ordre[i]] = i;
    }
}

void transposer(const char *contenu, const char *mot_de_passe) {
    int cols = strlen(mot_de_passe);
    int ordre[cols];
    generer_ordre(mot_de_passe, ordre);

    int longueur = strlen(contenu);
    int rows = (longueur + cols - 1) / cols;

    for (int col = 0; col < cols; col++) {
        char filename[20];
        snprintf(filename, sizeof(filename), "fichier%d", col + 1);
        FILE *file = fopen(filename, "w");
        if (file == NULL) {
            perror("Erreur d'ouverture du fichier");
            exit(EXIT_FAILURE);
        }

        int col_actuelle = ordre[col];
        for (int row = 0; row < rows; row++) {
            int index = row * cols + col_actuelle;
            if (index < longueur) {
                fputc(contenu[index], file);
            }
        }
        fclose(file);
        printf("Colonne %d (ordre réel : %d) enregistrée dans %s\n", col + 1, col_actuelle + 1, filename);
    }
}

void detransposer(const char *mot_de_passe, const char *output_file, int longueur) {
    int cols = strlen(mot_de_passe);
    int ordre[cols];
    int ordre_inverse[cols];
    generer_ordre(mot_de_passe, ordre);
    generer_ordre_inverse(ordre, ordre_inverse, cols);

    int rows = (longueur + cols - 1) / cols;

    char **colonnes = malloc(cols * sizeof(char *));
    for (int i = 0; i < cols; i++) {
        colonnes[i] = malloc(rows * sizeof(char));
        char filename[20];
        snprintf(filename, sizeof(filename), "fichier%d", ordre_inverse[i] + 1);
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            perror("Erreur d'ouverture d'une colonne");
            exit(EXIT_FAILURE);
        }

        for (int row = 0; row < rows; row++) {
            int c = fgetc(file);
            if (c != EOF) {
                colonnes[i][row] = c;
            } else {
                colonnes[i][row] = ' ';
            }
        }
        fclose(file);
    }

    FILE *output = fopen(output_file, "w");
    if (output == NULL) {
        perror("Erreur d'ouverture du fichier de sortie");
        exit(EXIT_FAILURE);
    }

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int index = row * cols + col;
            if (index < longueur) {
                fputc(colonnes[col][row], output);
            }
        }
    }

    fclose(output);
    printf("Contenu reconstruit dans %s\n", output_file);

    for (int i = 0; i < cols; i++) {
        free(colonnes[i]);
    }
    free(colonnes);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <mode> <fichier_source> <mot_de_passe>\n", argv[0]);
        fprintf(stderr, "Mode : -t (transposition), -d (détransposition)\n");
        return EXIT_FAILURE;
    }

    const char *mode = argv[1];
    const char *fichier_source = argv[2];
    const char *mot_de_passe = argv[3];

    if (strcmp(mode, "-t") == 0) {
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

        transposer(contenu, mot_de_passe);

        free(contenu);
    } else if (strcmp(mode, "-d") == 0) {
        FILE *file = fopen(fichier_source, "r");
        if (file == NULL) {
            perror("Erreur d'ouverture du fichier source pour longueur");
            return EXIT_FAILURE;
        }

        fseek(file, 0, SEEK_END);
        long longueur = ftell(file);
        fclose(file);

        detransposer(mot_de_passe, "reconstruit.txt", longueur);
    } else {
        fprintf(stderr, "Mode non reconnu : %s. Utilisez -t ou -d\n", mode);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}