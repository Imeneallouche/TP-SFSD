#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "lib.h"

/*************************************|
|                                     |
|         FICHIER TYPE TOVnC          |
|                                     |
|*************************************/
/*************************************************************|
|                                                             |
| Ouvrir fichier nom_fichier avec le mode correspondant TOVnC |
|               Realise par : Imene ALLOUCHE                  |
|                                                             |
|*************************************************************/
void Ouvrir_TOVnC(fichier_TOVnC *f, char nom_fichier[], char mode_ouverture)
{
    /// si on ouvre en mode nouveau
    if (tolower(mode_ouverture) == 'n')
    {
        f->fichier = fopen(nom_fichier, "wb+");
        if (f->fichier != NULL)
        {
            /// initialisation de l'entête :
            f->entete.nombre_bloc = 0;
            f->entete.nbr_caract_insert = 0;
            f->entete.nbr_caract_supp = 0;

            rewind(f->fichier); /// positionnement au debut du fichier
            fwrite(&(f->entete), sizeof(entete_TOVnC), 1, f->fichier);
        }
        else
            printf("\nErreur lors de l'ouverture du fichier... verifier le nom du fichier");
    }
    /// si on ouvre en mode ancien
    else if (tolower(mode_ouverture) == 'a')
    {
        f->fichier = fopen(nom_fichier, "rb+");
        if (f->fichier != NULL)
        {
            rewind(f->fichier);
            fread(&(f->entete), sizeof(entete_TOVnC), 1, f->fichier);
        }
        else
            printf("\nErreur lors de l'ouverture du fichier... verifier le nom du fichier");
    }
    else
    {
        f->fichier = NULL;
        printf("Mode d'ouverture erronne\n");
    }
}

/*************************************|
|                                     |
|     Fermer le fichier TOVnC         |
|   Realise par : Imene ALLOUCHE      |
|                                     |
|*************************************/
void Fermer_TOVnC(fichier_TOVnC *f)
{
    rewind(f->fichier);
    fwrite(&(f->entete), sizeof(entete_TOVnC), 1, f->fichier);
    fclose(f->fichier);
}

/********************************************|
|                                            |
|    Lire le i eme bloc dans buf TOVnC       |
|       Realise par : Imene ALLOUCHE         |
|                                            |
|********************************************/
void LireDir_TOVnC(fichier_TOVnC *f, int i, Tbloc_TOVnC *buf)
{
    /// on se jitionne au debut du i ème bloc puis on le lit dans buf
    fseek(f->fichier, sizeof(entete_TOVnC) + (sizeof(Tbloc_TOVnC) * (i - 1)), SEEK_SET);
    fread(buf, sizeof(Tbloc_TOVnC), 1, f->fichier);
}

/**********************************************|
|                                              |
|  Retoure la i ème valeur del'entete  TOVnC   |
|        Realise par : Imene ALLOUCHE          |
|                                              |
|**********************************************/
int Entete_TOVnC(fichier_TOVnC *f, int i)
{
    if (i == 1)
        return (f->entete.nombre_bloc);
    else if (i == 2)
        return (f->entete.nbr_caract_insert);
    else if (i == 3)
        return (f->entete.nbr_caract_supp);
    else
    {
        printf("Parametre inexistant dans l'entete\n");
        return -1;
    }
}

/*********************************************|
|                                             |
|    Ecrire buf dans le i eme bloc TOVnC      |
|        Realise par : Imene ALLOUCHE         |
|                                             |
|*********************************************/
void EcrireDir_TOVnC(fichier_TOVnC *f, int i, Tbloc_TOVnC buf)
{
    /// on se positionne au debut du i ème bloc puis on ecrit dans fichier->f
    fseek(f->fichier, sizeof(entete_TOVnC) + (sizeof(Tbloc_TOVnC) * (i - 1)), SEEK_SET);
    fwrite(&buf, sizeof(Tbloc_TOVnC), 1, f->fichier);
}

/***********************************************|
|                                               |
|  modifie la i ème valeur de l'entete  TOVnC   |
|          Realise par : Imene ALLOUCHE         |
|                                               |
|***********************************************/
void Aff_Entete_TOVnC(fichier_TOVnC *f, int i, int val)
{
    if (i == 1)
        f->entete.nombre_bloc = val;
    else if (i == 2)
        f->entete.nbr_caract_insert = val;
    else if (i == 3)
        f->entete.nbr_caract_supp = val;
    else
        printf("Parametre inexistant dans l'entete\n");
}

/**********************************************|
|                                              |
|   Retourne le numero du nouveau bloc TOVnC   |
|         Realise par : Imene ALLOUCHE         |
|                                              |
|**********************************************/
int Alloc_bloc_TOVnC(fichier_TOVnC *f)
{
    int i = Entete_TOVnC(f, 1);    // le nombre de bloc = le numero du dernier bloc -1
    Aff_Entete_TOVnC(f, 1, i + 1); // mettre a jour le nombre de bloc dans l'entete
    return Entete_TOVnC(f, 1);     // le nombre anciens de bloc = numero du dernier bloc
}

/**************************************************|
|                                                  |
|    afficher les caracteristqiues (entete) d'un   |
|       fichier "nom fichier" de type TOVnC        |
|          Realise par : Imene ALLOUCHE            |
|                                                  |
|**************************************************/
void affichage_entete_TOVnC(char nom_fichier[])
{
    fichier_TOVnC f;
    Ouvrir_TOVnC(&f, nom_fichier, 'A');
    printf("\n\n\n*************************************************\n");
    printf("*                                               *\n");
    printf("*       caracteristiques du fichier             *\n");
    printf("*                                               *\n");
    printf("*************************************************\n");
    printf(" -> Nombre de Blocs : %d\n", Entete_TOVnC(&f, 1));
    printf(" -> Nombre caracteres inseres : %d\n", Entete_TOVnC(&f, 2));
    printf(" -> Nombre caracteres supprimes : %d\n", Entete_TOVnC(&f, 3));
}

/**********************************************|
|                                              |
|       affichier le contenu d'un fichier      |
|                de type TOVnC                 |
|        Realise par : Imene ALLOUCHE          |
|                                              |
|**********************************************/
void afficher_fichier_TOVnC(char nom_fichier[])
{
    fichier_TOVnC f;
    Ouvrir_TOVnC(&f, nom_fichier, 'A');
    int i = 1,                                   // parcours bloc par bloc
        j = 0,                                   // parcours de position dans le bloc
        counter = 0;                             // numero de l'enregistrement dans le bloc
    Tampon_TOVnC Buf;                            // contenu d'un bloc dans un buffer
    char Identifiant[TAILLE_IDENTIFIANT + 1],    // numero d'identifiant(cle)
        Materiel[TAILLE_MATERIEL],               // le type du materiel
        Fonctionne[TAILLE_FONCTIONNEMENT + 1],   // fonctionne = "f", le materiel marche, fonctionne = "n" sinon
        Prix[TAILLE_PRIX + 1],                   // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],               // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1]; // la description (caracteristiques) du materiel

    /***************************************************************************************************|
    | Identifiant | champs fonctionne | Type materiel |    Prix   |   taille   | Description (variable) |
    |  (5 bytes)  |   (1 bytes)       |  (12 bytes)   | (6 bytes) |  (3 bytes) |  (max sur 273 bytes)   |
    |***************************************************************************************************/
    while (i <= Entete_TOVnC(&f, 1))
    {
        LireDir_TOVnC(&f, i, &Buf);
        printf("\n\n\n*************************************************\n");
        printf("*                                               *\n");
        printf("*            Le bloc numero : %i                 *\n", i);
        printf("*        rempli a %i / %i caracteres          *\n", Buf.nb, B);
        printf("*                                               *\n");
        printf("*************************************************\n");

        while (j < Buf.nb)
        {
            extraire_chaine_TOVnC(Identifiant, &j, TAILLE_IDENTIFIANT, &Buf);
            extraire_chaine_TOVnC(Fonctionne, &j, TAILLE_FONCTIONNEMENT, &Buf);
            extraire_chaine_TOVnC(Materiel, &j, TAILLE_MATERIEL - 1, &Buf);
            extraire_chaine_TOVnC(Prix, &j, TAILLE_PRIX, &Buf);
            extraire_chaine_TOVnC(Taille, &j, TAILLE_TAILLE, &Buf);
            extraire_chaine_TOVnC(Description, &j, atoi(Taille), &Buf);

            printf("\n\n.........................\n");
            printf(".                       .\n");
            printf(".  Materiel numero : %i  .\n", counter);
            printf(".                       .\n");
            printf(".........................\n");
            printf("identifiant: %.5s\n", Identifiant);
            printf("materiel: %.11s\n", Materiel);
            if (strcmp(Fonctionne, "f") == 0)
                printf("Fonctionne? : OUI\n");
            else
                printf("Fonctionne? : NON\n");
            printf("prix: %.6s DA\n", Prix);
            printf("taille de description: %.3s\n", Taille);
            printf("description: %s\n", Description);
            counter++;
        }
        j = 0;
        i++;
    }
}

/*




















*/

/*************************************|
|                                     |
|           FICHIER TYPE TOF          |
|                                     |
|*************************************/
/************************************************************|
|                                                            |
|  Ouvrir fichier nom_fichier avec le mode correspondant TOF |
|                Realise par : Imene ALLOUCHE                |
|                                                            |
|************************************************************/
void Ouvrir_TOF(fichier_TOF *f, char nom_fichier[], char mode)
{
    if (tolower(mode) == 'a')
    {
        f->fichier = fopen(nom_fichier, "rb+");
        if (f->fichier == NULL)
            printf("Erreur lors de l'ouverture du fichier... verifier le nom du fichier");
        else
        {
            rewind(f->fichier); /// positionnement au debut du fichier
            fread(&(f->entete), sizeof(entete_TOF), 1, f->fichier);
        }
    }
    else if (tolower(mode) == 'n')
    {
        f->fichier = fopen(nom_fichier, "wb+");
        Aff_Entete_TOF(f, 1, 0); // mettre le nombre de blocs à 0
        Aff_Entete_TOF(f, 2, 0); // mettre le nombre d'enregistrements inseres à 0
        Aff_Entete_TOF(f, 3, 0); // mettre le nombre d'enregistrements supprimes à 0

        rewind(f->fichier); /// positionnement au debut du fichier
        fwrite(&(f->entete), sizeof(entete_TOF), 1, f->fichier);
    }
    else
    {
        f->fichier = NULL;
        printf("Mode d'ouverture erronne\n");
    }
}

