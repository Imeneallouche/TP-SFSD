#include <stdio.h>
#define maxBloc 10 // max d'enregistrement dans un seul bloc
#define maxNomFichier 30
#define facteur_reorganisation 0.5

/*
GLOBAL VARIABLES AND STRUCTURES*/
typedef int typeval; // type de la valeur stockee dans l'enregistrement

/**************structure de l'enregistrement*******************/
typedef struct Tenreg // Définition de la structure d'un enregistrement
{
    int cle;        // cle unique a chaque enregistrement
    typeval valeur; // valeur stockée dans l'enregistrement
    int supprimer;  // booleen  si efface  sinon
} Tenreg;

/**************structure de bloc*******************/
typedef struct Tbloc // la definition du bloc
{
    Tenreg tab[maxBloc];
    int nombre_enreg;
} Tbloc;

/**************structure d'en_tete***************/
typedef struct entete_TOF // definition de la tete (premier bloc) du fichier TOF
{
    int blocs_total;     // nombre total des blocsdans le fichier
    int enreg_inseres;   // nombre d'enregistrements inserés
    int enreg_supprimes; // nombre d'enregistrement supprimés

} entete_TOF;

typedef Tbloc Tampon; // definition du type Tampon TOF(un alias du type tbloc)

/************structure de fichierLNOF**************/
typedef struct fichier_TOF // definition de la structure d'un fichier TOF
{
    FILE *fichier;     // pointeur vers le fichier
    entete_TOF entete; // lentete du fichier
} fichier_TOF;

/*
MACHINE ABSTRAITE*/
void Ouvrir_TOF(fichier_TOF *f, char nom_fichier[], char mode);
void Fermer_TOF(fichier_TOF f);
void LireDir_TOF(fichier_TOF f, int i, Tampon *buf, int *cpt_lect);
void EcrireDir_TOF(fichier_TOF f, int i, Tampon *buf, int *cpt_ecr);
int Entete_TOF(fichier_TOF f, int i);
void Aff_Entete_TOF(fichier_TOF *f, int i, int val);