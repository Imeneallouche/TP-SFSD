#define B 300                      // le nombre max de caractère que peut contenir le bloc
#define NB_TYPE_MATERIEL 6         // nombre de types de materiel qui existent
#define TAILLE_IDENTIFIANT 5       // la taille du champs identifiant (la cle) sur 5 octets
#define TAILLE_MATERIEL 12         // la taille du champ type du materiel sur 12 octets
#define TAILLE_FONCTIONNEMENT 1    // la taille du champs qui indique si le materiel fonctionne ou pas
#define TAILLE_PRIX 6              // la taille du champs prix du materiel
#define TAILLE_TAILLE 3            // la taille du champs taille (taille du champs description)
#define TAILLE_MAX_DESCRIPTION 273 // taille maximal du champs description (le nombre max de chars dans le bloc - la taille des autres champs)
#define PRIX_MAX 999999            // le prix max d'un materiel
#define maxBloc 10                 // max d'enregistrement dans un seul bloc (TOF)
#define maxNomFichier 30           // la taille max d'un nom du fichier
#define facteur_reorganisation 0.5 // le facteur de reorganisation du fichier

/*******************************************************************************************|
| Identifiant| Type materiel | fonctionne |    Prix   |   taille   | Description (variable) |
|  (5 bytes) |  (12 bytes)   | (1 bytes)  | (6 bytes) |  (3 bytes) |   (max sur 273 bits)   |
|*******************************************************************************************/

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
    char tableau[B];                 // taille variable
    int nb;                          // la position libre dans le bloc
    char cleMax[TAILLE_IDENTIFIANT]; // la cle max dans le bloc afin de pouvoir faire une recherche dichotomique dans le bloc

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
    int cle;            // cle unique a chaque enregistrement
    typeval_TOF valeur; // valeur stockée dans l'enregistrement
    int supprimer;      // booleen  si efface  sinon
} Tenreg_TOF;

/*********************************************|
|                                             |
|           Structure de bloc TOF             |
|                                             |
|*********************************************/
typedef struct Tbloc_TOF
{
    Tenreg_TOF tab[maxBloc];
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
void Fermer_TOF(fichier_TOF f);
void LireDir_TOF(fichier_TOF f, int i, Tampon_TOF *buf, int *cpt_lect);
void EcrireDir_TOF(fichier_TOF f, int i, Tampon_TOF *buf, int *cpt_ecr);
int Entete_TOF(fichier_TOF f, int i);
void Aff_Entete_TOF(fichier_TOF *f, int i, int val);
int Alloc_bloc_TOF(fichier_TOF *f);

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
void Ouvrir_LOVC(fichier_LOVC *f, const char nomFichier[], char mode);
void fermer_LOVC(fichier_LOVC *f);
void LireDir_LOVC(fichier_LOVC *f, int i, Tampon_LOVC *buf, int *cpt_lect);
void ecrireDir_LOVC(fichier_LOVC *f, int i, Tampon_LOVC *buf, int *cpt_ecr);
int entete_LOVC(fichier_LOVC *f, int i);
void aff_entete_LOVC(fichier_LOVC *f, int i, int val);
int alloc_bloc_LOVC(fichier_LOVC *fichier, int *cpt_lect, int *cpt_ecr, Tampon_LOVC *buf);

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

char *FICHIER_ORIGINAL = "Materiel_informatique_TOVnC.bin"; // le nom du fichier original

/***********************************************|
|                                               |
|     FONCTIONS IMPLEMENTES POUR PARTIE 01      |
|                                               |
|***********************************************/
void Chargement_initial_TOVnC(char nom_fichier[], int n);
void Generer_Chaine(char chaine[], int length, int number);
int Randomizeed_Numbers(int lower, int upper);
