void Recheche_TOVnC(fichier_TOVnC *f, char Identifiant_Recherche[], int *trouv, int *i, int *j)
{
    int binf = 1,                            // le premier bloc
        bsup = Entete_TOVnC(f, 1);           // le dernier bloc
    char Cle_Min[TAILLE_IDENTIFIANT],        // la plus petite cle dans un bloc (plus petit identifiant)
        Cle_Max[TAILLE_IDENTIFIANT],         // la plus grande cle dans un bloc (plus grand identifiant)
        Cle_Courrante[TAILLE_IDENTIFIANT],   // la cle courrant dont on s'est arrete dans le parcours
        Identifiant[TAILLE_IDENTIFIANT],     // numero d'identifiant(cle)
        Supprime,                            // supprimer='f' l'element n'a pas ete supprime supprimer='t' sinon
        Materiel[TAILLE_MATERIEL],           // le type du materiel
        Fonctionne,                          // fonctionne = 'f', le materiel marche, fonctionne = 'n' sinon
        Prix[TAILLE_PRIX],                   // le ptix du materiel
        Taille[TAILLE_TAILLE],               // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION]; // la description (caracteristiques) du materiel
    Tbloc_TOVnC buff;                        // un buffer pour charger un bloc de MS vers MC
    while (!(*trouv) && binf <= bsup)        // enregistrement non trouvé et recherche possible
    {
        *i = (binf + bsup) / 2;                                                   // numero de bloc a parcourir
        *j = 0;                                                                   // la premiere position dans le bloc
        LireDir_TOVnC(f, *i, &buff);                                              // lire le buffer
        extraire_chaine_TOVnC(Cle_Min, j, TAILLE_IDENTIFIANT, &buff);             // extraire la plus petite cle (premiere cle) de du bloc i
        strcat(Cle_Courrante, Cle_Min);                                           // mettre a jour la cle courrante
        strcat(Cle_Max, buff.cleMax);                                             // lire la cle max qui est dans le tableau
        if (Identifiant_Recherche >= Cle_Min && Identifiant_Recherche <= Cle_Max) // si la cle à recherchee est entre Cle_Min et Cle_Max du bloc on fait le recherche sequentielle dans le bloc
        {
            printf("here");
            while (!(*trouv) && *j < buff.nb)
            {
                extraire_chaine_TOVnC(&Supprime, j, TAILLE_SUPPRIMER, &buff); // récupérer un caractère d'Supprimeacement
                if (Identifiant_Recherche == Cle_Courrante && !Supprime)      // la cle st donc trouveée dans le bloc i
                {
                    extraire_chaine_TOVnC(Materiel, j, TAILLE_MATERIEL, &buff);
                    extraire_chaine_TOVnC(&Fonctionne, j, TAILLE_FONCTIONNEMENT, &buff);
                    extraire_chaine_TOVnC(Prix, j, TAILLE_PRIX, &buff);
                    extraire_chaine_TOVnC(Taille, j, TAILLE_TAILLE, &buff);
                    extraire_chaine_TOVnC(Description, j, atoi(Taille), &buff);
                    *trouv = 1;
                    printf("\n L'enregistrement est dans le bloc %d a la position %d", i, j);
                    printf("\n Les informations du materiel:  ----------------------------------------\n\n");
                    printf(" -> L'identifiant : %s\n", Identifiant);
                    printf(" -> Le type matériel : %s\n", Materiel);
                    printf(" -> Fonctionnement du materiel: %c\n", Fonctionne);
                    printf(" -> Le prix d'achat du matériel : %s\n", Prix);
                    printf(" -> La Description : %s\n", Description);
                }
                else
                {
                    *j = *j + TAILLE_MATERIEL + TAILLE_FONCTIONNEMENT + TAILLE_PRIX;
                    extraire_chaine_TOVnC(Taille, j, TAILLE_TAILLE, &buff); // récupérer un tableau de 3 caractères, la taille d'enregistrement
                    *j = *j + atoi(Taille);
                    extraire_chaine_TOVnC(Cle_Courrante, j, TAILLE_IDENTIFIANT, &buff);
                }
            }
        }
        else
        {
            printf("change the bloc");
            if (Identifiant_Recherche < Cle_Min) // la clé n'est pas dans ce bloc
                bsup = *i - 1;                   // la clé doit être avant le bloc courant
            else
                binf = *i + 1; /// clé  doit être après le bloc courant
        }
    }
    if (!(*trouv))
        printf("\nEnregistrement n'existe pas!\n");
}
