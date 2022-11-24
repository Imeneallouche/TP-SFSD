#include <stdio.h>
#define maxBloc 10 // max d'enregistrement dans un seul bloc

/**********************************************|
|                                              |
|   STRUCTURES ET VARIABLES GLOBALES TnOF      |
|                                              |
|**********************************************/
typedef int typeval_TnOF; // type de la valeur stockee dans l'enregistrement

/**********************************************|
|                                              |
|       structure de l'enregistrement TnOF     |
|                                              |
|*********************************************/
typedef struct Tenreg_TnOF
{
    int cle;             // cle unique a chaque enregistrement
    typeval_TnOF valeur; // valeur stockée dans l'enregistrement
    int supprimer;       // booleen  si efface  sinon

} Tenreg_TnOF;

/**********************************************|
|                                              |
|           Structure de bloc TnOF             |
|                                              |
|**********************************************/
typedef struct Tbloc_TnOF
{
    Tenreg_TnOF tab[maxBloc];
    int nombre_enreg;

} Tbloc_TnOF;

/*********************************************|
|                                             |
|         Structure d'en_tete TnOF            |
|                                             |
|*********************************************/
typedef struct entete_TnOF
{
    int blocs_total;     // nombre total des blocsdans le fichier
    int enreg_inseres;   // nombre d'enregistrements inserés
    int enreg_supprimes; // nombre d'enregistrement supprimés

} entete_TnOF;

typedef Tbloc_TnOF Tampon; // definition du type Tampon TnOF(un alias du type tbloc)

/*********************************************|
|                                             |
|         structure de fichier TnOF           |
|                                             |
|*********************************************/
typedef struct fichier_TnOF
{
    FILE *fichier;      // pointeur vers le fichier
    entete_TnOF entete; // lentete du fichier

} fichier_TnOF;

/*********************************************|
|                                             |
|           Machine Abstraite TnOF            |
|                                             |
|*********************************************/
void Ouvrir_TnOF(fichier_TnOF *f, char nom_fichier[20], char mode);
void Fermer_TnOF(fichier_TnOF f);
void LireDir_TnOF(fichier_TnOF f, int i, Tampon *buf, int *cpt_lect);
void EcrireDir_TnOF(fichier_TnOF f, int i, Tampon *buf, int *cpt_ecr);
int Entete_TnOF(fichier_TnOF f, int i);
void Aff_Entete_TnOF(fichier_TnOF *f, int i, int val);
int Alloc_bloc_TOF(fichier_TnOF f)