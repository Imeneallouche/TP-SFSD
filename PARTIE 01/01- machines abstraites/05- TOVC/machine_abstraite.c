#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

/*









/************************************************************
 |                                                          |
 |               Ouvrir fichier de nom_fichier              |
 |                                                          |
 ************************************************************/
TOVC *Ouvrir(char nom_fichier[30], char mode_ouverture)
{
    TOVC *fichier = malloc(sizeof(TOVC));

    /// si on ouvre en mode nouveau
    if ((mode_ouverture == 'n') || (mode_ouverture == 'N'))
    {
        fichier->f = fopen(nom_fichier, "wb+");
        if (fichier->f != NULL)
        {
            printf(" ouverture du fichier reussie\n\n");
            /// initialisation de l'entête :
            fichier->entete.adr_dernier_bloc = 0;
            fichier->entete.pos_libre_dernier_bloc = 0;
            fichier->entete.nbr_caract_insert = 0;
            fichier->entete.nbr_caract_supp = 0;

            rewind(fichier->f);                                          /// positionnement au début du fichier
            fwrite(&(fichier->entete), sizeof(S_Entete), 1, fichier->f); /// pourqoui il n'a pas fait directement size_of(entete)
        }
        else
            printf("fichier mal cree\n");
    }
    /// si on ouvre en mode ancien
    else
    {
        if ((mode_ouverture == 'a') || (mode_ouverture == 'A'))
        {
            fichier->f = fopen(nom_fichier, "rb+");
            if (fichier->f != NULL)
            {
                printf(" ouverture du fichier reussie\n");
                rewind(fichier->f);
                fread(&(fichier->entete), sizeof(S_Entete), 1, fichier->f);
                Affichage_Entete(fichier);
            }
            else
                printf(" fichier mal ouvert, creer le fichier avant de continuer\n");
        }
    }
    // rewind(fichier->f);
    return (fichier);
}

/*




/************************************************************
 |                                                          |
 |       Procedure de fermeture d'un fichier TOVC           |
 |                                                          |
 ************************************************************/
void Fermer(TOVC *fichier)
{
    rewind(fichier->f);
    fwrite(&(fichier->entete), sizeof(S_Entete), 1, fichier->f);
    fclose(fichier->f);
    free(fichier);
}

/*





/************************************************************
 |                                                          |
 |       Procedure de lecture d'un bloc methode TOVC        |
 |                                                          |
 ************************************************************/
/// permet de lire le i ème bloc du fichier TOVC pointé par fichier dans le Tbloc buf --------------------------------------------------
void Liredir(TOVC *fichier, int i, Tbloc *buf)
{
    /// on se jitionne au début du i ème bloc puis on le lit dans buf
    fseek(fichier->f, sizeof(S_Entete) + (sizeof(Tbloc) * (i - 1)), SEEK_SET);
    fread(buf, sizeof(Tbloc), 1, fichier->f);
}

/*






/************************************************************
 |                                                          |
 |      Fonction de lecture de l'entete methode TOVC        |
 |                                                          |
 ************************************************************/
/// Entete : retourne la caractèristique i du fichier TOVC -----------------------------------------------------------------------------
int Entete(TOVC *fichier, int i)
{
    if (i == 1)
    {
        return (fichier->entete.adr_dernier_bloc);
    }
    else if (i == 2)
    {
        return (fichier->entete.pos_libre_dernier_bloc);
    }
    else if (i == 3)
    {
        return (fichier->entete.nbr_caract_insert);
    }
    else
    {
        return (fichier->entete.nbr_caract_supp);
    }
}

/*





/************************************************************
 |                                                          |
 |       Procedure d'ecriture d'un bloc methode TOVC        |
 |                                                          |
 ************************************************************/
/// permet d'écrire le Tbloc buf au i ème bloc du fichier de type TOVC -----------------------------------------------------------------
void Ecriredir(TOVC *fichier, int i, Tbloc buf)
{
    /// on se positionne au début du i ème bloc puis on écrit dans fichier->f
    fseek(fichier->f, sizeof(S_Entete) + (sizeof(Tbloc) * (i - 1)), SEEK_SET);
    fwrite(&buf, sizeof(Tbloc), 1, fichier->f);
}

/*





/************************************************************
 |                                                          |
 |    Procedure de modification de l'entete methode TOVC    |
 |                                                          |
 ***********************************************************/
/// Aff-Entete : permet d'affecter val à la caractéristique i choisie ------------------------------------------------------------------
void Aff_Entete(TOVC *fichier, int i, int val)
{
    if (i == 1)
    {
        fichier->entete.adr_dernier_bloc = val;
    }
    else if (i == 2)
    {
        fichier->entete.pos_libre_dernier_bloc = val;
    }
    else if (i == 3)
    {
        fichier->entete.nbr_caract_insert = val;
    }
    else
    {
        fichier->entete.nbr_caract_supp = val;
    }
}

/*






/************************************************************
 |                                                          |
 |         allocation d'un nouveau bloc methode TOVC        |
 |                                                          |
 ***********************************************************/
/// permet d'allouer un nouveau bloc ---------------------------------------------------------------------------------------------------
void Alloc_bloc(TOVC *fichier) { Aff_Entete(fichier, 1, Entete(fichier, 1) + 1); }
///-------------------------------------------------------------------------------------------------------------------------------------
