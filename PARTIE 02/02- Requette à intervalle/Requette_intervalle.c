/**************************************************|
|                                                  |
|    extraire une chaine de taille "taille" dans   |
|      la chaine destination a partir pos j        |
|                                                  |
|**************************************************/
void extraire_chaine_LOVC(fichier_LOVC *f2 , char destination[], int *i,int *j, int taille, Tampon_LOVC *buf)
{
    int k = 0;
    sprintf(destination, "%s", "");
    for (k = 0; k < taille; k++)             // boucle de taille iterations correspondant a la longueur de la chaine
    {                                        // indice de parcours de la chaine resultata et j celui du tableau
      if((*j)<=B)  {  
        destination[k] = buf->tab[(*j)]; // recuperation du caractere dans la position k de la chaine
        (*j)++;                              // deplacement d'une position dans le buffer
    }                                        // fin de boucle
                    
    
    else                             // si le k ièeme caractère a recuperer sort du buffer
        {
            (*i)=buf->suivant;                      // on passe au suivant du bloc i 
            LireDir_LOVC(f2 ,*i, &buf) ;  // lecture
            destination[k]=buf->tab[0];       // recuperation du premier caractère du tableau dans la position k de la chaine
            (*j)=1;                      //positionnement eu second caractère dans le nouveau buffer
        }
    }
    destination[k] = '\0';                        // fin de la chaine obtenue
}





/**************************** FICHIER_MATERIEL_NON_FONCTIONNE = "Materiel_informatique_en_panne_LOVC.bin"*******************************************/


/*********************************************************************************|
| Identifiant  | Type materiel |    Prix   |   taille   | Description (variable) |
|  (5 bytes)   |  (12 bytes)   | (6 bytes) |  (3 bytes) |  (max sur 272 bytes)   |
|*********************************************************************************/


/***************************************************************************************************************************************************/

void Recherche_LOVC(char nom_fichier2[], int Prix_Min , int Prix_Max , int *montant)
{
    fichier_LOVC f2;
    Ouvrir_LOVC(&f2, nom_fichier2, 'A');
    int suivant,
        j,i,
        compteur=0;                                                                             // pour le calcul du montant

    char Identifiant[TAILLE_IDENTIFIANT +1],               // la plus petite cle dans un bloc (plus petit identifiant)
         Materiel[TAILLE_MATERIEL+1],                     // le type du materiel
         Prix[TAILLE_PRIX + 1],                          // le ptix du materiel
         Taille[TAILLE_TAILLE + 1],                     // taille du champs description
         Description[TAILLE_MAX_DESCRIPTION + 1];      // la description (caracteristiques) du materiel
    Tampon_LOVC buf;                                  // un buffer pour charger un bloc de MS vers MC
    
    i = entete_LOVC(&f2, 1)  ;                        // le numero (adresse)du dernier bloc
                                           
    while ( (i != -1)  && (buf.suivant != -1) )
    {


        LireDir_LOVC(&f2, i, &buf);                                                 // lire le buffer
        for (j = 0; j <=  entete_LOVC(&f2, 3)  ; j++ ) {                            // entete_LOVC(f, 3) represente la position libre dans le dernier bloc "pos_libre_dernier_bloc"
            extraire_chaine_LOVC(&f2 ,Identifiant,&i, &j, TAILLE_IDENTIFIANT, &buf);                      // extraire IDENTIFIANT
            extraire_chaine_LOVC(&f2 ,Materiel,&i, &j, TAILLE_MATERIEL - 1, &buf);                        // extraire le materiel
            extraire_chaine_LOVC(&f2 ,Prix, &i, &j, TAILLE_PRIX, &buf);                                   // extraire son prix
            extraire_chaine_LOVC(&f2 ,Taille,&i, &j, TAILLE_TAILLE, &buf);                              // extraire la taille de la description
            extraire_chaine_LOVC(&f2 ,Description, &i, &j, atoi(Taille), &buf);                         // extraire la description
            if ( Prix_Min <= atoi(Prix)  &&  atoi(Prix) <= Prix_Max){                         //  le pris doit etre compris entre ces deux valeurs : Pris_Max et Pris_Min

                    
                     compteur += atoi(Prix);

                        printf("\n\n----------------  les informations du matériel compris entre les deux valeurs que vous avez proposer: ------------\n\n");
                        printf("|    -> L'identifiant : %s\n", Identifiant);                 // afficher la cle
                        printf("|    -> Le type materiel : %s\n", Materiel);                   // afficher le materiel
                        printf("|    -> Le prix d'achat du materiel : %s\n", Prix);            // afficher le prix
                        printf("|    -> La Description : %s\n", Description);                  // afficher la description
                        printf("---------------------------------------------------------------------------------------------\n\n");
            }
             i=buf.suivant;
        }

         printf("|    -> le montant global est : %d\n", compteur);                 //  le montant global de tous ces  matériels affichés

    }
     fermer_LOVC(&f2);
}






 
