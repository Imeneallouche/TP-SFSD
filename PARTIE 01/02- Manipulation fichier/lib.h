#include <stdio.h>
#define B 300                                             // le nombre max de caractère que peut contenir le bloc
#define FICHIER_ORIGINAL "Materiel_informatique_TOVĈ.bin" // le nom du fichier original
#define NB_TYPE_MATERIEL 6                                // nombre de types de materiel qui existent
#define TAILLE_IDENTIFIANT 5                              // la taille du champs identifiant (la cle) sur 5 octets
#define TAILLE_PRIX 8                                     // la taille du champs prix du materiel
#define PRIX_MAX 99999999                                 // le prix max d'un materiel

/***********************************************|
|                                               |
|    STRUCTURES ET VARIABLES GLOBALES TOVnC     |
|                                               |
|***********************************************/
/*********************************************|
|                                             |
|        Structure d'en_tete TOVnC            |
|                                             |
|*********************************************/
typedef struct entete_TOVnC
{
    int nombre_bloc;       // nombre de bloc (ainsi que l'adresse du dernier bloc)
    int nbr_caract_insert; // nombre de caracteres inseres (nous aidera dans la reorganisation)
    int nbr_caract_supp;   // nombre de caracteres supprimes (nous aidera dans la reorganisation)

} entete_TOVnC;

/*********************************************|
|                                             |
|         Structure de bloc TOVnC             |
|                                             |
|*********************************************/
typedef struct Tbloc_TOVnC
{
    char tableau[B]; // taille variable
    int nb;          // la position libre dans le bloc

} Tbloc_TOVnC;

/*********************************************|
|                                             |
|         structure du fichier TOVnC          |
|                                             |
|*********************************************/
typedef struct fichier_TOVnC
{
    FILE *fichier;
    entete_TOVnC entete;

} fichier_TOVnC;

typedef Tbloc_TOVnC Tampon; // definition du type Tampon TOF(un alias du type tbloc)

/*********************************************|
|                                             |
|          Machine Abstraite TOVnC            |
|                                             |
|*********************************************/
void Ouvrir_TOVnC(fichier_TOVnC *f, char nom_fichier[], char mode_ouverture);
void Fermer_TOVnC(fichier_TOVnC *f);
int Entete_TOVnC(fichier_TOVnC *f, int i);
void Aff_Entete_TOVnC(fichier_TOVnC *f, int i, int val);
void EcrireDir_TOVnC(fichier_TOVnC *f, int i, Tbloc_TOVnC buf);
void LireDir_TOVnC(fichier_TOVnC *f, int i, Tbloc_TOVnC *buf);
int Alloc_bloc_TOVnC(fichier_TOVnC *f);

/*














*/
/***********************************************|
|                                               |
|          VARIABLES GLOBALES DU TP             |
|                                               |
|***********************************************/
char *MATERIAL_LIST[] = {
    "Imprimante",
    "PC Bureau",
    "PC Portable",
    "Television",
    "Telephone",
    "Projecteur"};

/***********************************************|
|                                               |
|     FONCTIONS IMPLEMENTES POUR PARTIE 01      |
|                                               |
|***********************************************/
void Chargement_initial_TOVnC(char nom_fichier[], int n);
void Generer_Chaine(char chaine[], int length, int number);
int Randomizeed_Numbers(int lower, int upper);