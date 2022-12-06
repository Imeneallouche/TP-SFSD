/**********************************************|
|                                              |
|       reorganiser un fichier TOVnC selon     |
|          le champs de fonctionnement         |
|                                              |
|**********************************************/
void Reorganisation_TOVnC(char nom_fichier[], char nom_fichier1[], char nom_fichier2[])
{
    fichier_TOVnC *f;                        // le fichier original qu'on doit reorganiser "Materiel_informatique_TOVnC.bin"
    Ouvrir_TOVnC(f, nom_fichier, 'A');       // ouvrir le fichier TOVnC
    fichier_TOVC *f1;                        // le fichier qui contient le materiel informatique ne marche
    Ouvrir_TOVC(f1, nom_fichier1, 'N');      // ouvrir le fichier TOVC
    fichier_LOVC *f2;                        // le fichier qui contient le materiel informatique en panne
    Ouvrir_LOVC(f2, nom_fichier2, 'N');      // ouvirir le fichier LOVC
    Tampon_TOVnC Buf;                        // Buffer de type TOVnC
    Tampon_TOVC Buf1;                        // Buffer de type TOVC
    Tampon_LOVC Buf2;                        // Buffer de type LOVC
    int i = 1,                               // le parcours des blocs du fichier
        j = 0;                               // le parcours de position du fichier
    char Identifiant[TAILLE_IDENTIFIANT],    // numero d'identifiant(cle)
        Supprime = 'f',                      // supprimer='f' l'element n'a pas ete supprime supprimer='t' sinon
        Materiel[TAILLE_MATERIEL],           // le type du materiel
        Fonctionne = 'f',                    // fonctionne = 'f', le materiel marche, fonctionne = 'n' sinon
        Prix[TAILLE_PRIX],                   // le ptix du materiel
        Taille[TAILLE_TAILLE],               // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION]; // la description (caracteristiques) du materiels

    while (i < Entete_TOVnC(f, 1))
    {
        LireDir_TOVnC(f, i, &Buf);
        while (j < Buf.nb)
        {
            extraire_chaine_TOVnC(Identifiant, &j, TAILLE_IDENTIFIANT, &Buf);
            extraire_chaine_TOVnC(&Supprime, &j, TAILLE_SUPPRIMER, &Buf);
            extraire_chaine_TOVnC(Materiel, &j, TAILLE_MATERIEL, &Buf);
            extraire_chaine_TOVnC(&Fonctionne, &j, TAILLE_FONCTIONNEMENT, &Buf);
            extraire_chaine_TOVnC(Prix, &j, TAILLE_PRIX, &Buf);
            extraire_chaine_TOVnC(Taille, &j, TAILLE_TAILLE, &Buf);
            extraire_chaine_TOVnC(Description, &j, atoi(Taille), &Buf);

            if (Supprime != 't')
            {
            }
        }
        j = 0;
        i++;
    }
}