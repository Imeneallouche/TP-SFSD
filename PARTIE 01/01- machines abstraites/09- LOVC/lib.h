#include <stdio.h>
#define TAILLE 1024                                      // La taille d'un bloc en octets
#define MAX2 ((TAILLE - 2 * sizeof(int)) / sizeof(char)) // le nombre de caracteres maximale qu'un bloc peut contenir
#define u 90                                             // pourcentage (%) de remplissement des blocs lors du chargement initial
#define champs_cle 5                                     // la taille du champs cle
#define champs_efface 1                                  // la taille du champs efface
#define champs_taille 3                                  // la taille du champs taille
#define champs_valeur 50

/*









/************************************************************
|                                                           |
|       Declaration de la structure de fichier LOVC         |
|                                                           |
|***********************************************************/

typedef struct Tbloc_LOVC
{
    char tab[MAX2]; // le tableau de caracteres
    int suivant;    // le numero du bloc suivant dans la liste
    int nb;         // le nombre de caracteres occupes dans le bloc (effaces inclus)
                    // correspend ainsi au premier indice libre dans le bloc
} Tbloc_LOVC;

typedef struct Tbloc_LOVC Buffer_LOVC; // identique a la structure du bloc en methode LOVC

typedef struct Entete_LOVC
{
    int Num_premier_bloc;       // le numéro(adresse) du premier bloc
    int pos_libre_dernier_bloc; // la position libre dans le dernier bloc
    int nb_chars_inseres;       // nombre de caractères inserés
    int nb_chars_supprimes;     // nombre de caractères supprimées
} Entete_LOVC;

typedef struct fichier_LOVC
{
    FILE *fichier;
    Entete_LOVC entete;
} fichier_LOVC;

/*















/************************************************************
|                                                           |
|       Les entetes des fonctions utilisees pour lOVC       |
|                                                           |
 ***********************************************************/

// ouvrir un fichier de nom "nomFichier" en mode 'a' ou 'n' et retourne
// un pointeur vers la structure ou NULL si l'ouverture a echoue
// 'a' : ouvrir un fichier qui existe deja en lecture/ecriture
// 'n' : creer un nouveau fichier en lecture/ecriture
void Ouvrir_LOVC(fichier_LOVC *f, const char nomFichier[], char mode);

// fermer le fichier en sauvegardant l'entete
void fermer_LOVC(fichier_LOVC *f);

// lire dans le buffer "buf" le contenu du bloc numero "i" du fichier "f" et incrementer le nombre de lecture (cpt_lect)
void LireDir_LOVC(fichier_LOVC *f, int i, Buffer_LOVC *buf, int *cpt_lect);

// ecrire le contenu du buffer "buf" dans le bloc numero "i" du fichier "f" et incrementer le nombre d'ecriture
void ecrireDir_LOVC(fichier_LOVC *f, int i, Buffer_LOVC *buf, int *cpt_ecr);

// retourner la valeur de la 'i'eme caracteristique du fichier 'f'
int entete_LOVC(fichier_LOVC *f, int i);

// affecter la valeur 'val' dans la 'i'eme caracteristique du fichier 'f'
void aff_entete_LOVC(fichier_LOVC *f, int i, int val);

// alloue un nouveau bloc au fichier et retourne son numero
int alloc_bloc_LOVC(fichier_LOVC *f);

/*
















/*****************************************************************************
|                                                                            |
|      Prototypes de quelques modules necessaires pour manipuler LOVC        |
|                                                                            |
*****************************************************************************/

// fonction qui permet de recuperer une chaine de longueur
//  n dans le bloc i  lu dans buffer a partir de la position j//
void recuperer_chaine(fichier_LOVC *fichier, int n, int *i, int *j, char chaine[], Buffer_LOVC *buf);

/*





















/***********************************************************************
 |       Prototypes des modules demandes dans la partie 2 du TP        |
 ***********************************************************************/

// rechercher la cle dans un fichier de nom donne et retourner le numero du bloc i et e deplacement ou il est ou devait exister
void recherche_LOVC(char nom_fichier[], int cle, int *trouv, int *i, int *j, int *cpt_lect);