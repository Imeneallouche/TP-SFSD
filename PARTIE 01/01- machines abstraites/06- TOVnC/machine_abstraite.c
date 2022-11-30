#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lib.h"

/*************************************************************|
|                                                             |
| Ouvrir fichier nom_fichier avec le mode correspondant TOVnC |
|                                                             |
|*************************************************************/
void Ouvrir_TOVnC(fichier_TOVnC *f, char nom_fichier[], char mode_ouverture)
{
    /// si on ouvre en mode nouveau
    if (tolower(mode_ouverture) == 'n')
    {
        f->fichier = fopen(nom_fichier, "wb+");
        if (f->fichier != NULL)
        {
            /// initialisation de l'entête :
            f->entete.nombre_bloc = 0;
            f->entete.nbr_caract_insert = 0;
            f->entete.nbr_caract_supp = 0;

            rewind(f->fichier); /// positionnement au début du fichier
            fwrite(&(f->entete), sizeof(entete_TOVnC), 1, f->fichier);
        }
        else
            printf("\nErreur lors de l'ouverture du fichier... verifier le nom du fichier");
    }
    /// si on ouvre en mode ancien
    else if (tolower(mode_ouverture) == 'a')
    {
        f->fichier = fopen(nom_fichier, "rb+");
        if (f->fichier != NULL)
        {
            rewind(f->fichier);
            fread(&(f->entete), sizeof(entete_TOVnC), 1, f->fichier);
        }
        else
            printf("\nErreur lors de l'ouverture du fichier... verifier le nom du fichier");
    }
    else
    {
        f->fichier = NULL;
        printf("Mode d'ouverture erronne\n");
    }
}

/*************************************|
|                                     |
|     Fermer le fichier TOVnC         |
|                                     |
|*************************************/
void Fermer_TOVnC(fichier_TOVnC *f)
{
    rewind(f->fichier);
    fwrite(&(f->entete), sizeof(entete_TOVnC), 1, f->fichier);
    fclose(f->fichier);
    free(f);
}

/********************************************|
|                                            |
|    Lire le i eme bloc dans buf TOVnC       |
|                                            |
|********************************************/
void LireDir_TOVnC(fichier_TOVnC *f, int i, Tbloc_TOVnC *buf)
{
    /// on se jitionne au début du i ème bloc puis on le lit dans buf
    fseek(f->fichier, sizeof(entete_TOVnC) + (sizeof(Tbloc_TOVnC) * (i - 1)), SEEK_SET);
    fread(buf, sizeof(Tbloc_TOVnC), 1, f->fichier);
}

/**********************************************|
|                                              |
|  Retoure la i ème valeur del'entete  TOVnC   |
|                                              |
|**********************************************/
int Entete_TOVnC(fichier_TOVnC *f, int i)
{
    if (i == 1)
        return (f->entete.nombre_bloc);
    if (i == 2)
        return (f->entete.nbr_caract_insert);
    if (i == 3)
        return (f->entete.nbr_caract_supp);
    else
        printf("Parametre inexistant dans l'entete\n");
}

/*********************************************|
|                                             |
|    Ecrire buf dans le i eme bloc TOVnC      |
|                                             |
|*********************************************/
void EcrireDir_TOVnC(fichier_TOVnC *f, int i, Tbloc_TOVnC buf)
{
    /// on se positionne au début du i ème bloc puis on écrit dans fichier->f
    fseek(f->fichier, sizeof(entete_TOVnC) + (sizeof(Tbloc_TOVnC) * (i - 1)), SEEK_SET);
    fwrite(&buf, sizeof(Tbloc_TOVnC), 1, f->fichier);
}

/***********************************************|
|                                               |
|  modifie la i ème valeur de l'entete  TOVnC   |
|                                               |
|***********************************************/
void Aff_Entete_TOVnC(fichier_TOVnC *f, int i, int val)
{
    if (i == 1)
        f->entete.nombre_bloc = val;
    if (i == 2)
        f->entete.nbr_caract_insert = val;
    if (i == 3)
        f->entete.nbr_caract_supp = val;
    else
        printf("Parametre inexistant dans l'entete\n");
}

/**********************************************|
|                                              |
|   Retourne le numéro du nouveau bloc TOVnC   |
|                                              |
|**********************************************/
int Alloc_bloc_TOVnC(fichier_TOVnC *f)
{
    int i = Entete_TOVnC(f, 1);    // le nombre de bloc = le numero du dernier bloc -1
    Aff_Entete_TOVnC(f, 1, i + 1); // mettre a jour le nombre de bloc dans l'entete
    return i;                      // le nombre anciens de bloc = numero du dernier bloc
}