/*************************************|
|                                     |
|       Fermer le fichier TOF         |
|    Realise par : Imene ALLOUCHE     |
|                                     |
|*************************************/
void Fermer_TOF(fichier_TOF *f)
{
    fseek(f->fichier, 0, SEEK_SET);
    fwrite(&(f->entete), sizeof(entete_TOF), 1, f->fichier);
    fclose(f->fichier);
}

/********************************************|
|                                            |
|      Lire le i eme bloc dans buf TOF       |
|       Realise par : Imene ALLOUCHE         |
|                                            |
|********************************************/
void LireDir_TOF(fichier_TOF *f, int i, Tampon_TOF *buf)
{
    rewind(f->fichier);
    fseek(f->fichier, sizeof(fichier_TOF) + (i - 1) * sizeof(Tampon_TOF), SEEK_SET);
    fread(buf, sizeof(Tampon_TOF), 1, f->fichier);
}

/*********************************************|
|                                             |
|      Ecrire buf dans le i eme bloc TOF      |
|        Realise par : Imene ALLOUCHE         |
|                                             |
|*********************************************/
void EcrireDir_TOF(fichier_TOF *f, int i, Tampon_TOF *buf)
{
    rewind(f->fichier);
    fseek(f->fichier, sizeof(entete_TOF) + (i - 1) * sizeof(Tampon_TOF), SEEK_SET);
    fwrite(buf, sizeof(Tampon_TOF), 1, f->fichier);
}

/********************************************|
|                                            |
|  Retoure la i ème valeur del'entete  TOF   |
|       Realise par : Imene ALLOUCHE         |
|                                            |
|********************************************/
int Entete_TOF(fichier_TOF *f, int i)
{
    if (i == 1) // nombre de blocs total
        return f->entete.blocs_total;
    else if (i == 2) // nombre d'enregistrements inseres
        return f->entete.enreg_inseres;
    else if (i == 3) // nombre d'enregistrements supprimes
        return f->entete.enreg_supprimes;
    else
        return -1;
}

/*********************************************|
|                                             |
|  modifie la i ème valeur de l'entete  TOF   |
|        Realise par : Imene ALLOUCHE         |
|                                             |
|*********************************************/
void Aff_Entete_TOF(fichier_TOF *f, int i, int val)
{
    if (i == 1) // nombre de blocs total
        f->entete.blocs_total = val;
    else if (i == 2) // nombre d'enregistrements inseres
        f->entete.enreg_inseres = val;
    else if (i == 3) // nombre d'enregistrements supprimes
        f->entete.enreg_supprimes = val;
    else
        printf("Parametre inexistant dans l'entete\n");
}

/********************************************|
|                                            |
|   Retourne le numero du nouveau bloc TOF   |
|        Realise par : Imene ALLOUCHE        |
|                                            |
|********************************************/
int Alloc_bloc_TOF(fichier_TOF *f)
{
    int i = Entete_TOF(f, 1);    // le nombre de bloc = le numero du dernier bloc
    Aff_Entete_TOF(f, 1, i + 1); // mettre a jour le nombre de bloc dans l'entete
    return Entete_TOF(f, 1);     // le nombre anciens de bloc = numero du dernier bloc
}

/**************************************************|
|                                                  |
|    afficher les caracteristqiues (entete) d'un   |
|        fichier "nom fichier" de type TOF         |
|          Realise par : Imene ALLOUCHE            |
|                                                  |
|**************************************************/
void affichage_entete_TOF(char nom_fichier[])
{
    fichier_TOF f;
    Ouvrir_TOF(&f, nom_fichier, 'A');
    printf("\n\n\n*************************************************\n");
    printf("*                                               *\n");
    printf("*       caracteristiques du fichier             *\n");
    printf("*                                               *\n");
    printf("*************************************************\n");
    printf(" -> Nombre de Blocs : %d\n", Entete_TOF(&f, 1));
    printf(" -> Nombre d'enregistrements inseres : %d\n", Entete_TOF(&f, 2));
    printf(" -> Nombre d'enregistrements supprimes : %d\n", Entete_TOF(&f, 3));
}

/**********************************************|
|                                              |
|       affichier le contenu d'un fichier      |
|                de type TOF                   |
|        Realise par : Imene ALLOUCHE          |
|                                              |
|**********************************************/
void afficher_fichier_TOF(char nom_fichier[])
{
    fichier_TOF f;
    Ouvrir_TOF(&f, nom_fichier, 'A');
    int i = 1,       // parcours bloc par bloc
        j = 0,       // parcours de position dans le bloc
        counter = 0; // numero de l'enregistrement dans le bloc
    Tampon_TOF Buf;  // contenu d'un bloc dans un buffer

    /*****************************|
    |  Identifiant  |     Prix    |
    |   (5 bytes)   |  (6 bytes)  |
    |*****************************/
    // while (i <= Entete_TOF(&f, 1))
    while (counter <= 2)
    {
        LireDir_TOF(&f, i, &Buf);
        j = 0;
        printf("\n\n\n*************************************************\n");
        printf("*                                               *\n");
        printf("*            Le bloc numero : %i                 *\n", i);
        printf("*        rempli a %i / %i enregistrements          *\n", Buf.nombre_enreg, MAX_ENREG);
        printf("*                                               *\n");
        printf("*************************************************\n");

        // while (j < Buf.nombre_enreg)
        while (counter <= 2)
        {
            printf("\n\n.........................\n");
            printf(".                       .\n");
            printf(".  Materiel numero : %i  .\n", counter);
            printf(".                       .\n");
            printf(".........................\n");
            printf("identifiant: %i\n", Buf.tab[j].Identifiant);
            printf("Fonctionne? : %i\n", Buf.tab[j].supprimer);
            printf("prix: %i DA\n", Buf.tab[j].Prix);
            counter++;
            j++;
        }
        i++;
    }
}

/*




















*/

/*************************************|
|                                     |
|           FICHIER TYPE TOVC         |
|                                     |
|*************************************/
/*************************************************************|
|                                                             |
|  Ouvrir fichier nom_fichier avec le mode correspondant TOVC |
|                Realise par : Imene ALLOUCHE                 |
|                                                             |
|*************************************************************/
void Ouvrir_TOVC(fichier_TOVC *f, char nom_fichier[], char mode_ouverture)
{
    /// si on ouvre en mode nouveau
    if (tolower(mode_ouverture) == 'n')
    {
        f->fichier = fopen(nom_fichier, "wb+");
        if (f->fichier != NULL)
        {
            /// initialisation de l'entête :
            f->entete.adr_dernier_bloc = 0;
            f->entete.pos_libre_dernier_bloc = 0;
            f->entete.nbr_caract_insert = 0;
            f->entete.nbr_caract_supp = 0;

            rewind(f->fichier); /// positionnement au debut du fichier
            fwrite(&(f->entete), sizeof(entete_TOVC), 1, f->fichier);
        }
        else
            printf("\nErreur lors de l'ouverture du fichier... verifier le nom du fichier");
    }
    /// si on ouvre en mode ancien
    else if (tolower(mode_ouverture) == 'a')
    {
        f->fichier = fopen(nom_fichier, "rb+");
        if (f->fichier != NULL)
        {
            rewind(f->fichier);
            fread(&(f->entete), sizeof(entete_TOVC), 1, f->fichier);
        }
        else
            printf("\nErreur lors de l'ouverture du fichier... verifier le nom du fichier");
    }
    else
    {
        f->fichier = NULL;
        printf("Mode d'ouverture erronne\n");
    }
}

/*************************************|
|                                     |
|      Fermer le fichier TOVC         |
|   Realise par : Imene ALLOUCHE      |
|                                     |
|*************************************/
void Fermer_TOVC(fichier_TOVC *f)
{
    rewind(f->fichier);
    fwrite(&(f->entete), sizeof(entete_TOVC), 1, f->fichier);
    fclose(f->fichier);
    free(f);
}

/********************************************|
|                                            |
|     Lire le i eme bloc dans buf TOVC       |
|       Realise par : Imene ALLOUCHE         |
|                                            |
|********************************************/
void LireDir_TOVC(fichier_TOVC *f, int i, Tbloc_TOVC *buf)
{
    /// on se jitionne au debut du i ème bloc puis on le lit dans buf
    fseek(f->fichier, sizeof(entete_TOVC) + (sizeof(Tbloc_TOVC) * (i - 1)), SEEK_SET);
    fread(buf, sizeof(Tbloc_TOVC), 1, f->fichier);
}

/********************************************|
|                                            |
| Retoure la i ème valeur del'entete  TOVC   |
|       Realise par : Imene ALLOUCHE         |
|                                            |
|********************************************/
int Entete_TOVC(fichier_TOVC *f, int i)
{
    if (i == 1)
        return (f->entete.adr_dernier_bloc);
    else if (i == 2)
        return (f->entete.pos_libre_dernier_bloc);
    else if (i == 3)
        return (f->entete.nbr_caract_insert);
    else if (i == 4)
        return (f->entete.nbr_caract_supp);
    else
        printf("Parametre inexistant dans l'entete\n");
}

/*********************************************|
|                                             |
|     Ecrire buf dans le i eme bloc TOVC      |
|        Realise par : Imene ALLOUCHE         |
|                                             |
|*********************************************/
void EcrireDir_TOVC(fichier_TOVC *f, int i, Tbloc_TOVC buf)
{
    /// on se positionne au debut du i ème bloc puis on ecrit dans fichier->f
    fseek(f->fichier, sizeof(entete_TOVC) + (sizeof(Tbloc_TOVC) * (i - 1)), SEEK_SET);
    fwrite(&buf, sizeof(Tbloc_TOVC), 1, f->fichier);
}

/*********************************************|
|                                             |
| modifie la i ème valeur de l'entete  TOVC   |
|       Realise par : Imene ALLOUCHE          |
|                                             |
|*********************************************/
void Aff_Entete_TOVC(fichier_TOVC *f, int i, int val)
{
    if (i == 1)
        f->entete.adr_dernier_bloc = val;
    else if (i == 2)
        f->entete.pos_libre_dernier_bloc = val;
    else if (i == 3)
        f->entete.nbr_caract_insert = val;
    else if (i == 4)
        f->entete.nbr_caract_supp = val;
    else
        printf("Parametre inexistant dans l'entete\n");
}

