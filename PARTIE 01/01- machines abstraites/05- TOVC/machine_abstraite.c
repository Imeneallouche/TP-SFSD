#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lib.h"

/************************************************************
 |                                                          |
 |               Ouvrir fichier de nom_fichier              |
 |                                                          |
 ************************************************************/
void Ouvrir_TOVC(fichier_TOVC *f, char nom_fichier[], char mode_ouverture)
{
    /// si on ouvre en mode nouveau
    if (tolower(mode_ouverture) == 'n')
    {
        f->fichier = fopen(nom_fichier, "wb+");
        if (f->fichier != NULL)
        {
            /// initialisation de l'entête :
            f->entete.adr_dernier_bloc = 0;
            f->entete.pos_libre_dernier_bloc = 0;
            f->entete.nbr_caract_insert = 0;
            f->entete.nbr_caract_supp = 0;

            rewind(f->fichier); /// positionnement au début du fichier
            fwrite(&(f->entete), sizeof(entete_TOVC), 1, f->fichier);
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
            fread(&(f->entete), sizeof(entete_TOVC), 1, f->fichier);
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

/************************************************************
 |                                                          |
 |       Procedure de fermeture d'un fichier TOVC           |
 |                                                          |
 ************************************************************/
void Fermer_TOVC(fichier_TOVC *f)
{
    rewind(f->fichier);
    fwrite(&(f->entete), sizeof(entete_TOVC), 1, f->fichier);
    fclose(f->fichier);
    free(f);
}

/************************************************************
 |                                                          |
 |       Procedure de lecture d'un bloc methode TOVC        |
 |                                                          |
 ************************************************************/
void LireDir_TOVC(fichier_TOVC *f, int i, Tbloc_TOVC *buf)
{
    /// on se jitionne au début du i ème bloc puis on le lit dans buf
    fseek(f->fichier, sizeof(entete_TOVC) + (sizeof(Tbloc_TOVC) * (i - 1)), SEEK_SET);
    fread(buf, sizeof(Tbloc_TOVC), 1, f->fichier);
}

/************************************************************
 |                                                          |
 |      Fonction de lecture de l'entete methode TOVC        |
 |                                                          |
 ************************************************************/
int Entete_TOVC(fichier_TOVC *f, int i)
{
    if (i == 1)
        return (f->entete.adr_dernier_bloc);
    if (i == 2)
        return (f->entete.pos_libre_dernier_bloc);
    if (i == 3)
        return (f->entete.nbr_caract_insert);
    if (i == 4)
        return (f->entete.nbr_caract_supp);
    else
        printf("Parametre inexistant dans l'entete\n");
}

/************************************************************
 |                                                          |
 |       Procedure d'ecriture d'un bloc methode TOVC        |
 |                                                          |
 ***********************************************************/
void EcrireDir_TOVC(fichier_TOVC *f, int i, Tbloc_TOVC buf)
{
    /// on se positionne au début du i ème bloc puis on écrit dans fichier->f
    fseek(f->fichier, sizeof(entete_TOVC) + (sizeof(Tbloc_TOVC) * (i - 1)), SEEK_SET);
    fwrite(&buf, sizeof(Tbloc_TOVC), 1, f->fichier);
}

/************************************************************
 |                                                          |
 |    Procedure de modification de l'entete methode TOVC    |
 |                                                          |
 ***********************************************************/
void Aff_Entete_TOVC(fichier_TOVC *f, int i, int val)
{
    if (i == 1)
        f->entete.adr_dernier_bloc = val;
    if (i == 2)
        f->entete.pos_libre_dernier_bloc = val;
    if (i == 3)
        f->entete.nbr_caract_insert = val;
    if (i == 4)
        f->entete.nbr_caract_supp = val;
    else
        printf("Parametre inexistant dans l'entete\n");
}

/************************************************************
 |                                                          |
 |         allocation d'un nouveau bloc methode TOVC        |
 |                                                          |
 ***********************************************************/
int Alloc_bloc_TOVC(fichier_TOVC *f)
{
    Aff_Entete_TOVC(f, 1, Entete_TOVC(f, 1) + 1);
    return Entete_TOVC(f, 1);
}