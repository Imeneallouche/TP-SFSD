#include <stdio.h>
#define maxBloc 10
#define maxNomFichier 30
#define facteur_reorganisation 0.5

/*









GLOBAL VARIABLES AND STRUCTURES*/

typedef int typeval; // le type de la valeur stockée dans l'enregistrement

/**************structure de l'enregistrement*******************/
typedef struct Lenreg
{
    int cle;        // la cle
    int supprimer;  // le champs efface
    typeval valeur; // la valeurstockee dans l'neregistrement
} Lenreg;           // la declaration de l'article

/**************structure de bloc*******************/
typedef struct Lbloc
{
    Lenreg tab[maxBloc]; // tableau d'enregistrement
    int nombre_enreg;    // le nombre d'enregistrement dans le Bloc
    int suivant;         // le numero (l'adresse)du prochain bloc
} LBloc;                 // la declaration du bloc

/**************structure d'en_tete***************/
typedef struct entete_LNOF
{
    int num_Bloc_entete;  // le numero(adresse) du bloc entete(premier)
    int enreg_inseres;    // Le nombre d'enregistrement inserés
    int enreg_supprimes;  // le nombre d'enregistrements suprimés
    int num_dernier_bloc; // le numero (adresse)du dernier bloc
} entete_LNOF;            // declaration du bloc en_tete

// typedef Lbloc Tampon; // definition du type Tampon LNOF(un alias du type Lbloc)

/************structure de fichierLNOF**************/
typedef struct fichier_LNOF
{
    FILE *fichier;        // declarer le fichier
    entete_LNOF *en_tete; // declarer l'en_tete
} fichier_LNOF;           // declaration de la structure LNOF

/*













MACHINE ABSTRAITE*/
void Ouvrir_LnOF(fichier_LNOF *fichier, char nom_fichier[], const char mode);
void Fermer_LnOF(fichier_LNOF *fichier);
void EcrireDir_LnOF(fichier_LNOF *fichier, int i, LBloc *buf, int *cpt_ecr);
void LireDir_LnOF(fichier_LNOF *fichier, int i, LBloc *buf, int *cpt_lect);
void aff_entete_LnOF(fichier_LNOF *fichier, int num_caract, int val);
int entete_LnOF(fichier_LNOF *fichier, int num_caract);
void alloc_bloc_LnOF(fichier_LNOF *fichier, char nom_fichier[], LBloc *buf);

// FONCTIONS DEDIEES A LA STRUCTURES LnOF
void Recherche_LnOF(int cle, char nom_fichier[], int *trouve, int *i, int *j, int cpt_lect);
void insertion_LnOF(Lenreg *n, char nom_fichier[], int *cpt_lect, int *cpt_ecr);
void Recherche_position_TnoF(char nom_fichier[], int *i, int *j, int *trouv, int *cpt_lect);
void Reorganisation_LnOF();