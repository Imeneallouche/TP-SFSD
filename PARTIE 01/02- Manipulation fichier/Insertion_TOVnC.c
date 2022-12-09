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





void inserer_TOVnC(fichier_TOVnC *f,char chaine[])            //procédure pour inserer une chaine dans un fichier , la chaine represente le materiel
{

            int     i,j,l,k,ident,cpt,trouv;
            int     stop=1;
            int     taille_materiel;
            int     TAILLE_chaine1;
            char    key [TAILLE_IDENTIFIANT + 1];
            char    chaine1 [2*B],                         //on déclare un tableau assez grand afin de s'assurer qu'il peut recevoir plus de materiels que contient notre bloc
                    Cle_Max[TAILLE_IDENTIFIANT + 1];
            //fichier_TOVnC f;
            Tbloc_TOVnC Buf;
            char nom_fichier[25];
            //nom_fichier = FICHIER_ORIGINAL;


                  //mettre notre identifiant de la chaine  dans 'key' pour ensuite l'utiliser pour la recherche

             key[0]=chaine[0];
             key[1]=chaine[1];
             key[2]=chaine[2];
             key[3]=chaine[3];
             key[4]=chaine[4];
             ident=atoi(key);                                         // taille de la cle de la chaine qu'on veut l'inserer (in case we need it )

            taille_materiel=strlen(chaine);                          //affecter à taille_materiel la taille du materiel (chaine) qu'on va insérer
          //Ouvrir_TOVnC(&f, FICHIER_ORIGINAL , 'A');
           Recherche_TOVnC(nom_fichier , key , &trouv, &i, &j);     // on fait la recherche de la cle du materiel qu'on veut l'inserer
 
            if (!trouv){                                           // chaine doit être inséré dans le bloc i à la position j
               strcpy(Cle_Max, Buf.cleMax);                         // on sauvgarde la cle Max pour l'utiliser dans la mise à jour
               while (stop && i <= Entete_TOVnC(&f, 1)) {
               LireDir_TOVnC(&f, i, &Buf);
                
 /**  si l'espace libre peut contenir le materiel, on décale si nécessaire et puis on insere le materiel  **/
                if( (B - Buf.nb) >= taille_materiel ){
                    l= Buf.nb;
                    k= Buf.nb + taille_materiel;
                    cpt = 1;
                    while( cpt <= Buf.nb)
                    {
                         Buf.tableau[k] = Buf.tableau[l];
                         k--;l--;cpt++;
                    }
                    ins_string(Buf.tableau,j,chaine);
                    Buf.nb = Buf.nb + taille_materiel ;
                    EcrireDir_TOVnC(&f,i,Buf);
                     // cle_Max reste la meme
                    stop=0;

                }



        /** si l'espace libre ne peut pas contenir le materiel [(taille du materiel) > (B-buf.nb)] **/
        else{
            /** si la taille du materiel + position ou il faut l'inserer(j) > B , on prend le materiel qu'on veut l'inserer et les matererls qui viennent apres comme
            une nouvelle chaine à inserer **/
            if( (j + taille_materiel)>B ){
                TAILLE_chaine1 = (Buf.nb - j);
                extraire_chaine_TOVnC(chaine1, j,TAILLE_chaine1, &Buf);

                Buf.nb =j;
                // ***update cle max?
                EcrireDir_TOVnC(&f,i,Buf); 
                strcat(chaine,chaine1);             // nouveau materirl à inserer (chaine) dans le prochain bloc = materiel qu'on veut inserer (chaine)+ les materierls
                                                   //qui viennent  apres ce dernier(chaine1)
                i= i+1, j=0;                      //  l'insertion se fera à la prochaine itération du TQ

                 // si on dépasse la fin de fichier, on rajoute un nouveau bloc contenant un chaine//
                     if (i> Entete_TOVnC(&f, 1)){
                        i== Alloc_bloc_TOVnC(&f);
                        strcpy (Buf.tableau , chaine1);                    // insertion chaine
                        Buf.nb== TAILLE_chaine1;
                        strcpy( Buf.cleMax, Cle_Max);                    // mise à jour de cle_Max
                        EcrireDir_TOVnC(&f,i,Buf);  
                     }


            else{
            /** si la taille du materiel + position ou il faut l'inserer(j) =< B **/
                TAILLE_chaine1 = (Buf.nb - j);
                extraire_chaine_TOVnC(chaine1,j,TAILLE_chaine1 , &Buf);      
                ins_string(Buf.tableau,j,chaine);
                Buf.nb = j + taille_materiel;
                EcrireDir_TOVnC(&f,i,Buf);
                strcpy(Buf.cleMax,key );                       // mise à jour de cle_Max
                strcpy(chaine,chaine1);                       // le neouveau materiel qui va etre inserer dans  prochain bloc
                 i=i+1 , j=0;   
                /*___________________________________________________________________________________________
                 REMARQUE: on a voulu laiser de vide dans le bloc pour faciliter les prochaines insertions
                ( évitant les décalages  )
                 ____________________________________________________________________________________________*/

              }
            }

    }
     /** si on dépasse la fin de fichier, on rajoute un nouveau bloc contenant un chaine**/
     if (i> Entete_TOVnC(&f, 1)){
        i== Alloc_bloc_TOVnC(&f);
        strcpy (Buf.tableau , chaine1);               // insertion chaine
        Buf.nb== TAILLE_chaine1;
        strcpy( Buf.cleMax, Cle_Max);                // mise à jour de cle_Max
        EcrireDir_TOVnC(&f,i,Buf);

     }
       Aff_Entete_TOVnC(&f, 2, Entete_TOVnC(&f,2)+taille_materiel);   // Entete_TOVnC(f,2) : nb d'insertion  , on incrémente le compteur d'insertions


   }
 }
  Fermer_TOVnC(&f);
}