/********************************************|
|                                            |
|  Retourne le numero du nouveau bloc TOVC   |
|       Realise par : Imene ALLOUCHE          |
|                                            |
|********************************************/
int Alloc_bloc_TOVC(fichier_TOVC *f)
{
    int i = Entete_TOVC(f, 1);    // le nombre de bloc = le numero du dernier bloc -1
    Aff_Entete_TOVC(f, 1, i + 1); // mettre a jour le nombre de bloc dans l'entete
    return Entete_TOVC(f, 1);     // le nombre anciens de bloc = numero du dernier bloc
}

/**************************************************|
|                                                  |
|    afficher les caracteristqiues (entete) d'un   |
|       fichier "nom fichier" de type TOVC         |
|          Realise par : Imene ALLOUCHE            |
|                                                  |
|**************************************************/
void affichage_entete_TOVC(char nom_fichier[])
{
    fichier_TOVC f;
    Ouvrir_TOVC(&f, nom_fichier, 'A');
    printf("\n\n\n*************************************************\n");
    printf("*                                               *\n");
    printf("*       caracteristiques du fichier             *\n");
    printf("*                                               *\n");
    printf("*************************************************\n");
    printf(" -> nombre de bloc : %d\n", Entete_TOVC(&f, 1));
    printf(" -> position libre dans le dernier bloc : %d\n", Entete_TOVC(&f, 2));
    printf(" -> Nombre caracteres inseres : %d\n", Entete_TOVC(&f, 3));
    printf(" -> Nombre caracteres supprimes : %d\n", Entete_TOVC(&f, 4));
}

/**********************************************|
|                                              |
|       affichier le contenu d'un fichier      |
|                de type TOVC                  |
|        Realise par : Imene ALLOUCHE          |
|                                              |
|**********************************************/
void afficher_fichier_TOVC(char nom_fichier[])
{
    fichier_TOVC f;
    Ouvrir_TOVC(&f, nom_fichier, 'A');
    int i = 1,       // parcours entre bloc du fichier LOVC du materiel en panne
        j = 0,       // parcours inter-bloc du fichier LOVC du materiel en panne
        counter = 0; // nombre d'elements
    Tampon_TOVC Buf; // buffer avec lequel on charge les bloc de MS vers MC

    char Identifiant[TAILLE_IDENTIFIANT + 1],    // numero d'identifiant(cle)
        Materiel[TAILLE_MATERIEL],               // le type du materiel
        Prix[TAILLE_PRIX + 1],                   // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],               // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1]; // la description (caracteristiques) du materiel

    LireDir_TOVC(&f, i, &Buf);
    while (!(i == Entete_TOVC(&f, 1) && j >= Entete_TOVC(&f, 2))) // tant que on est pas arrive a la fin du fichier
    {
        /***************************************************************************|
        | Identifiant | Type materiel |   Prix    |   taille   |    Description     |
        |  (5 bytes)  |   (12 bytes)  | (6 bytes) |  (3 bytes) |    (variable)      |
        |***************************************************************************/
        extraire_chaine_TOVC(&f, Identifiant, &i, &j, TAILLE_IDENTIFIANT, &Buf);
        extraire_chaine_TOVC(&f, Materiel, &i, &j, TAILLE_MATERIEL - 1, &Buf);
        extraire_chaine_TOVC(&f, Prix, &i, &j, TAILLE_PRIX, &Buf);
        extraire_chaine_TOVC(&f, Taille, &i, &j, TAILLE_TAILLE, &Buf);
        extraire_chaine_TOVC(&f, Description, &i, &j, atoi(Taille), &Buf);

        printf("\n.........................\n");
        printf(".                       .\n");
        printf(".  Materiel numero : %i  .\n", counter);
        printf(".                       .\n");
        printf(".........................\n");
        printf("identifiant: %.5s\n", Identifiant);
        printf("materiel: %.11s\n", Materiel);
        printf("Fonctionne? : OUI\n");
        printf("prix: %.6s DA\n", Prix);
        printf("taille de description: %.3s\n", Taille);
        printf("description: %s\n", Description);
        counter++;
    }
}

/*




















*/

/*************************************|
|                                     |
|           FICHIER TYPE LOVC         |
|                                     |
|*************************************/
/*************************************************************|
|                                                             |
|  Ouvrir fichier nom_fichier avec le mode correspondant LOVC |
|                 Realise par : Imene ALLOUCHE                |
|                                                             |
|*************************************************************/
void Ouvrir_LOVC(fichier_LOVC *f, char nom_fichier[], char mode)
{
    if (tolower(mode) == 'a') // mode ancien
    {
        f->fichier = fopen(nom_fichier, "rb+");
        if (f->fichier == NULL)
            printf("error interrupted the program ... check file's name");
        else
        {
            rewind(f->fichier);
            fread(&(f->entete), sizeof(Entete_LOVC), 1, f->fichier); // chargement de l'entete
        }
    }

    else if (tolower(mode) == 'n')
    {
        f->fichier = fopen(nom_fichier, "wb+");
        aff_entete_LOVC(f, 1, -1); // mise a NIL de l'adresse du premier bloc tant qu'il est vide
        aff_entete_LOVC(f, 2, -1); // metter l'adresse du dernier bloc a NIL tant que le fichier est vide
        aff_entete_LOVC(f, 3, 0);  // initialiser la position libre dans le dernier bloc a 0
        aff_entete_LOVC(f, 4, 0);  // initialiser le nombre de caractères inseres a 0
        aff_entete_LOVC(f, 5, 0);  // initialiser le nombre de caractères supprimes a 0

        rewind(f->fichier); /// positionnement au debut du fichier
        fwrite(&(f->entete), sizeof(Entete_LOVC), 1, f->fichier);
    }

    else // mode d'ouverture incorrecte
    {
        f->fichier = NULL;
        printf("Mode d'ouverture erronne\n");
    }
}

/**************************************|
|                                      |
|       Fermer le fichier LOVC         |
|    Realise par : Imene ALLOUCHE      |
|                                      |
|**************************************/
void fermer_LOVC(fichier_LOVC *f)
{
    rewind(f->fichier);                                       // repositionnement du curseur en debut de fichier
    fwrite(&(f->entete), sizeof(Entete_LOVC), 1, f->fichier); // sauvegarde l'entete en debut du fichier
    fclose(f->fichier);                                       // fermeture du fichier
}

/*********************************************|
|                                             |
|      Lire le i eme bloc dans buf LOVC       |
|        Realise par : Imene ALLOUCHE         |
|                                         ,   |
|*********************************************/
void LireDir_LOVC(fichier_LOVC *f, int i, Tampon_LOVC *buf)
{
    // positionnement au debut du bloc numero i
    fseek(f->fichier, (sizeof(Entete_LOVC) + sizeof(Tbloc_LOVC) * (i - 1)), SEEK_SET);
    // lecture d'un bloc de caractere correspondant a la taille du bloc dans le buffer
    fread(buf, sizeof(Tampon_LOVC), 1, f->fichier);
}

/**********************************************|
|                                              |
|      Ecrire buf dans le i eme bloc LOVC      |
|         Realise par : Imene ALLOUCHE         |
|                                              |
|*********************************************/
void ecrireDir_LOVC(fichier_LOVC *f, int i, Tampon_LOVC *buf)
{

    // positionnement au debut du bloc numero i
    fseek(f->fichier, sizeof(Entete_LOVC) + sizeof(Tbloc_LOVC) * (i - 1), SEEK_SET);
    // ecriture du contenu du buffer dans le bloc numero i du fichier
    fwrite(buf, sizeof(Tampon_LOVC), 1, f->fichier);
}

/*********************************************|
|                                             |
|  Retoure la i ème valeur del'entete  LOVC   |
|       Realise par : Imene ALLOUCHE          |
|                                             |
|*********************************************/
int entete_LOVC(fichier_LOVC *f, int i)
{
    if (i == 1)
        return (f->entete.Num_premier_bloc); // l'adresse du premier bloc
    else if (i == 2)
        return (f->entete.Num_dernier_bloc);
    else if (i == 3)
        return (f->entete.pos_libre_dernier_bloc); // la position libre dans le dernier bloc
    else if (i == 4)
        return (f->entete.nb_chars_inseres); // numero du bloc representatnt la tete du fichier
    else if (i == 5)
        return (f->entete.nb_chars_supprimes); // nombre de caracteres inseres (effaces non inclus)
    else
    {
        printf("Parametre inexistant dans l'entete\n");
        return -1;
    }
}

/**********************************************|
|                                              |
|  modifie la i ème valeur de l'entete  LOVC   |
|       Realise par : Imene ALLOUCHE          |
|                                              |
|**********************************************/
void aff_entete_LOVC(fichier_LOVC *f, int i, int val)
{
    if (i == 1)
        f->entete.Num_premier_bloc = val; // le numero du premier bloc
    else if (i == 2)
        f->entete.Num_dernier_bloc = val; // le numero du dernier bloc
    else if (i == 3)
        f->entete.pos_libre_dernier_bloc = val; // la position libre dans le denier bloc
    else if (i == 4)
        f->entete.nb_chars_inseres = val; // nombre de caractères inseres dans le fichier
    else if (i == 5)
        f->entete.nb_chars_supprimes = val; // nombre de caractères supprimes dans le fichier
    else
        printf("Parametre inexistant dans l'entete\n");
}

/*********************************************|
|                                             |
|   Retourne le numero du nouveau bloc LOVC   |
|       Realise par : Imene ALLOUCHE          |
|                                             |
|*********************************************/
int alloc_bloc_LOVC(fichier_LOVC *fichier, Tampon_LOVC *buf)
{
    if (entete_LOVC(fichier, 1) != -1) // si le fichier n'est pas vide
    {
        LireDir_LOVC(fichier, entete_LOVC(fichier, 2), buf);      // lecture du bloc correspondant a la queue
        buf->suivant = entete_LOVC(fichier, 2) + 1;               // mise a jour dui suivant de la queue au bloc correspondant a la nouvelle queue
        ecrireDir_LOVC(fichier, entete_LOVC(fichier, 2), buf);    // ecriture du bloc de queue dans le fichier
        aff_entete_LOVC(fichier, 2, entete_LOVC(fichier, 2) + 1); // mise a jour du numero du bloc correspondant a la nouvelle queue dan sl'entete
    }
    else // si le fichier est vide
    {
        aff_entete_LOVC(fichier, 1, 1); // mise a jour du numero du premier bloc du fichier
        aff_entete_LOVC(fichier, 2, 1); // mise a jour du numero du dernier bloc du fichier
    }
    buf->suivant = -1;              // mise a jour du suivant a nill
    aff_entete_LOVC(fichier, 3, 0); // mise a jour de la pos libre dans le dernier bloc( nouveau bloc vide)
    sprintf(buf->tab, "%s", "");    // vider la chaine du buffer
    return entete_LOVC(fichier, 2);
}

