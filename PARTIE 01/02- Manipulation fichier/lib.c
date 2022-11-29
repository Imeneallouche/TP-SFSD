#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

/*********************************************************************
|                                                                    |
|   permet de transformer un int num en un string s de longueur max  |
|                la difference est comblée par des zéros             |
|                                                                    |
|********************************************************************/
void num_chaine(int num, int max, char *s)
{
    char s_num[4]; /// on sait que l'int num est sur 3 positions

    sprintf(s_num, "%d", num);

    int j = max - strlen(s_num);
    sprintf(s, "%s", "");
    while (j > 0)
    {
        sprintf(s, "%s0", s);
        j--;
    }
    sprintf(s, "%s%s", s, s_num);
}

/*********************************************************************
|                                                                    |
|       permet de copier de la chaine s à partir de la position i    |
|                   une sous chaine r de longueur max                |
|                                                                    |
 ********************************************************************/
void copier_chaine(char *s, int i, int max, char *r)
{
    /// if (i < 0 ) i = 0 ;
    sprintf(r, "%s", "");
    while (i < strlen(s) && max > 0)
    {
        sprintf(r, "%s%c", r, s[i]);
        i++;
        max--;
    }
}

/*********************************************************************
|                                                                    |
|           permet de supprimer une partie de la chaine s            |
|             de longueur max à partir de la position i              |
|                                                                    |
 ********************************************************************/
void couper_chaine(char *s, int i, int max)
{
    char partie_droite[max_caractere + 1];
    char partie_gauche[max_caractere + 1];

    copier_chaine(s, 0, i, partie_gauche);
    copier_chaine(s, i + max, strlen(s), partie_droite);

    sprintf(s, "%s%s", partie_gauche, partie_droite);
}

/*********************************************************************
|                                                                    |
|  permet de transformer un semi_enregistrement en un enregistrement |
|           utilité : facilité de manipulation des données           |
|                                                                    |
 ********************************************************************/
void semi_to_enreg(Semi_enreg se, Enregistrement *en)
{
    char chaine[max_caractere + 1];

    /// la clé
    sprintf(chaine, "%s", "");
    copier_chaine(se, 3, 3, chaine);
    en->cle = atoi(chaine);

    /// le booleen
    sprintf(chaine, "%c", se[6]);
    en->supp = atoi(chaine);

    /// l'info
    copier_chaine(se, 7, strlen(se) - 7, en->info);
}

/*********************************************************************
|                                                                    |
|  permet de transformer un enregistrement en un semi_enregistrement |
|                                                                    |
|                                                                    |
 ********************************************************************/
void enreg_to_semi(Enregistrement en, Semi_enreg se)
{
    char chaine[4];
    int taille = strlen(en.info);

    sprintf(se, "%s", "");

    /// écriture de la taille de l'info dans le semi enregistrement
    num_chaine(taille, 3, chaine);
    sprintf(se, "%s%s", se, chaine);

    /// écriture de la clé dans le semi enregistrement
    num_chaine(en.cle, 3, chaine);
    sprintf(se, "%s%s", se, chaine);

    /// écriture du booléen supp
    num_chaine(en.supp, 1, chaine);
    sprintf(se, "%s%s", se, chaine);

    /// écriture de l'info
    sprintf(se, "%s%s", se, en.info);
}

/*********************************************************************
|                                                                    |
|   semi_enreg se = |longueur de l'info|cle|supp|l'information|      |
|       permet de récupèrer un enregistrement du fichier             |
|             et de le placer dans la variable se                    |
|                                                                    |
|********************************************************************/
void recuperer_se(TOVC *fichier, int *i, int *j, Semi_enreg se)
{
    Tbloc buf;
    char taille[4]; /// contiendra la taille de l'info
    int k;

    sprintf(taille, "%s", "");
    sprintf(se, "%s", "");
    Liredir(fichier, (*i), &buf);

    /// récuperation de la taille de l'info, int dans taille, char dans se
    for (k = 0; k < 3; k++)
    {
        if ((*j) < max_caractere)
        {
            sprintf(taille, "%s%c", taille, buf.tableau[*j]);
            sprintf(se, "%s%c", se, buf.tableau[*j]);
            (*j)++;
        }
        else
        {
            (*i)++;
            Liredir(fichier, (*i), &buf);
            sprintf(taille, "%s%c", taille, buf.tableau[0]);
            sprintf(se, "%s%c", se, buf.tableau[0]);
            (*j) = 1;
        }
    }

    /// recuperation de la clé, le booléen et l'info dans la variable se
    for (k = 0; k < (atoi(taille) + 4); k++)
    {
        if ((*j) < max_caractere)
        {
            sprintf(se, "%s%c", se, buf.tableau[*j]);
            (*j)++;
        }
        else
        {
            (*i)++;
            Liredir(fichier, (*i), &buf);
            sprintf(se, "%s%c", se, buf.tableau[0]);
            (*j) = 1;
        }
    }
}

