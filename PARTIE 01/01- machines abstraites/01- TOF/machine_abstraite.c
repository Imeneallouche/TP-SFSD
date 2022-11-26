#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "lib.h"

/************************************************************|
|                                                            |
|  Ouvrir fichier nom_fichier avec le mode correspondant TOF |
|                                                            |
|************************************************************/
void Ouvrir_TOF(fichier_TOF *f, char nom_fichier[], char mode)
{
    if (tolower(mode) == 'a')
    {
        f->fichier = fopen(nom_fichier, "rb+");
        if (f->fichier == NULL)
            printf("\nErreur lors de l'ouverture du fichier... verifier le nom du fichier");
        else
            fread(&(f->entete), sizeof(entete_TOF), 1, f->fichier);
    }
    else if (tolower(mode) == 'n')
    {
        f->fichier = fopen(nom_fichier, "wb+");
        Aff_Entete_TOF(f, 1, 0); // mettre le nombre de blocs à 0
        Aff_Entete_TOF(f, 2, 0); // mettre le nombre d'enregistrements inseres à 0
        Aff_Entete_TOF(f, 3, 0); // mettre le nombre d'enregistrements supprimes à 0
    }
    else
    {
        f->fichier = NULL;
        printf("Mode d'ouverture erronne\n");
    }
}

/*************************************|
|                                     |
|       Fermer le fichier TOF         |
|                                     |
|*************************************/
void Fermer_TOF(fichier_TOF f)
{
    fseek(f.fichier, 0, SEEK_SET);
    fwrite(&(f.entete), sizeof(entete_TOF), 1, f.fichier);
    fclose(f.fichier);
}

/********************************************|
|                                            |
|      Lire le i eme bloc dans buf TOF       |
|                                            |
|********************************************/
void LireDir_TOF(fichier_TOF f, int i, Tampon *buf, int *cpt_lect)
{
    rewind(f.fichier);
    fseek(f.fichier, sizeof(fichier_TOF) + (i) * sizeof(Tampon), SEEK_SET);
    fread(buf, sizeof(Tampon), 1, f.fichier);
    (*cpt_lect) = (*cpt_lect) + 1;
}

/*********************************************|
|                                             |
|      Ecrire buf dans le i eme bloc TOF      |
|                                             |
|********************************************/
void EcrireDir_TOF(fichier_TOF f, int i, Tampon *buf, int *cpt_ecr)
{
    rewind(f.fichier);
    fseek(f.fichier, sizeof(entete_TOF) + (i) * sizeof(Tampon), SEEK_SET);
    fwrite(buf, sizeof(Tampon), 1, f.fichier);
    (*cpt_ecr) = (*cpt_ecr) + 1;
}

/********************************************|
|                                            |
|  Retoure la i ème valeur del'entete  TOF   |
|                                            |
|********************************************/
int Entete_TOF(fichier_TOF f, int i)
{
    if (i == 1) // nombre de blocs total
        return f.entete.blocs_total;
    else if (i == 2) // nombre d'enregistrements inseres
        return f.entete.enreg_inseres;
    else if (i == 3) // nombre d'enregistrements supprimes
        return f.entete.enreg_supprimes;
    else
        return -1;
}

/*********************************************|
|                                             |
|  modifie la i ème valeur de l'entete  TOF   |
|                                             |
|*********************************************/
void Aff_Entete_TOF(fichier_TOF *f, int i, int val)
{
    if (i == 1) // nombre de blocs total
        f->entete.blocs_total = val;
    if (i == 2) // nombre d'enregistrements inseres
        f->entete.enreg_inseres = val;
    if (i == 3) // nombre d'enregistrements supprimes
        f->entete.enreg_supprimes = val;
    else
        printf("Parametre inexistant dans l'entete\n");
}

/********************************************|
|                                            |
|   Retourne le numéro du nouveau bloc TOF   |
|                                            |
|********************************************/
int Alloc_bloc_TOF(fichier_TOF *f)
{
    Aff_Entete_TOF(f, 1, Entete_TOF(*f, 1) + 1);
    return Entete_TOF(*f, 1);
}