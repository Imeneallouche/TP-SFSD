#define B 300                      // le nombre max de caractère que peut contenir le bloc
#define NB_TYPE_MATERIEL 6         // nombre de types de materiel qui existent
#define TAILLE_IDENTIFIANT 5       // la taille du champs identifiant (la cle) sur 5 octets
#define TAILLE_FONCTIONNEMENT 1    // la taille du champs qui indique si le materiel fonctionne ou pas (f-> fonctionne n-> ne fonctionne pas)
#define TAILLE_MATERIEL 12         // la taille du champ type du materiel sur 12 octets
#define TAILLE_PRIX 6              // la taille du champs prix du materiel
#define TAILLE_TAILLE 3            // la taille du champs taille (taille du champs description)
#define TAILLE_MAX_DESCRIPTION 273 // taille maximal du champs description (le nombre max de chars dans le bloc - la taille des autres champs)
#define PRIX_MAX 999999            // le prix max d'un materiel
#define MAX_ENREG 10               // max d'enregistrement dans un seul bloc (TOF)
#define MAX_NOM_FICHIER 40         // la taille max d'un nom du fichier
#define facteur_reorganisation 0.5 // le facteur de reorganisation du fichier
#define MAX_ENREG_INDEX 1000       // le nombre max d'enreg que la tabel d'index peut contenir

/***************************************************************************************************|
| Identifiant | champs fonctionne | Type materiel |    Prix   |   taille   | Description (variable) |
|  (5 bytes)  |   (1 bytes)       |  (12 bytes)   | (6 bytes) |  (3 bytes) |  (max sur 273 bytes)   |
|***************************************************************************************************/
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

typedef Tbloc_TOVnC Tampon_TOVnC; // definition du type Tampon TOF(un alias du type tbloc)

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
    int adr_dernier_bloc;       // adresse du dernier bloc / nombre de bloc -1
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
    char tableau[B]; // taille variable

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

typedef Tbloc_TOVC Tampon_TOVC; // definition du type Tampon TOF(un alias du type tbloc)

/*********************************************|
|                                             |
|            Machine Abstraite TOVC           |
|                                             |
|*********************************************/
void Ouvrir_TOVC(fichier_TOVC *f, char nom_fichier[], char mode_ouverture);
void Fermer_TOVC(fichier_TOVC *f);
int Entete_TOVC(fichier_TOVC *f, int i);
void Aff_Entete_TOVC(fichier_TOVC *f, int i, int val);
void EcrireDir_TOVC(fichier_TOVC *f, int i, Tbloc_TOVC buf);
void LireDir_TOVC(fichier_TOVC *f, int i, Tbloc_TOVC *buf);
int Alloc_bloc_TOVC(fichier_TOVC *f);

/*















*/

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
    char tab[B]; // le tableau de caracteres
    int suivant; // le numero du bloc suivant dans la liste

} Tbloc_LOVC;

typedef struct Tbloc_LOVC Tampon_LOVC; // identique a la structure du bloc en methode LOVC

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
void Ouvrir_LOVC(fichier_LOVC *f, char nom_fichier[], char mode);
void fermer_LOVC(fichier_LOVC *f);
void LireDir_LOVC(fichier_LOVC *f, int i, Tampon_LOVC *buf);
void ecrireDir_LOVC(fichier_LOVC *f, int i, Tampon_LOVC *buf);
int entete_LOVC(fichier_LOVC *f, int i);
void aff_entete_LOVC(fichier_LOVC *f, int i, int val);
int alloc_bloc_LOVC(fichier_LOVC *fichier, Tampon_LOVC *buf);

/*















*/

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
    char Identifiant[TAILLE_IDENTIFIANT + 1]; // cle unique a chaque enregistrement (identifiant)
    int Prix;                                 // le prix du materiel
    int supprimer;                            // booleen si efface ou non
} Tenreg_TOF;