/*********************************************************************
|                                                                    |
|               affichage de l'Entete du fichier                     |
|                                                                    |
|                                                                    |
 ********************************************************************/
void Affichage_Entete(TOVC *fichier)
{
    printf("\n Les caracteristiques du fichier ----------------------------------------\n\n");
    printf(" -> Adresse dernier bloc : %d\n", Entete(fichier, 1));
    printf(" -> position libre dernier bloc : %d\n", Entete(fichier, 2));
    printf(" -> Nombre caracteres inseres : %d\n", Entete(fichier, 3));
    printf(" -> Nombre caracteres supprimes : %d\n", Entete(fichier, 4));
}

/*********************************************************************
|                                                                    |
|                        affichage d'un bloc i                       |
|                                                                    |
|                                                                    |
 ********************************************************************/
void Affichage_bloc(TOVC *fichier)
{
    Tbloc buf;
    int i;
    printf("\n\n Le contenu des blocs ------------------------------------------------------\n");
    if (Entete(fichier, 1) > 0)
    {
        for (i = 1; i <= Entete(fichier, 1); i++)
        {
            Liredir(fichier, i, &buf);
            printf(" Le contenu du bloc %d : %s\n", i, buf.tableau);
        }
    }
}

/*********************************************************************
|                                                                    |
|          permet d'afficher les enregistrements du fichier          |
|        enregistrements supprimés logiquement : non-affichés        |
|                                                                    |
|********************************************************************/
void Affichage_fichier(TOVC *fichier)
{
    int i = 1;
    int j = 0;
    Enregistrement en;
    Semi_enreg se;
    char taille[4];

    printf("\n\n Les enregistrements --------------------------------------------------\n\n");
    while (i <= Entete(fichier, 1))
    {
        recuperer_se(fichier, &i, &j, se);
        /// printf("se = %s   i = %d   j = %d\n",se,i,j);
        semi_to_enreg(se, &en);
        num_chaine(en.cle, 3, taille);

        printf("  %s | %d | %s\n", taille, en.supp, en.info);
        if ((i == Entete(fichier, 1)) && (j == Entete(fichier, 2)))
            break;
    }
}

/*********************************************************************
|                                                                    |
|       permet d'afficher les enregistrements en chevauchement       |
|                                                                    |
|                                                                    |
 ********************************************************************/
void Affichage_chevauchement(TOVC *fichier)
{
    int i = 1;
    int i1 = 1; /// i1 servira de comparaison avec i
    int j = 0;
    int j1 = 0;

    Enregistrement en;
    Semi_enreg se;
    char taille[4];

    printf("\n\nLes chevauchements:\n\n");
    while (i <= Entete(fichier, 1))
    {
        recuperer_se(fichier, &i1, &j1, se); /// si i1 != i => changement de bloc donc chevauchement
        if (i1 != i)
        {
            semi_to_enreg(se, &en);        /// transformation de la chaine en enregistrement
            num_chaine(en.cle, 3, taille); /// géneration de la taille de l'info

            /// affichage que si l'enregistrement n'est pas supprimé logiquement
            if (en.supp == 0)
            {
                printf("entre bloc %d et %d : %s | %d | %s  \n", i, i1, taille, en.supp, en.info);
            }

            if (j1 == max_caractere)
            {
                i1++;
                j1 = 0;
            }
            /// i et j reçoient les valeurs de i1 et j1 pour pouvoir faire la comparaison au prochain enregistrement
            i = i1;
            j = j1;
        }
        if ((i == Entete(fichier, 1)) && (j == Entete(fichier, 2)))
            break;
    }
}