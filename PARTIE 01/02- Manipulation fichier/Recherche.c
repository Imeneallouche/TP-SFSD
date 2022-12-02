/****************************************************|
|                                                   |
|   Recherche dun materiel selon son identifiant    |
|                                                   |
|****************************************************/


void Recheche (fichier_TOVnc *f ,int clerecherch , int i,j )
{
    int binf,bsup,cle1 ,cle2 , cle;
    bool trouve;
    char taille,eff,clecourante;
    Tbloc_TOVnC buff;
    binf=1;                                                   //le premier bloc
    bsup=Entete_TOVnC(f,1) ;                                   //le dernier bloc
    while (!trouve && inf <= sup)                             //enregistrement non trouvé et recherche possible
    {
        i = (binf+bsup)/2;                                     //numero de bloc a parcourir
        j=1;

        LireDir_TOVnC(fichier_TOVnC *f, int i, Tbloc_TOVnC *buf);

        cle1=Generer_Chaine(buff.tableau[j+4..j+8]);                 // Generer_Chaine est une fonction pour faire la conversion,
        cle2=Generer_Chaine(buff.cleMax);                         //d'une chaine contenant un nombre vers l'entier qu'elle contienne
        if ( clerecherch>=cle1 && clerecherch<=cle2)             //si la cle à recherchee est entre cle1 et cle2 du bloc on fait le recherche sequentielle dans le bloc
            {
            	
                while (!trouve && j!= buff.pos_libre )
                {
                    taille = buf.tableau[j..j+2]; 				//récupérer un tableau de 3 caractères, la taille d'enregistrement
	            eff = buf.tab[j+3];              			       // récupérer un caractère d'effacement
		    clecourante =buf.tableau[j+4..j+8];           	     //récupérer 5 caractères:la taille du champ identifiant (la cle) sur 5 octets
                    cle=Generer_Chaine(clecourante);
                    if ( clerecherch == clecourante && !eff)               //la cle st donc trouveée dans le bloc i
                    {
                        trouv =Vrai;                           
                    }
                    else
                    {
                        j = j + taille;                                 //passer vers l'enregistrement suivant
                    }
                }
            }
        else
        {
            if (clerecherch<cle1)                                     //la clé n'est pas dans ce bloc
            {
                bsup=i-1;                                            //la clé doit être avant le bloc courant
            }
            else
            {
                binf=i+1;                                             ///clé  doit être après le bloc courant
            }
        }


    }

    if (!trouve)
    {
        printf("\nEnregistrement n'existe pas!, vous devez l'inserez si vous voulez!\n");
    }
    else
    {
        printf("\n L'enregistrement est dans le bloc %d a la position %d",i,j);
        Affichage_E(f,i,j);
    }
}



/****************************************************|
|                                                   |
|        Affichage d'un enregistrement              |
|                                                   |
|****************************************************/
void Affichage_E(TOVnC_fichier *f,int i,j)
{
    printf("\n Les informations du materiel:  ----------------------------------------\n\n");
    printf(" -> L'identifiant : %s\n",   buf.tableau[j+4..j+8);
    printf(" -> Le type matériel : %s\n",buf.tableau[j+9..j+20]);
    printf(" -> Fonctionnement du materiel: %c\n", buf.tableau[j+21]);
    printf(" -> Le prix d’achat du matériel : %s\n", buf.tableau[j+22..j+27]);
    printf(" -> La Description : %s\n",buf.tableau[j+28..j+299]);
}
// Eregistrement = | longueur de l'info|   eff      | clé(identifiant) | l'information (from j+4 TO j+300) *cle est inclue dans l'information* | 
//                 |    (3 bits)       |  (1 bit)   |     (5bits)      |               (296bits)                   |

