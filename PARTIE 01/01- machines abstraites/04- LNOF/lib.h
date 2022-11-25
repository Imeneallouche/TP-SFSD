#include <stdio.h>
#define maxBloc 10
#define maxNomFichier 30
#define facteur_reorganisation 0.5

/**********************************************|
|                                              |
|    STRUCTURES ET VARIABLES GLOBALES LnOF     |
|                                              |
|*********************************************/
typedef int typeval_LnOF; // le type de la valeur stockée dans l'enregistrement

/*********************************************|
|                                             |
|      Structure de l'enregistrement LnOF     |
|                                             |
|*********************************************/
typedef struct Tenreg_LnOF
{
    int cle;             // la cle
    int supprimer;       // le champs efface
    typeval_LnOF valeur; // la valeurstockee dans l'neregistrement
} Tenreg_LnOF;

/*********************************************|
|                                             |
|           Structure de bloc LnOF            |
|                                             |
|*********************************************/
typedef struct bloc_LnOF
{
    Tenreg_LnOF tab[maxBloc]; // tableau d'enregistrement
    int nombre_enreg;         // le nombre d'enregistrement dans le Bloc
    int suivant;              // le numero (l'adresse)du prochain bloc
} TBloc_LnOF;

/*********************************************|
|                                             |
|          Structure d'en_tete LnOF           |
|                                             |
|*********************************************/
typedef struct entete_LNOF
{
    int num_Bloc_entete;  // le numero(adresse) du bloc entete(premier)
    int enreg_inseres;    // Le nombre d'enregistrement inserés
    int enreg_supprimes;  // le nombre d'enregistrements suprimés
    int num_dernier_bloc; // le numero (adresse)du dernier bloc
} entete_LNOF;            // declaration du bloc en_tete

typedef TBloc_LnOF Tampon; // definition du type Tampon LNOF(un alias du type TBloc_LnOF)

/*********************************************|
|                                             |
|         structure de fichier LnOF           |
|                                             |
|*********************************************/
typedef struct fichier_LNOF
{
    FILE *fichier;        // declarer le fichier
    entete_LNOF *en_tete; // declarer l'en_tete
} fichier_LNOF;           // declaration de la structure LNOF

/*********************************************|
|                                             |
|            Machine Abstraite LnOF            |
|                                             |
|*********************************************/
void Ouvrir_LnOF(fichier_LNOF *fichier, char nom_fichier[], const char mode);
void Fermer_LnOF(fichier_LNOF *fichier);
void EcrireDir_LnOF(fichier_LNOF *fichier, int i, TBloc_LnOF *buf, int *cpt_ecr);
void LireDir_LnOF(fichier_LNOF *fichier, int i, TBloc_LnOF *buf, int *cpt_lect);
void aff_entete_LnOF(fichier_LNOF *fichier, int num_caract, int val);
int entete_LnOF(fichier_LNOF *fichier, int num_caract);
int alloc_bloc_LnOF(fichier_LNOF *fichier, char nom_fichier[], TBloc_LnOF *buf);