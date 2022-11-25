#define max_caractere 300 // le nombre max de caractère que peut contenir le bloc

/*














/************************************************************
|                                                           |
|       Declaration de la structure de fichier TOVC         |
|                                                           |
|***********************************************************/

/// structure de l'entete ------------------------------------------------------------------------------------------------------
typedef struct S_Entete
{
    int adr_dernier_bloc;       /// adresse du dernier bloc, pas besoin du 1er bloc puisque la structure est un tableau => contig�e
    int pos_libre_dernier_bloc; /// necessaire aux decalages pour l'insertion ordonnee
    int nbr_caract_insert;      /// necessaire pour la comparaison avec nbr caracteres supprimes
    int nbr_caract_supp;        /// si > 50% => reorganisation du fichier
} S_Entete;
///-----------------------------------------------------------------------------------------------------------------------------

/// structure du bloc ----------------------------------------------------------------------------------------------------------
typedef struct Tbloc
{
    char tableau[max_caractere + 1]; /// puisque taille variable => (bloc = tableau de caract�re)
} Tbloc;
///-----------------------------------------------------------------------------------------------------------------------------

/// structure du fichier de type TOVC ------------------------------------------------------------------------------------------
typedef struct TOVC
{
    FILE *f;
    S_Entete entete;
} TOVC;
///-----------------------------------------------------------------------------------------------------------------------------

//// structures du semi enregistrement ------------------------------------------------------------------------------------------
typedef char Semi_enreg[max_caractere + 1]; //// chaine de caract�re qui contiendra l'information pour r�cup_chaine et �crire_chaine
typedef struct Enregistrement
{
    //// longueur de l'info => 3 caract�res
    int cle;  /// 3 caract�res
    int supp; /// booleen : 3 caract�res
    char info[max_caractere - 6];
    /// la taille max de l'enregistrement est le bloc. ( 6 = taille longueur + taille cl� + taille supp + 1)
} Enregistrement;
///----------------------------------------------------------------------------------------------------------------------------

/*














/************************************************************
|                                                           |
|       Les entetes des fonctions utilisees pour TOVC       |
|                                                           |
|***********************************************************/

int Entete(TOVC *fichier, int i);
void Aff_Entete(TOVC *fichier, int i, int val);
TOVC *Ouvrir(char nom_fichier[30], char mode_ouverture);
void Fermer(TOVC *fichier);
void Ecriredir(TOVC *fichier, int i, Tbloc buf);
void Liredir(TOVC *fichier, int i, Tbloc *buf);
void Alloc_bloc(TOVC *fichier);
void num_chaine(int num, int max, char *s);
void copier_chaine(char *s, int i, int max, char *r);
void couper_chaine(char *s, int i, int max);
void semi_to_enreg(Semi_enreg se, Enregistrement *en);
void enreg_to_semi(Enregistrement en, Semi_enreg se);
void recuperer_se(TOVC *fichier, int *i, int *j, Semi_enreg se);
/*














/************************************************************
|                                                           |
|     Les entetes des fonctions d'affichage pour lOVC       |
|                                                           |
|***********************************************************/

void Affichage_fichier(TOVC *fichier);
void Affichage_Entete(TOVC *fichier);
void Affichage_bloc(TOVC *fichier);
void Affichage_chevauchement(TOVC *fichier);

/*















/************************************************************
|                                                           |
|       Declaration de la structure de fichier TOVC         |
|                                                           |
|***********************************************************/
/// manipulation du fichier ---------------------------------------------------------------------------------------------------
void Recherche_TOVC(TOVC *fichier, int cle, int *i, int *j, int *trouv);
void Suppression_logique(TOVC *fichier, int cle);
void Suppression_physique(TOVC *fichier, int cle); /// big probleme
void insertion_pos_rec(TOVC *fichier, int *i, int *j, Semi_enreg SE);
void insertion_TOVC(TOVC *fichier, Enregistrement E);
void reorganisation_TOVC(TOVC *fichier, char *nouveau_fichier);