#include <stdio.h>
#define TAILLE 1024                                      // La taille d'un bloc en octets
#define MAX2 ((TAILLE - 2 * sizeof(int)) / sizeof(char)) // le nombre de caracteres maximale qu'un bloc peut contenir
#define u 90                                             // pourcentage (%) de remplissement des blocs lors du chargement initial
#define champs_cle 5                                     // la taille du champs cle
#define champs_efface 1                                  // la taille du champs efface
#define champs_taille 3                                  // la taille du champs taille
#define champs_valeur 50

/*********************************************|
|                                             |
|   STRUCTURES ET VARIABLES GLOBALES LOVC     |
|                                             |
|*********************************************/

/*********************************************|
|                                             |
|      structure de l'enregistrement LOVC     |
|                                             |
|*********************************************/
typedef struct Tbloc_LOVC
{
    char tab[MAX2]; // le tableau de caracteres
    int suivant;    // le numero du bloc suivant dans la liste

} Tbloc_LOVC;

typedef struct Tbloc_LOVC Buffer_LOVC; // identique a la structure du bloc en methode LOVC

/*********************************************|
|                                             |
|         Structure d'en_tete LOVC            |
|                                             |
|*********************************************/
typedef struct Entete_LOVC
{
    int Num_premier_bloc;       // le numéro(adresse) du premier bloc
    int Num_dernier_bloc;       // le numero (adresse)du dernier bloc
    int pos_libre_dernier_bloc; // la position libre dans le dernier bloc
    int nb_chars_inseres;       // nombre de caractères inserés
    int nb_chars_supprimes;     // nombre de caractères supprimées
} Entete_LOVC;

/*********************************************|
|                                             |
|         structure du fichier LOVC           |
|                                             |
|*********************************************/
typedef struct fichier_LOVC
{
    FILE *fichier;
    Entete_LOVC entete;
} fichier_LOVC;

/*********************************************|
|                                             |
|           Machine Abstraite LOVC            |
|                                             |
|*********************************************/
void Ouvrir_LOVC(fichier_LOVC *f, const char nomFichier[], char mode);
void fermer_LOVC(fichier_LOVC *f);
void LireDir_LOVC(fichier_LOVC *f, int i, Buffer_LOVC *buf, int *cpt_lect);
void ecrireDir_LOVC(fichier_LOVC *f, int i, Buffer_LOVC *buf, int *cpt_ecr);
int entete_LOVC(fichier_LOVC *f, int i);
void aff_entete_LOVC(fichier_LOVC *f, int i, int val);
int alloc_bloc_LOVC(fichier_LOVC *fichier, int *cpt_lect, int *cpt_ecr, Buffer_LOVC *buf);