/**************************************************|
|                                                  |
|    afficher les caracteristqiues (entete) d'un   |
|       fichier "nom fichier" de type TOVC         |
|          Realise par : Imene ALLOUCHE            |
|                                                  |
|**************************************************/
void affichage_entete_LOVC(char nom_fichier[])
{
    fichier_LOVC f;
    Ouvrir_LOVC(&f, nom_fichier, 'A');
    printf("\n\n\n*************************************************\n");
    printf("*                                               *\n");
    printf("*       caracteristiques du fichier             *\n");
    printf("*                                               *\n");
    printf("*************************************************\n");
    printf(" -> adresse du premier bloc : %d\n", entete_LOVC(&f, 1));
    printf(" -> adresse du dernier bloc : %d\n", entete_LOVC(&f, 2));
    printf(" -> la position libre dans le dernier bloc : %d\n", entete_LOVC(&f, 3));
    printf(" -> Nombre caracteres inseres : %d\n", entete_LOVC(&f, 4));
    printf(" -> Nombre caracteres supprimes : %d\n", entete_LOVC(&f, 5));
}

/**********************************************|
|                                              |
|       affichier le contenu d'un fichier      |
|                de type LOVC                  |
|         Realise par : Imene ALLOUCHE         |
|                                              |
|**********************************************/
void afficher_fichier_LOVC(char nom_fichier[])
{
    fichier_LOVC f;
    Ouvrir_LOVC(&f, nom_fichier, 'A');
    int i = entete_LOVC(&f, 1), // parcours entre bloc du fichier LOVC du materiel en panne
        j = 0,                  // parcours inter-bloc du fichier LOVC du materiel en panne
        counter = 0;            // nombre d'elements
    Tampon_LOVC Buf;            // buffer avec lequel on charge les bloc de MS vers MC

    char Identifiant[TAILLE_IDENTIFIANT + 1],    // numero d'identifiant(cle)
        Materiel[TAILLE_MATERIEL],               // le type du materiel
        Prix[TAILLE_PRIX + 1],                   // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],               // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1]; // la description (caracteristiques) du materiel

    LireDir_LOVC(&f, i, &Buf);
    while (!(i == entete_LOVC(&f, 2) && j >= entete_LOVC(&f, 3))) // tant que on est pas arrive a la fin du fichier
    {
        /***************************************************************************|
        | Identifiant | Type materiel |   Prix    |   taille   |    Description     |
        |  (5 bytes)  |   (12 bytes)  | (6 bytes) |  (3 bytes) |    (variable)      |
        |***************************************************************************/
        extraire_chaine_LOVC(&f, Identifiant, &i, &j, TAILLE_IDENTIFIANT, &Buf);
        extraire_chaine_LOVC(&f, Materiel, &i, &j, TAILLE_MATERIEL - 1, &Buf);
        extraire_chaine_LOVC(&f, Prix, &i, &j, TAILLE_PRIX, &Buf);
        extraire_chaine_LOVC(&f, Taille, &i, &j, TAILLE_TAILLE, &Buf);
        extraire_chaine_LOVC(&f, Description, &i, &j, atoi(Taille), &Buf);

        printf("\n.........................\n");
        printf(".                       .\n");
        printf(".  Materiel numero : %i  .\n", counter);
        printf(".                       .\n");
        printf(".........................\n");
        printf("identifiant: %.5s\n", Identifiant);
        printf("materiel: %.11s\n", Materiel);
        printf("Fonctionne? : NON\n");
        printf("prix: %.6s DA\n", Prix);
        printf("taille de description: %.3s\n", Taille);
        printf("description: %s\n", Description);
        counter++;
    }
}
/*




















*/