/*********************************************|
|                                             |
|           Structure de bloc TOF             |
|                                             |
|*********************************************/
typedef struct Tbloc_TOF
{
    Tenreg_TOF tab[MAX_ENREG];
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

typedef Tbloc_TOF Tampon_TOF; // definition du type Tampon TOF(un alias du type tbloc)

/*********************************************|
|                                             |
|          structure du fichier TOF           |
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
void Fermer_TOF(fichier_TOF *f);
void LireDir_TOF(fichier_TOF *f, int i, Tampon_TOF *buf);
void EcrireDir_TOF(fichier_TOF *f, int i, Tampon_TOF *buf);
int Entete_TOF(fichier_TOF *f, int i);
void Aff_Entete_TOF(fichier_TOF *f, int i, int val);
int Alloc_bloc_TOF(fichier_TOF *f);

/*















*/

/*********************************************|
|                                             |
|   STRUCTURES ET VARIABLES GLOBALES de la    |
|      table d'index et fichier d'index       |
|                                             |
|*********************************************/
/*******************************************|
|                                           |
|    structure de l'enregistrement de la    |
|         table et fichier d'index          |
|                                           |
|*******************************************/
typedef struct Tenreg_INDEX
{
    char Identifiant[TAILLE_IDENTIFIANT + 1]; // l'identifiant ou la cle
    int NumBloc;                              // 1ere coordonnee (numero du bloc) de l'adresse de l'identifiant dans le fichier TOVnC
    int Deplacement;                          // 2eme coordonnee(deplacement dans loe bloc) de l'adresse de l'identifiant dans TOVnC
} Tenreg_INDEX;

/*******************************************|
|                                           |
|    Structure de la table d'index en MC    |
|                                           |
|*******************************************/
typedef struct Table_Index
{
    Tenreg_INDEX table_Index[MAX_ENREG_INDEX]; // tableau d'enregistrement d'index pas plus de MAX_ENREG_INDEX (variable globale)
    int nombre_enreg_inseres;                  // nombre d'enregistrements inseres dans la table ( < MAX_ENREG_INDEX)
} Table_Index;

Table_Index Index; // declaration d'une table d'index globale

/*********************************************|
|                                             |
|  Structure du Bloc du fichier d'index en MS |
|                                             |
|*********************************************/
typedef struct Tbloc_INDEX
{
    Tenreg_INDEX tab_INDEX[MAX_ENREG]; // max d'enreg = 10 dans un bloc de fichier TOF index
    int nombre_enreg;                  // nombre d'enregistrements inseres dans le bloc

} Tbloc_INDEX;

typedef Tbloc_INDEX Tampon_INDEX; // definition du type Tampon Index(un alias du type tbloc)

/*********************************************|
|                                             |
|    Structure d'en_tete du fichier Index     |
|                                             |
|*********************************************/
/*___________________________________________________
|   Meme structure que le fichier TOF des materiels  |
/*___________________________________________________

/*********************************************|
|                                             |
|         structure du fichier Index          |
|                                             |
|*********************************************/
/*___________________________________________________
|   Meme structure que le fichier TOF des materiels  |
/*___________________________________________________*/

/*********************************************|
|                                             |
|     Machine Abstraite FICHIER INDEX TOF     |
|                                             |
|*********************************************/
/*___________________________________________________________________________________________________________
|   Ouvrir, fermer, entete, aff_entete, alloc_bloc sont identiques a la structure TOF declaree precedement  |
/*__________________________________________________________________________________________________________*/
void LireDir_Index_TOF(fichier_TOF *f, int i, Tampon_INDEX *buf);
void EcrireDir_Index_TOF(fichier_TOF *f, int i, Tampon_INDEX *buf);
/*


















*/

/***********************************************|
|                                               |
|          VARIABLES GLOBALES DU TP             |
|                                               |
|***********************************************/
/***********************************|
|                                   |
|    liste de materiel (6 types)    |
|                                   |
|***********************************/
char *MATERIAL_LIST[] = {
    "Imprimante ",
    "PC Bureau  ",
    "PC Portable",
    "Television ",
    "Telephone  ",
    "Projecteur "};

/************************************|
|                                    |
|   noms globaux des fichiers du TP  |
|                                    |
|************************************/
char *FICHIER_ORIGINAL = "Materiel_informatique_TOVnC.bin";                        // le nom du fichier original
char *FICHIER_MATERIEL_FONCTIONNE = "Materiel_informatique_en_marche_TOVC.bin";    // le nom du fichier qui contient le materiel en fonction
char *FICHIER_MATERIEL_NON_FONCTIONNE = "Materiel_informatique_en_panne_LOVC.bin"; // le nom du fichier qui contient le materiel en panne
char *FICHIER_INDEX = "Index_Materiel_informatique_TOF.bin";                       // le nom du fichier Index de materiel informatique de type TOF

/**************************************************|
|                                                  |
|       structure pour faciliter generation        |
|     les 6 fichiers TOF des 6 types de materile   |
|                                                  |
|**************************************************/
typedef struct FICHIER_MATERIEL
{
    fichier_TOF f;                   // fichier TOF
    char file_name[MAX_NOM_FICHIER]; // le nom du fichier TOF
    char Materiel[TAILLE_MATERIEL];  // le type du materiel associe au fichier
    Tampon_TOF Buf;                  // le buffer qu'on remplira pour chaque fichier jusqu'a finir le fichier ou jusqu'a ce qu'il soit plein
    int i;                           // le numero du bloc ou l'insertion s'est arrete
    int j;                           // l'enregistrement dans bloc i ou l'insertion s'est arrete
} FICHIER_MATERIEL;

FICHIER_MATERIEL Files[NB_TYPE_MATERIEL];

/*


















*/

/***********************************************|
|                                               |
|     FONCTIONS IMPLEMENTES POUR PARTIE 01      |
|                                               |
|***********************************************/
/*******************************|
|                               |
|     FONCTIONS GLOBALES        |
|                               |
|*******************************/
void Generer_Chaine(char chaine[], int length, int number);
void concatenate(char *destination, char *identifiant, char *fonctionne, char *materiel, char *prix, char *taille, char *description);
void Demande_Information_Utilisateur(char *Fonctionnement, char *Materiel, char *Prix, char *Description);
int Random_Number(int lower, int upper);
void Random_String(int length, char chaine[]);

/*******************************|
|                               |
|       FONCTIONS TOVnC         |
|                               |
|*******************************/
// FICHIER_ORIGINAL = "Materiel_informatique_TOVnC.bin"
/***************************************************************************************************|
| Identifiant | champs fonctionne | Type materiel |    Prix   |   taille   | Description (variable) |
|  (5 bytes)  |   (1 bytes)       |  (12 bytes)   | (6 bytes) |  (3 bytes) |  (max sur 273 bytes)   |
|***************************************************************************************************/
void Ecrire_chaine_TOVnC(fichier_TOVnC *F, char chaine[], int *i, int *j, Tampon_TOVnC *Buf);
void extraire_chaine_TOVnC(char destination[], int *j, int taille, Tampon_TOVnC *Buf);
void affichage_entete_TOVnC(char nom_fichier[]);
void afficher_fichier_TOVnC(char nom_fichier[]);

void Chargement_initial_TOVnC(char nom_fichier[], int n);
void Recherche_TOVnC(char nom_fichier[], char Identifiant_Recherche[], int *trouv, int *i, int *j);
void Insertion_TOVnC(char nom_fichier[]);
void Reorganisation_TOVnC(char nom_fichier[], char nom_fichier1[], char nom_fichier2[]);
void Suppression_TOVnC(char nom_fichier[], char identifiant_a_supprimer[]);

/*******************************|
|                               |
|        FONCTIONS TOVC         |
|                               |
|*******************************/
// FICHIER_MATERIEL_FONCTIONNE = "Materiel_informatique_en_marche_TOVC.bin"
/***************************************************************************|
| Identifiant | Type materiel |   Prix    |   taille   |    Description     |
|  (5 bytes)  |   (12 bytes)  | (6 bytes) |  (3 bytes) |    (variable)      |
|***************************************************************************/
void Ecrire_Chaine_TOVC(fichier_TOVC *fichier, int *i, int *j, char chaine[], Tampon_TOVC *buf);
void extraire_chaine_TOVC(fichier_TOVC *f, char destination[], int *i, int *j, int taille, Tampon_TOVC *Buf);
void affichage_entete_TOVC(char nom_fichier[]);
void afficher_fichier_TOVC(char nom_fichier[]);

/*******************************|
|                               |
|        FONCTIONS LOVC         |
|                               |
|*******************************/
// FICHIER_MATERIEL_NON_FONCTIONNE = "Materiel_informatique_en_panne_LOVC.bin"
/***************************************************************************|
| Identifiant | Type materiel |   Prix    |   taille   |    Description     |
|  (5 bytes)  |   (12 bytes)  | (6 bytes) |  (3 bytes) |    (variable)      |
|***************************************************************************/
void Ecrire_Chaine_LOVC(fichier_LOVC *fichier, int *i, int *j, char chaine[], Tampon_LOVC *buf);
void extraire_chaine_LOVC(fichier_LOVC *f, char destination[], int *i, int *j, int taille, Tampon_LOVC *Buf);
void affichage_entete_LOVC(char nom_fichier[]);
void afficher_fichier_LOVC(char nom_fichier[]);

/*









*/

/***********************************************|
|                                               |
|     FONCTIONS IMPLEMENTES POUR PARTIE 02      |
|                                               |
|***********************************************/
/*******************************|
|                               |
|         FONCTIONS TOF         |
|                               |
|*******************************/
/************************************************|
|  Identifiant  |     Prix    |     Supprimer    |
|   (5 bytes)   |  (integer)  |     (integer)    |
|************************************************/
void Generation_fichiers_Materiel(char nom_fichier[]);
void Inserer_Enreg_TOF(fichier_TOF *f, Tenreg_TOF Enregistrement_TOF, int *i, int *j, Tampon_TOF *Buf);
void affichage_entete_TOF(char nom_fichier[]);
void afficher_fichier_TOF(char nom_fichier[]);

void Creer_Index(char nom_fichier_TOVnC[], char nom_fichier_Index[]);
void Afficher_Table_Index(Table_Index Index);
void Recherche_Dichotomique_Table_Index_TOF(char Cle[], int *trouv, int *k);
void Insertion_Table_Index(Tenreg_INDEX enregistrement_index, int k);
void Chargement_Table_Index_TOF(char nom_fichier_index[], Table_Index *Index);
void Sauvegarde_Table_Index_TOF(char nom_fichier_index[], Table_Index Index);

/*******************************|
|                               |
|        FONCTIONS LOVC         |
|                               |
|*******************************/
// FICHIER_MATERIEL_NON_FONCTIONNE = "Materiel_informatique_en_panne_LOVC.bin"
/***************************************************************************|
| Identifiant | Type materiel |   Prix    |   taille   |    Description     |
|  (5 bytes)  |   (12 bytes)  | (6 bytes) |  (3 bytes) |    (variable)      |
|***************************************************************************/
void Requette_intervalle_LOVC(char nom_fichier[], int Prix_Min, int Prix_Max, int *montant);

/*******************************|
|                               |
|       FONCTIONS TOVnC         |
|                               |
|*******************************/
// FICHIER_ORIGINAL = "Materiel_informatique_TOVnC.bin"
void Insertion_TnOVnC(char nom_fichier[]);