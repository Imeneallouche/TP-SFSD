#include <stdlib.h>
#include <stdio.h>
#include "lib.h"

/*************************************************************|
|                                                             |
|  Ouvrir fichier nom_fichier avec le mode correspondant TnOF |
|                                                             |
|*************************************************************/
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

/*************************************|
|                                     |
|      Fermer le fichier TnOF         |
|                                     |
|*************************************/
void Fermer_TnOF(fichier_TnOF f)
{
    fseek(f.fichier, 0, SEEK_SET);
    fwrite(&(f.entete), sizeof(entete_TnOF), 1, f.fichier); //
    fclose(f.fichier);
}

/********************************************|
|                                            |
|     Lire le i eme bloc dans buf TnOF       |
|                                            |
|********************************************/
void LireDir_TnOF(fichier_TnOF f, int i, Tampon *buf, int *cpt_lect)
{
    rewind(f.fichier);
    fseek(f.fichier, sizeof(fichier_TnOF) + (i - 1) * sizeof(Tampon), SEEK_SET); //
    fread(buf, sizeof(Tampon), 1, f.fichier);
    (*cpt_lect) = (*cpt_lect) + 1;
}

/*********************************************|
|                                             |
|     Ecrire buf dans le i eme bloc TnOF      |
|                                             |
|********************************************/
void EcrireDir_TnOF(fichier_TnOF f, int i, Tampon *buf, int *cpt_ecr)
{
    rewind(f.fichier);
    fseek(f.fichier, sizeof(entete_TnOF) + (i - 1) * sizeof(Tampon), SEEK_SET);
    fwrite(buf, sizeof(Tampon), 1, f.fichier);
    (*cpt_ecr) = (*cpt_ecr) + 1;
}

/*********************************************|
|                                             |
|  Retoure la i ème valeur del'entete  TnOF   |
|                                             |
|*********************************************/
int Entete_TnOF(fichier_TnOF f, int i)
{
    if (i == 1) // nombre de blocs total (numero du dernier bloc)
        return f.entete.blocs_total;
    if (i == 2) // nombre d'enregistrement inseres
        return f.entete.enreg_inseres;
    if (i == 3) // nombre d'enregistrements supprimes
        return f.entete.enreg_supprimes;
    else
        return -1;
}

/**********************************************|
|                                              |
|  Modifie la i ème valeur de l'entete  TnOF   |
|                                              |
|**********************************************/
void Aff_Entete_TnOF(fichier_TnOF *f, int i, int val)
{
    if (i == 1) // nombre de blocs total (numero du dernier bloc)
        f->entete.blocs_total = val;
    if (i == 2) // nombre d'enregistrement inseres
        f->entete.enreg_inseres = val;
    if (i == 3) // nombre d'enregistrements supprimes
        f->entete.enreg_supprimes = val;
    else
        printf("Parametre inexistant dans l'entete\n");
}

/*********************************************|
|                                             |
|   Retourne le numéro du nouveau bloc TnOF   |
|                                             |
|*********************************************/
int Alloc_bloc_TOF(fichier_TnOF f)
{
    return Entete_TOF(f, 1) + 1;
}