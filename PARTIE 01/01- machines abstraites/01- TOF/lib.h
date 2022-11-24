#include <stdio.h>
#define maxBloc 10 // max d'enregistrement dans un seul bloc
#define maxNomFichier 30
#define facteur_reorganisation 0.5

/*********************************************|
|                                             |
|   STRUCTURES ET VARIABLES GLOBALES TOF      |
|                                             |
|*********************************************/
typedef int typeval_TOF; // type de la valeur stockee dans l'enregistrement

/*********************************************|
|                                             |
|       structure de l'enregistrement TOF     |
|                                             |
|*********************************************/
typedef struct Tenreg_TOF
{
    int cle;        // cle unique a chaque enregistrement
    typeval valeur; // valeur stockée dans l'enregistrement
    int supprimer;  // booleen  si efface  sinon
} Tenreg_TOF;

/*********************************************|
|                                             |
|           Structure de bloc TOF             |
|                                             |
|*********************************************/
typedef struct Tbloc_TOF
{
    Tenreg tab[maxBloc];
    int nombre_enreg;

} Tbloc_TOF;

/*********************************************|
|                                             |
|          Structure d'en_tete TOF            |
|                                             |
|*********************************************/
typedef struct entete_TOF
{
    int blocs_total;     // nombre total des blocsdans le fichier
    int enreg_inseres;   // nombre d'enregistrements inserés
    int enreg_supprimes; // nombre d'enregistrement supprimés

} entete_TOF;

typedef Tbloc_TOF Tampon; // definition du type Tampon TOF(un alias du type tbloc)

/*********************************************|
|                                             |
|          structure de fichier TOF           |
|                                             |
|*********************************************/
typedef struct fichier_TOF
{
    FILE *fichier;     // pointeur vers le fichier
    entete_TOF entete; // lentete du fichier

} fichier_TOF;

/*********************************************|
|                                             |
|            Machine Abstraite TOF            |
|                                             |
|*********************************************/
void Ouvrir_TOF(fichier_TOF *f, char nom_fichier[], char mode);
void Fermer_TOF(fichier_TOF f);
void LireDir_TOF(fichier_TOF f, int i, Tampon *buf, int *cpt_lect);
void EcrireDir_TOF(fichier_TOF f, int i, Tampon *buf, int *cpt_ecr);
int Entete_TOF(fichier_TOF f, int i);
void Aff_Entete_TOF(fichier_TOF *f, int i, int val);