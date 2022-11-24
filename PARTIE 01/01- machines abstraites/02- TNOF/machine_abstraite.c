#include <stdlib.h>
#include <stdio.h>
#include "lib.h"

// ouvrir le fichier TnOF dans le mode correspondant
void Ouvrir_TnOF(fichier_TnOF *f, char nom_fichier[30], char mode)
{
    if (tolower(mode) == 'a')
    {
        f->fichier = fopen(nom_fichier, "rb+");
        if (f->fichier == NULL)
            printf("error interrupted the program ... check fichier's name");
        else
            fread(&(f->entete), sizeof(entete_TnOF), 1, f->fichier);
    }
    else if (tolower(mode) == 'n')
    {
        f->fichier = fopen(nom_fichier, "wb+");
        Aff_Entete(f, 1, 0); // mettre le nombre de blocs à 0
        Aff_Entete(f, 2, 0); // mettre le nombre d'enregistrements inseres à 0
        Aff_Entete(f, 3, 0); // mettre le nombre d'enregistrements supprimes à 0
    }
    else
    {
        f->fichier = NULL;
        printf("Mode d'ouverture erronné\n");
    }
}

// fermer le fichier TnOF
void Fermer_TnOF(fichier_TnOF f)
{
    fseek(f.fichier, 0, SEEK_SET);
    fwrite(&(f.entete), sizeof(entete_TnOF), 1, f.fichier); //
    fclose(f.fichier);
}

// lire le i ème bloc
void LireDir_TnOF(fichier_TnOF f, int i, Tampon *buf, int *cpt_lect)
{
    rewind(f.fichier);
    fseek(f.fichier, sizeof(fichier_TnOF) + (i - 1) * sizeof(Tampon), SEEK_SET); //
    fread(buf, sizeof(Tampon), 1, f.fichier);
    (*cpt_lect) = (*cpt_lect) + 1;
}

// ecrire un bloc
void EcrireDir_TnOF(fichier_TnOF f, int i, Tampon *buf, int *cpt_ecr)
{
    rewind(f.fichier);
    fseek(f.fichier, sizeof(entete_TnOF) + (i - 1) * sizeof(Tampon), SEEK_SET);
    fwrite(buf, sizeof(Tampon), 1, f.fichier);
    (*cpt_ecr) = (*cpt_ecr) + 1;
}

// retoure la i ème valeur del'entete
int Entete_TnOF(fichier_TnOF f, int i)
{
    if (i == 1)
        return f.entete.blocs_total;
    else if (i == 2)
        return f.entete.enreg_inseres;
    else if (i == 3)
        return f.entete.enreg_supprimes;
    else
        return -1;
}

// 1 pour le nombre de bloc total
// 2 pour le nombre d'enregistrement inseres
// 3 pour le nombre d'enregistrements supprimes
void Aff_Entete_TnOF(fichier_TnOF *f, int i, int val)
{
    if (i == 1)
        f->entete.blocs_total = val;
    else if (i == 2)
        f->entete.enreg_inseres = val;
    else if (i == 3)
        f->entete.enreg_supprimes = val;
    else
        printf("Parametre inexistant dans l'entete\n");
}

// it extracts a key according to its index that is given in the variable number
int extract_nbr(int nbr)
{
    char chaine[20];
    int cpt = 1;
    fichier_TnOF *fichier = fopen("chargement_init.txt", "r");
    if (!fichier)
        printf("erreur\n");
    while (fgets(chaine, 20, fichier) && (cpt < nbr))
        cpt++;
    int nb;
    sscanf(chaine, "%d\n", &nb);
    fclose(fichier);
    return nb;
}