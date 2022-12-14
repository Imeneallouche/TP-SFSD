
/***********************************************|
|                                               |
| COPIER LA CHAINE str DANS LA CHAINE tableau   |
| A PARTIR DE LA POSITION pos                   |
|                                               |
|***********************************************/


void ins_string(char tableau[B],int pos,char str[B])                               // module à utilisé pour l'insertion 
{
    int i = 0;
    while(i < strlen(str))
    {
        tableau[pos] = str[i];
        pos++;
        i++;
    }
}


/****************************************************|
|                                                    |
|insertion d’un matériel dans le fichier de données  |           
|                                                    |
|***************************************************/





void inserer_TOVnC(char nom_fichier[],char chaine[])            //procédure pour inserer une chaine dans un fichier , la chaine represente le materiel
{                                                               

            int     i,j,                // l'emplacement ou on va inserer le matereil(i:le bloc , j: la posistion) 
                    l,k,cpt,           // des variables seront utilisés pour le decalage
                    ident,
                    trouv;            // boolean utilisé pour la recherche du materiel qu'on veut l'inserer
            int     stop=1;            // booleen pour continuer l'operation de l'insertion 
            int     taille_materiel;  // taille du materiel qu'on veut l'inserer
            int     TAILLE_chaine1;  
            char    IDENTIFIANT [TAILLE_IDENTIFIANT + 1];
            char    chaine1 [B+1],                        
                    Cle_Max[TAILLE_IDENTIFIANT + 1];
            fichier_TOVnC f;
            Tbloc_TOVnC Buf;
            


                  //mettre notre identifiant de la chaine  dans 'key' pour ensuite l'utiliser pour la recherche

            IDENTIFIANT[0]=chaine[0];
            IDENTIFIANT[1]=chaine[1];
            IDENTIFIANT[2]=chaine[2];
            IDENTIFIANT[3]=chaine[3];
            IDENTIFIANT[4]=chaine[4];
           // ident=atoi(IDENTIFIANT);                                         // taille de la cle de la chaine qu'on veut l'inserer (in case we need it )

           taille_materiel=strlen(chaine);                                   //affecter à taille_materiel la taille du materiel (chaine) qu'on va insérer
          //Ouvrir_TOVnC(&f, FICHIER_ORIGINAL , 'A');
           Recherche_TOVnC(nom_fichier , IDENTIFIANT , &trouv, &i, &j);     // on fait la recherche de la cle du materiel qu'on veut l'inserer
 
            if (!trouv){                                                   // chaine doit être inséré dans le bloc i à la position j
               while (stop && i <= Entete_TOVnC(&f, 1)) {
               LireDir_TOVnC(&f, i, &Buf);                                // on lit le bloc ou la chaine doit etre inserer
                
 /**  si l'espace libre peut contenir le materiel, on décale si nécessaire et puis on insere le materiel  **/
                if( (B - Buf.nb) >= taille_materiel ){                    // l'éspace libre est plus grand ou egale à la taille du materiel qu'on veut l'inserer
                    l= Buf.nb;
                    k= Buf.nb + taille_materiel;
                    cpt = 1;
                    while( cpt <= Buf.nb)
                    {
                         Buf.tableau[k] = Buf.tableau[l];                     // on fait le decalage des materiaux qui viennent juste apres le materiel qu'on va l'inserer 
                         k--;l--;cpt++;                              
                    }
                    ins_string(Buf.tableau,j,chaine);                      // insertion du materiel 
                    Buf.nb = Buf.nb + taille_materiel ;                   // mise à jour de la position libre dans un bloc (buf.nb)
                    EcrireDir_TOVnC(&f,i,Buf);
                    stop=0;

                }



        /** si l'espace libre ne peut pas contenir le materiel [(taille du materiel) > (B-buf.nb)] **/
           else{
            /** si la taille du materiel + position ou il faut l'inserer(j) > B , on prend le materiel qu'on veut l'inserer et les matererls qui viennent apres comme
            une nouvelle chaine à inserer **/
              if( (j + taille_materiel)>B ){
                TAILLE_chaine1 = (Buf.nb - j);                              // la taille des enregistrements qui viennent juste apres le meterile qu'on veut l'inserer     
                extraire_chaine_TOVnC(chaine1, j,TAILLE_chaine1, &Buf);     // on fait sortir ces derniers enregistrements du bloc pour les inserer avec le materiel 
                                                                            // qu'on veut l'inserer 
                Buf.nb =j;                                                  // mise à jour de la position libre dans un bloc (buf.nb)
                EcrireDir_TOVnC(&f,i,Buf); 
                strcat(chaine,chaine1);             // nouveau materiel à inserer (chaine) dans le prochain bloc = materiel qu'on veut inserer (chaine)+ les materierls
                                                   //qui viennent  apres ce dernier(chaine1)
                i= i+1, j=0;                      //  l'insertion se fera à la prochaine itération du TQ , dans le prochain bloc à la premiere position 

              }
              else {
            /** si la taille du materiel + position ou il faut l'inserer(j) =< B **/
                TAILLE_chaine1 = (Buf.nb - j);
                extraire_chaine_TOVnC(chaine1,j,TAILLE_chaine1 , &Buf);      
                ins_string(Buf.tableau,j,chaine);               // on insere le materiel
                Buf.nb = j + taille_materiel;                   //mise à jour la position libre (buf.nb)
                EcrireDir_TOVnC(&f,i,Buf);
                strcpy(chaine,chaine1);                       // le nouveau materiel qui va etre inserer dans le prochain bloc (chaine reçoit chaine1)
                 i=i+1 , j=0;   
                /*___________________________________________________________________________________________
                 REMARQUE: on a voulu laiser de vide dans le bloc pour faciliter les prochaines insertions
                ( évitant les décalages  )
                 ____________________________________________________________________________________________*/

              
            
              } 
         }
           }

    }
     
     /** si on dépasse la fin de fichier, on rajoute un nouveau bloc contenant la chaine**/
     if (i> Entete_TOVnC(&f, 1)){
        i== Alloc_bloc_TOVnC(&f);
        strcpy (Buf.tableau , chaine1);                // insertion chaine
        Buf.nb== TAILLE_chaine1;                      //mise à jour de la position libre dans un bloc (buf.nb)
        EcrireDir_TOVnC(&f,i,Buf);

     }
       Aff_Entete_TOVnC(&f, 2, Entete_TOVnC(&f,2)+taille_materiel);   // Entete_TOVnC(f,2) : nb d'insertion  , on incrémente le compteur d'insertions


 
 }
   Fermer_TOVnC(&f);
