/*********************************************************|
|                                                         |
|       structure de l'enregistrement Table d'index     |
|                                                         |
|*********************************************************/
typedef struct Tenreg_INDEX
{
    int Identifiant; // la cle unique de chaque enregistrement (identifiant) dans TOVnC
    int numbloc;        // numero du bloc de la cle unique (identifiant)  de chaque enregistrement dans TOVnC
    int deplacement;   // le deplacement la cle unique (identifiant)  de chaque enregistrement dans TOVnC
} Tenreg_INDEX;

/*********************************************|
|                                             |
|           Structure de bloc table INDEX     |
|                                             |
|*********************************************/
typedef struct Tbloc_INDEX
{
    Tenreg_INDEX tab_INDEX[MAX];    // on ajoute le MAX au lib.h
    int nombre_enreg;

} Tbloc_INDEX;

/*********************************************|
|                                             |
|    la création de la table index            |
| et du fichier index associé à cette table   |      
|                                             |
|*********************************************/
void creerIndex () {
    fichier_TOVnC F;
    char Id[TAILLE_IDENTIFIANT + 1],             // la plus petite cle dans un bloc (plus petit identifiant)   // la cle courrant dont on s'est arrete dans le parcours
         Materiel[TAILLE_MATERIEL],               // le type du materiel
         Fonctionne[TAILLE_FONCTIONNEMENT + 1],   // fonctionne = 'f', le materiel marche, fonctionne = 'n' sinon
         Prix[TAILLE_PRIX + 1],                   // le ptix du materiel
         Taille[TAILLE_TAILLE + 1],               // taille du champs description
         Description[TAILLE_MAX_DESCRIPTION + 1];  // la description (caracteristiques) du materiel
    Tbloc_INDEX IndexBuf;
    Tenreg_INDEX indexEnreg;
    Tampon_TOVnC Buf;
    int i = 1, j = 0 ,       // i :  parcours bloc par bloc fichier TOVnC ; j: parcours de position dans le bloc de fichier TOVnC
        k = 0;              // pour le parcour de la table d'index
    int i1, j1;
    Ouvrir_TOVnC(&F, FICHIER_ORIGINAL, 'A');


        while (i <= Entete_TOVnC(&F, 1))
    {
        LireDir_TOVnC(&F, i, &Buf);     // lire le fichier TOVnC
        j = 0;

       while (j < Buf.nb)
        {
            i1 = i;           // sauvgarde  num bloc
            j1 = j;           // sauvgarde position
            extraire_chaine_TOVnC(Id, &j, TAILLE_IDENTIFIANT, &Buf);
            extraire_chaine_TOVnC(Fonctionne, &j, TAILLE_FONCTIONNEMENT, &Buf);
            extraire_chaine_TOVnC(Materiel, &j, TAILLE_MATERIEL - 1, &Buf);
            extraire_chaine_TOVnC(Prix, &j, TAILLE_PRIX, &Buf);
            extraire_chaine_TOVnC(Taille, &j, TAILLE_TAILLE, &Buf);
            extraire_chaine_TOVnC(Description, &j, atoi(Taille), &Buf);

       
           
        / **************************************************************************************************************************|
        |                                                                                                                           |
        |                                   remplisage d'enregistrement index                                                       |
        |                                                                                                                           |
        |**************************************************************************************************************************/
            
            
            indexEnreg.Identifiant = atoi(Id);
            indexEnreg.numbloc = i1;
            indexEnreg.deplacement = j1;
           
           
        / **************************************************************************************************************************|
        |                                                                                                                           |
        |                                   remplisage de la table index                                                            |
        |                                                                                                                           |
        |**************************************************************************************************************************/
           
       
            IndexBuf.tab_INDEX[k++] = indexEnreg;

        }
        i++;    // passer au prochain bloc
    } 
    
    /**************************** affichage table d'index ***********************************/
    for (k = 0; k < MAX ; k++ ) {
           printf("%d  ===   ",  IndexBuf.tab_INDEX[k].Identifiant);
           printf("%d  ===   ",  IndexBuf.tab_INDEX[k].numbloc);
           printf("%d  ===   ",  IndexBuf.tab_INDEX[k].deplacement);

      }
    }
    
    // reste : fichier index ( en cours )


