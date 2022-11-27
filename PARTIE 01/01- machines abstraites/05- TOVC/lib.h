#define max_caractere 300 // le nombre max de caractère que peut contenir le bloc
#include <stdio.h>

/**********************************************|
|                                              |
|    STRUCTURES ET VARIABLES GLOBALES TOVC     |
|                                              |
|**********************************************/
/*********************************************|
|                                             |
|         Structure d'en_tete TOVC            |
|                                             |
|*********************************************/
typedef struct entete_TOVC
{
    int adr_dernier_bloc;       // adresse du dernier bloc
    int pos_libre_dernier_bloc; // position libre dans le dernier bloc
    int nbr_caract_insert;      // nombre de caracteres inseres (nous aidera dans la reorganisation)
    int nbr_caract_supp;        // nombre de caracteres supprimes (nous aidera dans la reorganisation)

} entete_TOVC;

/*********************************************|
|                                             |
|          Structure de bloc TOVC             |
|                                             |
|*********************************************/
typedef struct Tbloc_TOVC
{
    char tableau[max_caractere + 1]; // taille variable

} Tbloc_TOVC;

/*********************************************|
|                                             |
|          structure du fichier TOVC          |
|                                             |
|*********************************************/
typedef struct fichier_TOVC
{
    FILE *fichier;
    entete_TOVC entete;

} fichier_TOVC;

typedef Tbloc_TOVC Tampon; // definition du type Tampon TOF(un alias du type tbloc)

/*********************************************|
|                                             |
|            Machine Abstraite TOF            |
|                                             |
|*********************************************/
void Ouvrir_TOVC(fichier_TOVC *f, char nom_fichier[], char mode_ouverture);
void Fermer_TOVC(fichier_TOVC *f);
int Entete_TOVC(fichier_TOVC *f, int i);
void Aff_Entete_TOVC(fichier_TOVC *f, int i, int val);
void EcrireDir_TOVC(fichier_TOVC *f, int i, Tbloc_TOVC buf);
void LireDir_TOVC(fichier_TOVC *f, int i, Tbloc_TOVC *buf);
int Alloc_bloc_TOVC(fichier_TOVC *f);