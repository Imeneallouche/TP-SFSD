/****************************************************|
|                                                    |
|   Recherche dun materiel selon son identifiant     |
|                                                    |
|****************************************************/
void Recherche_TOVnC(char nom_fichier[], char Identifiant_Recherche[], int *trouv, int *i, int *j)
{
    fichier_TOVnC f;
    Ouvrir_TOVnC(&f, nom_fichier, 'A');
    int binf = 1,                                                                                         // le premier bloc
        bsup = Entete_TOVnC(&f, 1),                                                                       // le dernier bloc
        temp_j,                                                                                           // sauvegarder la pos dans le bloc avant de se deplacer
        stop = 0;                                                                                         // booleen pour arreter la recherche si on arrive a une cle superieure a la cle recherche
    char Cle_Min[TAILLE_IDENTIFIANT + 1],                                                                 // la plus petite cle dans un bloc (plus petit identifiant)
        Cle_Max[TAILLE_IDENTIFIANT + 1],                                                                  // la plus grande cle dans un bloc (plus grand identifiant)
        Cle_Courrante[TAILLE_IDENTIFIANT + 1],                                                            // la cle courrant dont on s'est arrete dans le parcours
        Identifiant[TAILLE_IDENTIFIANT + 1],                                                              // numero d'identifiant(cle)
        Supprime[TAILLE_SUPPRIMER + 1],                                                                   // supprimer='f' l'element n'a pas ete supprime supprimer='t' sinon
        Materiel[TAILLE_MATERIEL],                                                                        // le type du materiel
        Fonctionne[TAILLE_FONCTIONNEMENT + 1],                                                            // fonctionne = 'f', le materiel marche, fonctionne = 'n' sinon
        Prix[TAILLE_PRIX + 1],                                                                            // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],                                                                        // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1];                                                          // la description (caracteristiques) du materiel
    Tbloc_TOVnC Buf;                                                                                      // un buffer pour charger un bloc de MS vers MC
    *trouv = 0;                                                                                           // initialiser trouv vers faux
    while (!(*trouv) && binf <= bsup && !stop)                                                            // enregistrement non trouve et recherche possible(cle_courante<cle_recherche)
    {                                                                                                     // recherche dichotomique entre les bloc
        *i = (binf + bsup) / 2;                                                                           // numero de bloc a parcourir
        *j = 0;                                                                                           // la premiere position dans le bloc
        LireDir_TOVnC(&f, *i, &Buf);                                                                      // lire le buffer
        temp_j = *j;                                                                                      // sauvegarder j avant de se deplacer
        extraire_chaine_TOVnC(Cle_Min, j, TAILLE_IDENTIFIANT, &Buf);                                      // extraire la plus petite cle (premiere cle) de du bloc i
        strcpy(Cle_Courrante, Cle_Min);                                                                   // mettre a jour la cle courrante
        strcpy(Cle_Max, Buf.cleMax);                                                                      // lire la cle max qui est dans le tableau
        if (atoi(Identifiant_Recherche) >= atoi(Cle_Min) && atoi(Identifiant_Recherche) <= atoi(Cle_Max)) // si la cle à recherchee est entre Cle_Min et Cle_Max du bloc
        {                                                                                                 // alors on recherche la cle dans ce bloc
            while (!(*trouv) && *j < Buf.nb && !stop)                                                     // stop = cle_courante > cle_recherche
            {                                                                                             // recherche sequentielle dans le bloc
                extraire_chaine_TOVnC(Supprime, j, TAILLE_SUPPRIMER, &Buf);                               // recuperer un le champs de suppression
                if (strcmp(Identifiant_Recherche, Cle_Courrante) == 0 && strcmp(Supprime, "f") == 0)      // la cle est donc trouveee dans le bloc i
                {                                                                                         // extraire les autres champs pour afficher les infos du materiel
                    extraire_chaine_TOVnC(Materiel, j, TAILLE_MATERIEL - 1, &Buf);                        // extraire le materiel
                    extraire_chaine_TOVnC(Fonctionne, j, TAILLE_FONCTIONNEMENT, &Buf);                    // extraire son etat de fonctionnement
                    extraire_chaine_TOVnC(Prix, j, TAILLE_PRIX, &Buf);                                    // extraire son prix
                    extraire_chaine_TOVnC(Taille, j, TAILLE_TAILLE, &Buf);                                // extraire la taille de la description
                    extraire_chaine_TOVnC(Description, j, atoi(Taille), &Buf);                            // extraire la description
                    *trouv = 1;                                                                           // mettre trouve a vrai pour arreter la recherche
                    *j = temp_j;                                                                          // faire revenir j a la position du premier champs avant decalage (l'identifiant)
                    printf("\n\n---------------- L'enregistrement trouve dans le bloc %d a la position %d ------------------\n", *i, *j);
                    printf("| -> L'identifiant : %s\n", Cle_Courrante);                                                       // afficher la cle
                    printf("| -> Le type materiel : %s\n", Materiel);                                                         // afficher le materiel
                    if (strcmp(Fonctionne, "f") == 0)                                                                            // si fonctionne est a vrai
                        printf("| -> Fonctionnement du materiel: En marche\n", Fonctionne);                                   // le materiel est en marche
                    else                                                                                                         // sinon (fonctionne="n")
                        printf("|-> Fonctionnement du materiel: En panne\n", Fonctionne);                                    // le materiel est en panne
                    printf("| -> Le prix d'achat du materiel : %s\n", Prix);                                                  // afficher le prix
                    printf("|-> La Description : %s\n", Description);                                                        // afficher la description
                    printf("---------------------------------------------------------------------------------------------\n\n"); // fin d'affichage
                }
                else                                                       // si la cle courante n'est pas egale a la cle recherche
                {                                                          // alors
                    if (atoi(Identifiant_Recherche) < atoi(Cle_Courrante)) // si on a depasse l'ordre de la cle recherche
                    {                                                      // donc
                        stop = 1;                                          // la cle surement n'existe pas stop=vrai, on arrete la recherche
                        *j = temp_j;                                       // faire revenir j a la position du premier champs avant decalage (l'identifiant)
                        printf("\n\n---------------- L'enregistrement n'exsite pas, il devait exister: ------------------\n", *i, *j);
                        printf("|    -> Dans le bloc numero : %i\n", *i);
                        printf("|    -> Dans la position : %i\n", *j);
                        printf("-------------------------------------------------------------------------------------\n\n");
                    }
                    else                                                                     // sinon on passe au prochain materiel dans le meme bloc toujours
                    {                                                                        // alors
                        *j = *j + TAILLE_MATERIEL - 1 + TAILLE_FONCTIONNEMENT + TAILLE_PRIX; // zapper les champs: materiel, fonctionnement, taille
                        extraire_chaine_TOVnC(Taille, j, TAILLE_TAILLE, &Buf);               // recuperer la taille de la description
                        *j = *j + atoi(Taille);
                        temp_j = *j;                                                       // zapper la description egalement
                        extraire_chaine_TOVnC(Cle_Courrante, j, TAILLE_IDENTIFIANT, &Buf); // recuperer la cle prochaine a verifier
                    }
                }
            }
        }
        else                                                 // si la cle recherche n'est pas comprise entre la cle min et max du bloc
        {                                                    // alors
            if (atoi(Identifiant_Recherche) < atoi(Cle_Min)) // si la cle recherche > a la cle min du bloc
                bsup = *i - 1;                               // la cle doit être avant le bloc courant
            else                                             // si la cle recherche < a la cle min du bloc
                binf = *i + 1;                               /// cle  doit être après le bloc courant
        }
    }
    if (binf > bsup) // si on est sortie a cause des chevauchement des base
    {
        *i = binf;   // le bloc ou devait exister l'identifiant = binf
        *j = temp_j; // la pos ou devait commencer le materiel = temp_j
        printf("\n\n!---------------- L'enregistrement n'exsite pas, il devait exister: ------------------\n", *i, *j);
        printf("|    -> Dans le bloc numero : %i\n", *i);
        printf("|    -> Dans la position : %i\n", *j);
        printf("-------------------------------------------------------------------------------------\n\n");
    }
    Fermer_TOVnC(&f);
}