/*
/**********************************************|
|                                              |
|            FUNCTIONS USED IN TP              |
|                                              |
|**********************************************/
/**********************************************|
|                                              |
|     Generer un nombre aleatoire qui se       |
|   trouve entre les bornes [lower, upper]     |
|       Realise par : Imene ALLOUCHE          |
|                                              |
|**********************************************/
int Random_Number(int lower, int upper)
{
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

/**********************************************|
|                                              |
|    Generer une chaine aleatoire de taille    |
|         length extraite de alphabet          |
|         Realise par : Imene ALLOUCHE         |
|                                              |
|**********************************************/
void Random_String(int length, char chaine[])
{
    char alphabet[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    int i;
    for (i = 0; i < length; i++)
    {
        chaine[i] = alphabet[Random_Number(0, strlen(alphabet) - 1)];
    }
    chaine[length] = '\0';
}

/**********************************************|
|                                              |
|     Generer une chaine de longueur length    |
|         a partir d'un entier number          |
|        Realise par : Imene ALLOUCHE          |
|                                              |
|**********************************************/
void Generer_Chaine(char chaine[], int length, int number)
{
    int i;
    for (i = length - 1; i >= 0; i--)
    {
        chaine[i] = number % 10 + '0';
        number = number / 10;
    }
    chaine[length] = '\0';
}

/**********************************************|
|                                              |
|      Concatener tous les champs dans         |
|       une seule chaine destination           |
|       Realise par : Imene ALLOUCHE          |
|                                              |
|**********************************************/
void concatenate(char *destination, char *identifiant, char *fonctionne, char *materiel, char *prix, char *taille, char *description)
{
    /***************************************************************************************************|
    | Identifiant | champs fonctionne | Type materiel |    Prix   |   taille   | Description (variable) |
    |  (5 bytes)  |   (1 bytes)       |  (12 bytes)   | (6 bytes) |  (3 bytes) |  (max sur 273 bytes)   |
    |***************************************************************************************************/

    sprintf(destination, "%s", "");                          // vider le tableau des caractères
    strncat(destination, identifiant, TAILLE_IDENTIFIANT);   // destination+=identifiant
    strncat(destination, fonctionne, TAILLE_FONCTIONNEMENT); // destination+=fonctionne
    strncat(destination, materiel, TAILLE_MATERIEL);         // destination+=materiel
    strncat(destination, prix, TAILLE_PRIX);                 // destination+=prix
    strncat(destination, taille, TAILLE_TAILLE);             // destination+=taille
    strcat(destination, description);                        // destination+=description
}

/*










*/

/**********************************************|
|                                              |
| Ecrire la chaine d'identifiant cle a partir  |
|       du bloc i a la pos j dans Buf          |
|       Realise par : Imene ALLOUCHE          |
|                                              |
|**********************************************/
void Ecrire_chaine_TOVnC(fichier_TOVnC *F, char chaine[], int *i, int *j, Tampon_TOVnC *Buf)
{
    if (*j + strlen(chaine) > B) // inserer le nouvel element dans un nouveau bloc
    {
        EcrireDir_TOVnC(F, *i, *Buf);                     // Ecrire le bloc courant pour passer au prochain
        printf("\nNouveau bloc a ete cree");              // la creation d'un nouveau bloc
        *i = Alloc_bloc_TOVnC(F);                         // passer au nouveau bloc
        *j = 0;                                           // revenir a la premiere position dans le nouveau bloc
        memset(Buf->tableau, '\0', sizeof(Buf->tableau)); // vider la chaine de caracteres
    }
    strcat(Buf->tableau, chaine);                                // mise a jour du bloc: inserer la chaine de caractere
    Aff_Entete_TOVnC(F, 2, Entete_TOVnC(F, 2) + strlen(chaine)); // mise a jour de l'entere: nombre de caracteres inseres
    *j = *j + strlen(chaine);                                    // mise a jour du deplacement libre du buffer
    Buf->nb = *j;                                                // mise a jour du bloc: la permiere pos libre
}

/*










*/

/**********************************************|
|                                              |
| Ecrire la chaine d'identifiant cle a partir  |
|     du bloc i a la pos j dans Buf TOVC       |
|       Realise par : Imene ALLOUCHE          |
|                                              |
|**********************************************/
void Ecrire_Chaine_TOVC(fichier_TOVC *fichier, int *i, int *j, char chaine[], Tampon_TOVC *buf)
{
    int k;                               // variable pour parcourir la chaine caractere par caractere
    for (k = 0; k < strlen(chaine); k++) // k pour le deplacement dans la chaine
    {                                    // parcours
        if ((*j) < B)                    // si je suis toujours dans le bloc conserne
        {
            buf->tableau[*j] = chaine[k]; // affectation des caracteres de la chaine dans le buffer un a un
            (*j)++;                       // deplacement dans le buffer
        }
        else                                                  // si la chaine a inserer depasse le buffer
        {                                                     // nouveau bloc
            EcrireDir_TOVC(fichier, (*i), (*buf));            // ecriture du precedent buffer dans le fichier
            (*i) = Alloc_bloc_TOVC(fichier);                  // alocation d'un nouveau bloc afin de recevoir le reste de la chaine
            memset(buf->tableau, '\0', sizeof(buf->tableau)); // vider la chaine de caracteres
            buf->tableau[0] = chaine[k];                      // ecrtiture du k eme caractere de la chaine dans la position 0
            (*j) = 1;                                         // passage a la position 1
        }
    }
    buf->tableau[*j] = '\0';                                               // fin de la chaine
    Aff_Entete_TOVC(fichier, 3, Entete_TOVC(fichier, 3) + strlen(chaine)); // mise a jour de l'entere: nombre de caracteres inseres
    Aff_Entete_TOVC(fichier, 2, *j);                                       // mise a jour de l'entete: la 1ere pos libre dans le dernier bloc
}

/*










*/

/**********************************************|
|                                              |
| Ecrire la chaine d'identifiant cle a partir  |
|       du bloc i a la pos j dans Buf          |
|       Realise par : Imene ALLOUCHE          |
|                                              |
|**********************************************/
void Ecrire_Chaine_LOVC(fichier_LOVC *fichier, int *i, int *j, char chaine[], Tampon_LOVC *buf)
{
    int k;                               // variable pour parcourir la chaine caractere par caractere
    for (k = 0; k < strlen(chaine); k++) // k pour le deplacement dans la chaine
    {                                    // parcours
        if ((*j) < B)                    // si je suis toujours dans le bloc conserne
        {
            buf->tab[*j] = chaine[k]; // affectation des caracteres de la chaine dans le buffer un a un
            (*j)++;                   // deplacement dans le buffer
        }
        else                                          // si la chaine a inserer depasse le buffer
        {                                             // nouveau bloc
            ecrireDir_LOVC(fichier, *i, buf);         // ecriture du precedent buffer dans le fichier
            (*i) = alloc_bloc_LOVC(fichier, buf);     // alocation d'un nouveau bloc afin de recevoir le reste de la chaine
            memset(buf->tab, '\0', sizeof(buf->tab)); // vider la chaine de caracteres

            buf->tab[0] = chaine[k]; // ecrtiture du kiem caractere de la chaine dans la position 0
            (*j) = 1;                // passage a la position 1
        }
    }
    buf->tab[*j] = '\0';                                                   // fin de la chaine
    aff_entete_LOVC(fichier, 3, *j);                                       // mise a jour de l'entete: la 1ere pos libre dans le dernier bloc
    aff_entete_LOVC(fichier, 4, entete_LOVC(fichier, 4) + strlen(chaine)); // mise a jour de l'entere: nombre de caracteres inseres
}

/*










*/

/**************************************************|
|                                                  |
|    extraire une chaine de taille "taille" dans   |
|      la chaine destination a partir pos j        |
|          Realise par : Imene ALLOUCHE            |
|                                                  |
|**************************************************/
void extraire_chaine_TOVnC(char destination[], int *j, int taille, Tampon_TOVnC *Buf)
{
    int k = 0;                               // variable pour le parcours des caracteres
    sprintf(destination, "%s", "");          // vider la chaine destination
    for (k = 0; k < taille; k++)             // boucle de taille iterations correspondant a la longueur de la chaine
    {                                        // indice de parcours de la chaine resultata et j celui du tableau
        destination[k] = Buf->tableau[(*j)]; // recuperation du caractere dans la position k de la chaine
        (*j)++;                              // deplacement d'une position dans le buffer
    }                                        // fin de boucle
    destination[k] = '\0';                   // fin de la chaine obtenue
}

/*










*/

/**************************************************|
|                                                  |
|    extraire une chaine de taille "taille" dans   |
|      la chaine destination a partir pos j        |
|           Realise par : Imene ALLOUCHE           |
|                                                  |
|**************************************************/
void extraire_chaine_TOVC(fichier_TOVC *f, char destination[], int *i, int *j, int taille, Tampon_TOVC *Buf)
{
    int k = 0;                      // variable pour le parcours des caracteres
    sprintf(destination, "%s", ""); // vider la chaine destination
    for (k = 0; k < taille; k++)    // boucle de taille iterations correspondant a la longueur de la chaine
    {                               // indice de parcours de la chaine resultata et j celui du tableau
        if (*j >= B)                // si on est arrive a la fin du bug (chevauchement)
        {
            (*i) = (*i) + 1;          // aller au prochain bloc
            LireDir_TOVC(f, *i, Buf); // le lire
            printf("\n\n\n*****************************\n");
            printf("*                           *\n");
            printf("*   au bloc Numero:  %i      *\n", *i);
            printf("*                           *\n");
            printf("*****************************\n");
            *j = 0; // se placer au debut du bloc
        }
        destination[k] = Buf->tableau[(*j)]; // recuperation du caractere dans la position k de la chaine
        (*j)++;                              // deplacement d'une position dans le buffer
    }                                        // fin de boucle
    destination[k] = '\0';                   // fin de la chaine obtenue
}

/*










*/

/**************************************************|
|                                                  |
|    extraire une chaine de taille "taille" dans   |
|      la chaine destination a partir pos j        |
|           Realise par : Imene ALLOUCHE           |
|                                                  |
|**************************************************/
void extraire_chaine_LOVC(fichier_LOVC *f, char destination[], int *i, int *j, int taille, Tampon_LOVC *Buf)
{
    int k = 0;                      // variable pour le parcours des caracteres
    sprintf(destination, "%s", ""); // vider la chaine destination
    for (k = 0; k < taille; k++)    // boucle de taille iterations correspondant a la longueur de la chaine
    {                               // indice de parcours de la chaine resultata et j celui du tableau
        if (*j >= B)                // si on est arrive a la fin du bloc (chevauchement)
        {
            (*i) = Buf->suivant;      // aller au prochain bloc
            LireDir_LOVC(f, *i, Buf); // le lire
            *j = 0;                   // se placer au debut du bloc
        }
        destination[k] = Buf->tab[(*j)]; // recuperation du caractere dans la position k de la chaine
        (*j) = (*j) + 1;                 // deplacement d'une position dans le buffer
    }                                    // fin de boucle
    destination[k] = '\0';               // fin de la chaine obtenue
}

/*










*/
/**********************************************|
|                                              |
|       initialiser fichier nom_fichier        |
|        de type TOVnC avec n valeurs          |
|        Realise par : Imene ALLOUCHE          |
|                                              |
|**********************************************/
void Chargement_initial_TOVnC(char nom_fichier[], int n)
{
    fichier_TOVnC *F;
    Ouvrir_TOVnC(F, nom_fichier, 'N');
    Tampon_TOVnC buf;
    memset(buf.tableau, '\0', sizeof(buf.tableau));  // vider le tableau des caractères
    int i = Alloc_bloc_TOVnC(F),                     // le numero du bloc pour le parcours entre bloc
        j = 0,                                       // la position dans le bloc pour le parcours interbloc
        k,                                           // le numero de l'element insere de 1 a n
        l;                                           // la longueur total de l'enregistrement ajoute
    char Identifiant[TAILLE_IDENTIFIANT + 1],        // numero d'identifiant(cle)
        Fonctionne[TAILLE_FONCTIONNEMENT + 1] = "f", // fonctionne = 'f', le materiel marche, fonctionne = 'n' sinon
        Materiel[TAILLE_MATERIEL + 1],               // le type du materiel
        Prix[TAILLE_PRIX + 1],                       // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],                   // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1];     // la description (caracteristiques) du materiel

    for (k = 0; k < n; k++)
    {                                                                            // l'identifiant= multiple de 10 pour garder l'ordre et possibilite d'insertion
        Generer_Chaine(Identifiant, TAILLE_IDENTIFIANT, 10 * k);                 // generer l'identifiant sous forme de chaine sur 5 positions
        strcpy(Materiel, MATERIAL_LIST[Random_Number(0, NB_TYPE_MATERIEL - 1)]); // tirer un materiel de la liste  des materiels selon index genere aleartoirement
        Generer_Chaine(Prix, TAILLE_PRIX, Random_Number(0, PRIX_MAX));           // generer le prix du materiel aleartoirement
        l = Random_Number(1, TAILLE_MAX_DESCRIPTION / 2);                        // generer la taille de la description aleatoirement
        Random_String(l, Description);                                           // generer la description de taille l aleatoirement
        Generer_Chaine(Taille, TAILLE_TAILLE, l);                                // transformer le chaps taille de description a une chaine de caracteres

        printf("\n\n\n.........................\n");
        printf(".                       .\n");
        printf(". element numero : %i    .\n", k);
        printf(".                       .\n");
        printf(".........................\n");
        printf("identifiant: %.5s\n", Identifiant);
        printf("materiel: %.11s\n", Materiel);
        printf("prix: %.6s DA\n", Prix);
        printf("taille de description: %.3s\n", Taille);
        printf("description: %s\n", Description);

        l = TAILLE_IDENTIFIANT + TAILLE_FONCTIONNEMENT + TAILLE_MATERIEL + TAILLE_PRIX + TAILLE_TAILLE + strlen(Description);
        char Enreg[l];
        concatenate(Enreg, Identifiant, Fonctionne, Materiel, Prix, Taille, Description);
        printf("l'element sera insere sous cette forme: %s\n", Enreg);
        Ecrire_chaine_TOVnC(F, Enreg, &i, &j, &buf);
    }
    EcrireDir_TOVnC(F, i, buf); // ecrire le dernier buffer meme si il n'etait pas plein
    Fermer_TOVnC(F);            // fermer le fichier
}

/*










*/

/****************************************************|
|                                                    |
|   Recherche dun materiel selon son identifiant     |
|          Realise par : Ibtissam SEFFAH             |
|                                                    |
|****************************************************/
void Recherche_TOVnC(char nom_fichier[], char Identifiant_Recherche[], int *trouv, int *i, int *j)
{
    fichier_TOVnC f;
    Ouvrir_TOVnC(&f, nom_fichier, 'A');
    int binf = 1,                                // le premier bloc
        bsup = Entete_TOVnC(&f, 1),              // le dernier bloc
        temp_j,                                  // sauvegarder la pos dans le bloc avant de se deplacer
        stop = 0;                                // booleen pour arreter la recherche si on arrive a une cle superieure a la cle recherche
    char Cle_Min[TAILLE_IDENTIFIANT + 1],        // la plus petite cle dans un bloc (plus petit identifiant)
        Cle_Courrante[TAILLE_IDENTIFIANT + 1],   // la cle courrant dont on s'est arrete dans le parcours
        Materiel[TAILLE_MATERIEL],               // le type du materiel
        Fonctionne[TAILLE_FONCTIONNEMENT + 1],   // fonctionne = 'f', le materiel marche, fonctionne = 'n' sinon
        Prix[TAILLE_PRIX + 1],                   // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],               // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1]; // la description (caracteristiques) du materiel
    Tbloc_TOVnC Buf;                             // un buffer pour charger un bloc de MS vers MC
    *trouv = 0;                                  // initialiser trouv vers faux
    while (!(*trouv) && binf <= bsup && !stop)
    { // recherche dichotomique entre les bloc tq materiel non trouve et recherche possible(cle_courante<cle_recherche)

        *i = (binf + bsup) / 2;                                          // numero de bloc a parcourir
        *j = 0;                                                          // la premiere position dans le bloc
        LireDir_TOVnC(&f, *i, &Buf);                                     // lire le buffer
        temp_j = *j;                                                     // sauvegarder j avant de se deplacer
        extraire_chaine_TOVnC(Cle_Min, &(*j), TAILLE_IDENTIFIANT, &Buf); // extraire la plus petite cle (premiere cle) de du bloc i
        strcpy(Cle_Courrante, Cle_Min);                                  // mettre a jour la cle courrante

        if (atoi(Identifiant_Recherche) >= atoi(Cle_Min))                                              // si la cle à recherchee est entre Cle_Min et Cle_Max du bloc
        {                                                                                              // alors on recherche la cle dans ce bloc
            while (!(*trouv) && *j < Buf.nb && !stop)                                                  // stop = cle_courante > cle_recherche
            {                                                                                          // recherche sequentielle dans le bloc
                extraire_chaine_TOVnC(Fonctionne, j, TAILLE_FONCTIONNEMENT, &Buf);                     // recuperer un le champs de suppression
                if (strcmp(Identifiant_Recherche, Cle_Courrante) == 0 && strcmp(Fonctionne, "f") == 0) // la cle est donc trouveee dans le bloc i
                {
                    extraire_chaine_TOVnC(Materiel, j, TAILLE_MATERIEL - 1, &Buf); // extraire le materiel
                    extraire_chaine_TOVnC(Prix, j, TAILLE_PRIX, &Buf);             // extraire son prix
                    extraire_chaine_TOVnC(Taille, j, TAILLE_TAILLE, &Buf);         // extraire la taille de la description
                    extraire_chaine_TOVnC(Description, j, atoi(Taille), &Buf);     // extraire la description
                    *trouv = 1;                                                    // mettre trouve a vrai pour arreter la recherche
                    *j = temp_j;                                                   // faire revenir j a la position du premier champs avant decalage (l'identifiant)
                    printf("\n\n---------------- L'enregistrement trouve dans le bloc %d a la position %d ------------------\n", *i, *j);
                    printf("|    -> L'identifiant : %s\n", Cle_Courrante);                 // afficher la cle
                    printf("|    -> Le type materiel : %s\n", Materiel);                   // afficher le materiel
                    printf("|    -> Fonctionnement du materiel: En marche\n", Fonctionne); // le materiel est en marche
                    printf("|    -> Le prix d'achat du materiel : %s DA\n", Prix);         // afficher le prix
                    printf("|    -> La taille de la description : %s\n", Taille);          // afficher le prix
                    printf("|    -> La Description : %s\n", Description);                  // afficher la description
                    printf("---------------------------------------------------------------------------------------------\n\n");
                }
                else                                                       // si la cle courante n'est pas egale a la cle recherche
                {                                                          // alors
                    if (atoi(Identifiant_Recherche) < atoi(Cle_Courrante)) // si on a depasse l'ordre de la cle recherche
                    {                                                      // donc
                        stop = 1;                                          // la cle surement n'existe pas stop=vrai, on arrete la recherche
                        *j = temp_j;                                       // faire revenir j a la position du premier champs avant decalage (l'identifiant)
                    }
                    else                                                           // sinon on passe au prochain materiel dans le meme bloc toujours
                    {                                                              // alors
                        *j = *j + TAILLE_MATERIEL - 1 + TAILLE_PRIX;               // zapper les champs: materiel, fonctionnement, taille
                        extraire_chaine_TOVnC(Taille, &(*j), TAILLE_TAILLE, &Buf); // recuperer la taille de la description
                        *j = *j + atoi(Taille);                                    // zapper la description egalement
                        temp_j = *j;
                        if (*j < Buf.nb)
                            extraire_chaine_TOVnC(Cle_Courrante, &(*j), TAILLE_IDENTIFIANT, &Buf); // recuperer la cle prochaine a verifier
                    }
                }
            }
            if (*j >= Buf.nb)                   // avant de juger que le placement cle n'est pas dans ce bloc
            {                                   // on veut regler les valeur de i et j (bloc et pos ou la cle existe ou devait exister)
                if ((*i) < Entete_TOVnC(&f, 1)) // si ce n'est pas le dernier bloc
                {
                    temp_j = Buf.nb;                                             // voir la cle du bloc prochain
                    *j = 0;                                                      // peut etre que si la cle recherche n'existe pas elle doit etre a la fin du bloc parcourru
                    LireDir_TOVnC(&f, (*i) + 1, &Buf);                           // donc on lie le bloc prochain
                    extraire_chaine_TOVnC(Cle_Min, j, TAILLE_IDENTIFIANT, &Buf); // on verirife la cle min du bloc prochain
                    if (strcmp(Cle_Min, Identifiant_Recherche) > 0)              // si la cle min du bloc prochain > a la cle recherche
                    {
                        stop = 1;    // donc le i doit retourner le bloc precedent
                        *j = temp_j; // avec la pos du buf.nb qui veut dire la fin du bloc
                    }
                    else               // si la cle recherche < a la cle min du bloc
                        binf = *i + 1; // cle  doit être après le bloc courant
                }
                else
                    stop = 1;
            }
        }
        else               // si la cle recherche > a la cle min du bloc
            bsup = *i - 1; // la cle doit être avant le bloc courant
    }
    if (binf > bsup && !stop) // si on est sortie a cause des chevauchement des base
    {
        *i = binf;   // le bloc ou devait exister l'identifiant = binf
        *j = temp_j; // la pos ou devait commencer le materiel = temp_j
        stop = 1;
    }
    if (stop)
    {
        printf("\n\n---------------- L'enregistrement n'exsite pas, il devait exister: ------------------\n", *i, *j);
        printf("|    -> Dans le bloc numero : %i\n", *i);
        printf("|    -> Dans la position : %i\n", *j);
        printf("-------------------------------------------------------------------------------------\n\n");
    }
    Fermer_TOVnC(&f);
}

/*













*/
/********************************************************|
|                                                        |
|   insertion d’un matériel dans le fichier de données   |
|           Realise par : Ibtissam SEFFAH                |
|                                                        |
|********************************************************/
/***********************************************|
|                                               |
| COPIER LA CHAINE str DANS LA CHAINE tableau   |
| A PARTIR DE LA POSITION pos                   |
|                                               |
|***********************************************/

void ins_string(char tableau[B], int pos, char str[B]) // module à utilisé pour l'insertion
{
    int i = 0;
    while (i < strlen(str))
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
void Insertion_TOVnC(char nom_fichier[], char chaine[]) // procédure pour inserer une chaine dans un fichier , la chaine represente le materiel
{
    // collecter les informations necessaires sur le nouveau mateirel a inserer

    int i, j,      // l'emplacement ou on va inserer le matereil(i:le bloc , j: la posistion)
        l, k, cpt, // des variables seront utilisés pour le decalage
        ident,
        trouv;           // boolean utilisé pour la recherche du materiel qu'on veut l'inserer
    int stop = 1;        // booleen pour continuer l'operation de l'insertion
    int taille_materiel; // taille du materiel qu'on veut l'inserer
    int TAILLE_chaine1;
    char IDENTIFIANT[TAILLE_IDENTIFIANT + 1];
    char chaine1[B + 1];

    fichier_TOVnC f;
    Tbloc_TOVnC Buf;

    // mettre notre identifiant de la chaine  dans 'key' pour ensuite l'utiliser pour la recherche

    IDENTIFIANT[0] = chaine[0];
    IDENTIFIANT[1] = chaine[1];
    IDENTIFIANT[2] = chaine[2];
    IDENTIFIANT[3] = chaine[3];
    IDENTIFIANT[4] = chaine[4];
    // ident=atoi(IDENTIFIANT);                                         // taille de la cle de la chaine qu'on veut l'inserer (in case we need it )

    taille_materiel = strlen(chaine); // affecter à taille_materiel la taille du materiel (chaine) qu'on va insérer
    // Ouvrir_TOVnC(&f, FICHIER_ORIGINAL , 'A');
    Recherche_TOVnC(nom_fichier, IDENTIFIANT, &trouv, &i, &j); // on fait la recherche de la cle du materiel qu'on veut l'inserer

    if (!trouv)
    { // chaine doit être inséré dans le bloc i à la position j
        while (stop && i <= Entete_TOVnC(&f, 1))
        {
            LireDir_TOVnC(&f, i, &Buf); // on lit le bloc ou la chaine doit etre inserer

            /**  si l'espace libre peut contenir le materiel, on décale si nécessaire et puis on insere le materiel  **/
            if ((B - Buf.nb) >= taille_materiel)
            { // l'éspace libre est plus grand ou egale à la taille du materiel qu'on veut l'inserer
                l = Buf.nb;
                k = Buf.nb + taille_materiel;
                cpt = 1;
                while (cpt <= Buf.nb)
                {
                    Buf.tableau[k] = Buf.tableau[l]; // on fait le decalage des materiaux qui viennent juste apres le materiel qu'on va l'inserer
                    k--;
                    l--;
                    cpt++;
                }
                ins_string(Buf.tableau, j, chaine); // insertion du materiel
                Buf.nb = Buf.nb + taille_materiel;  // mise à jour de la position libre dans un bloc (buf.nb)
                EcrireDir_TOVnC(&f, i, Buf);
                stop = 0;
            }

            /** si l'espace libre ne peut pas contenir le materiel [(taille du materiel) > (B-buf.nb)] **/
            else
            {
                /** si la taille du materiel + position ou il faut l'inserer(j) > B , on prend le materiel qu'on veut l'inserer et les matererls qui viennent apres comme
                une nouvelle chaine à inserer **/
                if ((j + taille_materiel) > B)
                {
                    TAILLE_chaine1 = (Buf.nb - j);                           // la taille des enregistrements qui viennent juste apres le meterile qu'on veut l'inserer
                    extraire_chaine_TOVnC(chaine1, j, TAILLE_chaine1, &Buf); // on fait sortir ces derniers enregistrements du bloc pour les inserer avec le materiel
                                                                             // qu'on veut l'inserer
                    Buf.nb = j;                                              // mise à jour de la position libre dans un bloc (buf.nb)
                    EcrireDir_TOVnC(&f, i, Buf);
                    strcat(chaine, chaine1); // nouveau materiel à inserer (chaine) dans le prochain bloc = materiel qu'on veut inserer (chaine)+ les materierls
                                             // qui viennent  apres ce dernier(chaine1)
                    i = i + 1, j = 0;        //  l'insertion se fera à la prochaine itération du TQ , dans le prochain bloc à la premiere position
                }
                else
                {
                    /** si la taille du materiel + position ou il faut l'inserer(j) =< B **/
                    TAILLE_chaine1 = (Buf.nb - j);
                    extraire_chaine_TOVnC(chaine1, j, TAILLE_chaine1, &Buf);
                    ins_string(Buf.tableau, j, chaine); // on insere le materiel
                    Buf.nb = j + taille_materiel;       // mise à jour la position libre (buf.nb)
                    EcrireDir_TOVnC(&f, i, Buf);
                    strcpy(chaine, chaine1); // le nouveau materiel qui va etre inserer dans le prochain bloc (chaine reçoit chaine1)
                    i = i + 1, j = 0;
                    /*___________________________________________________________________________________________
                     REMARQUE: on a voulu laiser de vide dans le bloc pour faciliter les prochaines insertions
                    ( évitant les décalages  )
                     ____________________________________________________________________________________________*/
                }
            }
        }
    }

    /** si on dépasse la fin de fichier, on rajoute un nouveau bloc contenant la chaine**/
    if (i > Entete_TOVnC(&f, 1))
    {
        i == Alloc_bloc_TOVnC(&f);
        strcpy(Buf.tableau, chaine1); // insertion chaine
        Buf.nb == TAILLE_chaine1;     // mise à jour de la position libre dans un bloc (buf.nb)
        EcrireDir_TOVnC(&f, i, Buf);
    }
    Aff_Entete_TOVnC(&f, 2, Entete_TOVnC(&f, 2) + taille_materiel); // Entete_TOVnC(f,2) : nb d'insertion  , on incrémente le compteur d'insertions
    Fermer_TOVnC(&f);
}

/*










*/

/**********************************************|
|                                              |
|      Supprimer un materiel logiquement       |
|            selon son identifiant             |
|        Realise par : Imene ALLOUCHE          |
|                                              |
|**********************************************/
void Suppression_TOVnC(char nom_fichier[], char identifiant_a_supprimer[])
{
    /***************************************************************************************************|
    | Identifiant | champs fonctionne | Type materiel |    Prix   |   taille   | Description (variable) |
    |  (5 bytes)  |   (1 bytes)       |  (12 bytes)   | (6 bytes) |  (3 bytes) |  (max sur 273 bytes)   |
    |***************************************************************************************************/
    int trouv, // booleen pour verifier si le
        i,     // le numero du bloc pour le parcours entre bloc
        j;     // la position dans le bloc pour le parcours interbloc

    Recherche_TOVnC(nom_fichier, identifiant_a_supprimer, &trouv, &i, &j); // rechercher l'identifiant
    if (trouv)                                                             // si il existe alors on va le supprimer logiquement
    {
        fichier_TOVnC F;                    // fichier TOVnC
        Ouvrir_TOVnC(&F, nom_fichier, 'A'); // ouvrir le fichier de "nom fichier"
        Tampon_TOVnC buf;                   // un buffer pour charger le contenu de la MS vers la MC
        char Taille[TAILLE_TAILLE + 1];     // champs taille pour remettre l'entete a jour avec le nombre de char supprime
        LireDir_TOVnC(&F, i, &buf);         // lire le bloc retenu par la recherche
        j += TAILLE_IDENTIFIANT;            // sauter le champs identifiant
        buf.tableau[j] = 'n';               // remettre le champs fonctionne a faux

        j += TAILLE_FONCTIONNEMENT + TAILLE_MATERIEL + TAILLE_PRIX; // sauter tous les champs jusqèa champs taille
        extraire_chaine_TOVnC(Taille, &j, TAILLE_TAILLE, &buf);     // extraire le champs taille de description
        int Nombre_chars_supprime = TAILLE_IDENTIFIANT + TAILLE_FONCTIONNEMENT + TAILLE_MATERIEL - 1 + TAILLE_PRIX + atoi(Taille);

        Aff_Entete_TOVnC(&F, 3, Entete_TOVnC(&F, 3) + Nombre_chars_supprime); // mettre le nombre de char supprime dans le fichier a jour
        EcrireDir_TOVnC(&F, i, buf);                                          // ecrire le buffer ne MS aprés la mise a jour du champs supprimer
        Fermer_TOVnC(&F);                                                     // fermer le fichier
        printf("\nle materiel a ete supprime");                               // pour informer l'audience
    }                                                                         // mais
    else                                                                      // si le materiel n'existe pas d'origine
        printf("\nle materiel n'existe pas, aucune suppression n'a eu lieu"); // alors informer mon audience qu'aucune suppression n'a eu lieu
}

/*










*/

/**********************************************|
|                                              |
|       reorganiser un fichier TOVnC selon     |
|          le champs de fonctionnement         |
|         Realise par : Imene ALLOUCHE         |
|                                              |
|**********************************************/
void Reorganisation_TOVnC(char nom_fichier[], char nom_fichier1[], char nom_fichier2[])
{
    fichier_TOVnC f;                                  // le fichier original qu'on doit reorganiser "Materiel_informatique_TOVnC.bin"
    Ouvrir_TOVnC(&f, nom_fichier, 'A');               // ouvrir le fichier TOVnC
    fichier_TOVC f1;                                  // le fichier qui contient le materiel informatique ne marche
    Ouvrir_TOVC(&f1, nom_fichier1, 'N');              // ouvrir le fichier TOVC
    fichier_LOVC f2;                                  // le fichier qui contient le materiel informatique en panne
    Ouvrir_LOVC(&f2, nom_fichier2, 'N');              // ouvirir le fichier LOVC
    Tampon_TOVnC Buf;                                 // un buffer en MC pour le fichier TOVnC
    Tampon_TOVC Buf1;                                 // un buffer en MC pour le fichier TOVC
    Tampon_LOVC Buf2;                                 // un Buffer en MC pour le fichier LOVC
    memset(Buf1.tableau, '\0', sizeof(Buf1.tableau)); // vider le tableau BUf1
    memset(Buf2.tab, '\0', sizeof(Buf2.tab));         // vider le tableau Buf2
    int i = 1,                                        // le parcours des blocs du fichier original TOVnC
        j = 0,                                        // le parcours de position du fichier original TOVnC
        i1 = Alloc_bloc_TOVC(&f1),                    // le parcours des blocs du fichier original TOVC
        j1 = 0,                                       // le parcours de position du fichier original TOVC
        i2 = alloc_bloc_LOVC(&f2, &Buf2),             // le parcours des blocs du fichier original LOVC
        j2 = 0;                                       // le parcours de position du fichier original LOVC

    char Identifiant[TAILLE_IDENTIFIANT + 1],    // numero d'identifiant(cle)
        Fonctionne[TAILLE_FONCTIONNEMENT + 1],   // fonctionne = 'f', le materiel marche, fonctionne = 'n' sinon
        Materiel[TAILLE_MATERIEL],               // le type du materiel
        Prix[TAILLE_PRIX + 1],                   // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],               // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1], // la description (caracteristiques) du materiel
        Destination[B];                          // la chaine ou on fera la concatenation des champs pour l'ecrire ensuite dans le Buf
    while (i <= Entete_TOVnC(&f, 1))             // tant que on n'est pas arrive a la fin du fichier
    {
        LireDir_TOVnC(&f, i, &Buf); // lire le i eme bloc
        while (j < Buf.nb)          // parcourir le i eme bloc jusqu'a la premiere position libre (Buf.nb)
        {
            extraire_chaine_TOVnC(Identifiant, &j, TAILLE_IDENTIFIANT, &Buf);   // extraire le champs Identifiant
            extraire_chaine_TOVnC(Fonctionne, &j, TAILLE_FONCTIONNEMENT, &Buf); // extraire le champs d'etat de fonctionnement
            extraire_chaine_TOVnC(Materiel, &j, TAILLE_MATERIEL - 1, &Buf);     // extraire le champs materiel
            extraire_chaine_TOVnC(Prix, &j, TAILLE_PRIX, &Buf);                 // extraire le champs du prix
            extraire_chaine_TOVnC(Taille, &j, TAILLE_TAILLE, &Buf);             // extraire le champs taille de description
            extraire_chaine_TOVnC(Description, &j, atoi(Taille), &Buf);         // extraire le champs description

            concatenate(Destination, Identifiant, "", Materiel, Prix, Taille, Description);

            /*********************************************************************************|
            |  Identifiant |  Type materiel |   Prix    |   taille   | Description (variable) |
            |   (5 bytes)  |    (12 bytes)  | (6 bytes) |  (3 bytes) |  (max sur 273 bytes)   |
            |*********************************************************************************/

            if (strcmp(Fonctionne, "f") == 0) // si le materiel n'a pas ete supprime logiquement (fonctionne)
            {                                 // le mettre dans le fichier des materiaux en marche (TOVC)
                Ecrire_Chaine_TOVC(&f1, &i1, &j1, Destination, &Buf1);
            }
            else // si le materiel a ete supprime (ne fonctionne pas)
            {    // le mettre dans le fichier des materiaux en panne (LOVC)
                Ecrire_Chaine_LOVC(&f2, &i2, &j2, Destination, &Buf2);
            }
        }
        j = 0;
        i++;
    }

    EcrireDir_TOVC(&f1, i1, Buf1);  // ecrire dernier buffer du fichier des materiaux en marche
    ecrireDir_LOVC(&f2, i2, &Buf2); // ecrire dernier buffer du fichier des materiaux en marche
    Fermer_TOVC(&f1);
    fermer_LOVC(&f2);
    Fermer_TOVnC(&f);
}

/*


































*/
/*********************************************************|
|                                                         |
|         FONCTIONS IMPLEMENTES POUR PARTIE 02            |
|                                                         |
|*********************************************************/
/************************************************|
|                                                |
|    Inserer un Enregistrement dans le bloc i    |
|      dans l'enreg num j dans fichier TOF       |
|         Realise par : Imene ALLOUCHE           |
|                                                |
|************************************************/
void Inserer_Enreg_TOF(fichier_TOF *f, Tenreg_TOF Enregistrement_TOF, int *i, int *j, Tampon_TOF *Buf)
{
    printf("\nbloc %i pos %i", *i, *j);
    if (*j > MAX_ENREG)
    {
        Buf->nombre_enreg = MAX_ENREG;
        EcrireDir_TOF(f, *i, Buf); // ecrire le buf i                                                                                      // incrementer le i
        *i = Alloc_bloc_TOF(f);    // nouveau bloc + mise a jour de l'entete
        *j = 0;
    }
    Buf->tab[*j].Identifiant = Enregistrement_TOF.Identifiant; // mise a jour du Buf: champs identifiant
    Buf->tab[*j].Prix = Enregistrement_TOF.Prix;               // mise a jour du Buf: champs Prix
    Buf->tab[*j].supprimer = Enregistrement_TOF.supprimer;     // mise a jour du Buf: champs Supprimer

    Buf->nombre_enreg = *j;                     // mise a jour du Buf: nombre d'enreg dans le buf
    *j = *j + 1;                                // aller a la prochaine pos libre dans le Buf
    Aff_Entete_TOF(f, 2, Entete_TOF(f, 2) + 1); // mise a jour de l'entete: nombre d'enregistrement inseres++
}

/*










*/

/************************************************|
|                                                |
|  Generer fichiers TOF a partir du fichier TOVC |
|     "nom_fichier" selon le champ materiel      |
|         Realise par : Imene ALLOUCHE           |
|                                                |
|************************************************/
void Generation_fichiers_Materiel(char nom_fichier[])
{
    int k,     // parcourir tous les materiel existants (6 types de materiels)
        i = 1, // parcours entre-blocs du fichier TOVC du materiel en marche
        j = 0, // parcours inter-blocs du fichier TOVC du materiel en marche
        trouv; // si on a trouve le nom du materiel dans la liste des types de materiel

    fichier_TOVC f;                // le fichier TOVC des materiels en marche qu'on parcourrira
    Tampon_TOVC Buf;               // buffer avec lequel on parourrira le fichier de materiel en marche TOVC
    Tenreg_TOF Enregistrement_TOF; // enregistrement des fichiers TOFs formés de 3 champs: identifiant, prix, supprimer

    char Identifiant[TAILLE_IDENTIFIANT + 1],    // numero d'identifiant(cle)
        Materiel[TAILLE_MATERIEL],               // le type du materiel
        Prix[TAILLE_PRIX + 1],                   // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],               // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1]; // la description (caracteristiques) du materiel

    Ouvrir_TOVC(&f, nom_fichier, 'A'); // ouvrir le fichier TOVC du materiel en marche qu'on parcourira

    for (k = 0; k < NB_TYPE_MATERIEL; k++) // Ouvrir tous les fichiers TOFs et initialiser leurs champs associes
    {
        strcpy(Files[k].Materiel, MATERIAL_LIST[k]);                                      // affcter le nom du materiel
        sprintf(Files[k].file_name, "Materiel_en_marche_%s_TOF.bin\0", MATERIAL_LIST[k]); // generer le nom du fichier selon le nom du materiel
        Ouvrir_TOF(&(Files[k].f), Files[k].file_name, 'N');                               // ouvrir le fichier correspondant en mode nouveau

        Files[k].i = Alloc_bloc_TOF(&(Files[k].f)); // initialisation de numero du bloc ou affecter
        Files[k].j = 0;                             // initialisation de la position ou affecter
        Files[k].Buf.nombre_enreg = 0;              // initialiser le nombre d'enregistrement dans le buf courant(buf numero 1) a 0
    }

    LireDir_TOVC(&f, i, &Buf);
    while (!(i == Entete_TOVC(&f, 1) && j >= Entete_TOVC(&f, 2))) // tant que on est pas arrive a la fin du fichier
    {
        /***************************************************************************|
        | Identifiant | Type materiel |   Prix    |   taille   |    Description     |
        |  (5 bytes)  |   (12 bytes)  | (6 bytes) |  (3 bytes) |    (variable)      |
        |***************************************************************************/
        extraire_chaine_TOVC(&f, Identifiant, &i, &j, TAILLE_IDENTIFIANT, &Buf);
        extraire_chaine_TOVC(&f, Materiel, &i, &j, TAILLE_MATERIEL - 1, &Buf);
        extraire_chaine_TOVC(&f, Prix, &i, &j, TAILLE_PRIX, &Buf);
        extraire_chaine_TOVC(&f, Taille, &i, &j, TAILLE_TAILLE, &Buf);
        extraire_chaine_TOVC(&f, Description, &i, &j, atoi(Taille), &Buf);

        Enregistrement_TOF.Identifiant = atoi(Identifiant); // mise a jour de l'enreg a inserer: champ identifiant
        Enregistrement_TOF.Prix = atoi(Prix);               // mise a jour de l'enreg a inserer: champ Prix
        Enregistrement_TOF.supprimer = 0;                   // mise a jour de l'enreg a inserer: champ Supprimer

        trouv = 0; // pour arreter si on a trouve le fichier correspondant au type du materiel qu'on veut inserer
        k = 0;     // le numero du materiel dans la liste des types de materiel

        while (!trouv && k < NB_TYPE_MATERIEL) // tant qu'on a pas trouve le fichier correspondant au materiel qu'on veut inserer
        {
            if (strcmp(Files[k].Materiel, Materiel) == 0) // si on a trouve le fichier
            {
                trouv = 1; // fichier correspondant trouve
                Inserer_Enreg_TOF(&(Files[k].f), Enregistrement_TOF, &(Files[k].i), &(Files[k].j), &(Files[k].Buf));
            }
            else // si on a pas trouve le fichier aller au prochain fichier
                k++;
        }
    }

    for (k = 0; k < NB_TYPE_MATERIEL; k++) // fermiture de tous les fichier TOFs
    {
        EcrireDir_TOF(&(Files[k].f), Files[k].i, &(Files[k].Buf));
        Fermer_TOF(&(Files[k].f));
    }

    Fermer_TOVC(&f); // fermer le fichier des materiel en marche TOVC
}

/*










*/
/**************************************************|
|                                                  |
|     Requette a intervalle par rapport au prix    |
|    [Prix_Min , Prix_Min] dans un fichier LOVC    |
|          Realise par : Ibtissam SEFFAH           |
|                                                  |
|**************************************************/
// FICHIER_MATERIEL_NON_FONCTIONNE = "Materiel_informatique_en_panne_LOVC.bin"
void Requette_intervalle_LOVC(char nom_fichier[], int Prix_Min, int Prix_Max, int *montant)
{

    fichier_LOVC f;
    Ouvrir_LOVC(&f, nom_fichier, 'A');
    int i = entete_LOVC(&f, 1), // parcours entre bloc du fichier LOVC du materiel en panne
        j = 0;                  // parcours inter-bloc du fichier LOVC du materiel en panne
    *montant = 0;               // pour le calcul du montant annuel du materiel dont le prix est compris en prix min et prix max

    char Identifiant[TAILLE_IDENTIFIANT + 1],    // la plus petite cle dans un bloc (plus petit identifiant)
        Materiel[TAILLE_MATERIEL],               // le type du materiel
        Prix[TAILLE_PRIX + 1],                   // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],               // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1]; // la description (caracteristiques) du materiel

    Tampon_LOVC buf;           // un buffer pour charger un bloc de MS vers MC
    LireDir_LOVC(&f, i, &buf); // lire le premier buffer buffer

    while (!(i == entete_LOVC(&f, 2) && j >= entete_LOVC(&f, 3))) // tant que on est pas arrive a la fin du fichier
    {

        /*********************************************************************************|
        | Identifiant  | Type materiel |    Prix   |   taille   | Description (variable)  |
        |  (5 bytes)   |  (12 bytes)   | (6 bytes) |  (3 bytes) |  (max sur 272 bytes)    |
        |*********************************************************************************/
        // entete_LOVC(f, 3) represente la position libre dans le dernier bloc "pos_libre_dernier_bloc"

        extraire_chaine_LOVC(&f, Identifiant, &i, &j, TAILLE_IDENTIFIANT, &buf); // extraire IDENTIFIANT
        extraire_chaine_LOVC(&f, Materiel, &i, &j, TAILLE_MATERIEL - 1, &buf);   // extraire le materiel
        extraire_chaine_LOVC(&f, Prix, &i, &j, TAILLE_PRIX, &buf);               // extraire son prix
        extraire_chaine_LOVC(&f, Taille, &i, &j, TAILLE_TAILLE, &buf);           // extraire la taille de la description
        extraire_chaine_LOVC(&f, Description, &i, &j, atoi(Taille), &buf);       // extraire la description

        if (Prix_Min <= atoi(Prix) && atoi(Prix) <= Prix_Max)
        { //  le pris doit etre compris entre ces deux valeurs : Pris_Max et Pris_Min

            (*montant) = (*montant) + atoi(Prix);

            printf("\n\n----------- Materiel trouve dans le bloc %i a la pos %i dont le prix est compris [%i , %i]: ------------\n\n", i, j, Prix_Min, Prix_Max);
            printf("|    -> L'identifiant : %s\n", Identifiant);           // afficher la cle
            printf("|    -> Le type materiel : %s\n", Materiel);           // afficher le materiel
            printf("|    -> Le prix d'achat du materiel : %s DA\n", Prix); // afficher le prix
            printf("|    -> La taille de la description : %s\n", Taille);  // afficher la taille de la description
            printf("|    -> La Description : %s\n", Description);          // afficher la description
            printf("----------------------------------------------------------------------------------------------------------\n\n");
        }
    }
    printf("\n\n-----------------------------------------------\n");
    printf("|    -> le montant global est : %d DA\n", *montant); //  le montant global de tous ces  matériels affichés
    printf("-----------------------------------------------\n\n");

    fermer_LOVC(&f);
}

/*














*/
int main(void)
{
    srand(time(NULL));
    printf("a printing is needed");
    int k;
    // Generation_fichiers_Materiel(FICHIER_MATERIEL_FONCTIONNE);
    Requette_intervalle_LOVC(FICHIER_MATERIEL_NON_FONCTIONNE, 20000, 900000, &k);
    // afficher_fichier_LOVC(FICHIER_MATERIEL_NON_FONCTIONNE);
    /*
    for (k = 0; k < NB_TYPE_MATERIEL; k++)
    {
        strcpy(Files[k].Materiel, MATERIAL_LIST[k]);                                      // affcter le nom du materiel
        sprintf(Files[k].file_name, "Materiel_en_marche_%s_TOF.bin\0", MATERIAL_LIST[k]); // generer le nom du fichier selon le nom du materiel
        afficher_fichier_TOF(Files[k].file_name);
    }
    */
}