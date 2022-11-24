#include <stdio.h>
#define maxBloc 10 // max d'enregistrement dans un seul bloc

/*
GLOBAL VARIABLES AND STRUCTURES*/
typedef int typeval; // type de la valeur stockee dans l'enregistrement

/**************structure de bloc*******************/
typedef struct Tbloc // la definition du bloc
{
    Tenreg tab[maxBloc];
    int nombre_enreg;
} Tbloc;

/**************structure de l'enregistrement*******************/
typedef struct Tenreg // Définition de la structure d'un enregistrement
{
    int cle;        // cle unique a chaque enregistrement
    typeval valeur; // valeur stockée dans l'enregistrement
    int supprimer;  // booleen  si efface  sinon

} Tenreg;

/**************structure d'en_tete***************/
typedef struct entete_TnOF // definition de la tete (premier bloc) du fichier TOF
{
    int blocs_total;     // nombre total des blocsdans le fichier
    int enreg_inseres;   // nombre d'enregistrements inserés
    int enreg_supprimes; // nombre d'enregistrement supprimés

} entete_TnOF;

typedef Tbloc Tampon; // definition du type Tampon TnOF(un alias du type tbloc)

/************structure de fichierLNOF**************/
typedef struct fichier_TnOF // definition de la structure d'un fichier TOF
{
    FILE *fichier;      // pointeur vers le fichier
    entete_TnOF entete; // lentete du fichier
} fichier_TnOF;

/*
MACHINE ABSTRAITE*/
void Ouvrir(fichier_TnOF *f, char nom_fichier[20], char mode);
void Fermer(fichier_TnOF f);
void LireDir(fichier_TnOF f, int i, Tampon *buf, int *cpt_lect);
void EcrireDir(fichier_TnOF f, int i, Tampon *buf, int *cpt_ecr);
int Entete(fichier_TnOF f, int i);
void Aff_Entete(fichier_TnOF *f, int i, int val);

// FPNCTIONS SPECIALS POUR LA STRUCUTRES
void Recherche_TnOF(int cle, char nom_fichier[], int *found, int *i, int *j, int *cpt_lect);
void Chargement_initial_TnOF(char nom_fichier[], int n, float u);
void Insertion_TnOF(Tenreg e, char nom_fichier[], double *fact, int *cpt_lect, int *cpt_ecr);

// FONCTIONS GENERALS HORS DE LA STRUCTURES
int extract_nbr(int nbr);
