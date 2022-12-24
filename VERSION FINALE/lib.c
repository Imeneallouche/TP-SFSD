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

    affichage_entete_TOVnC(nom_fichier);

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
|           FICHIER TYPE TOVC         |
|                                     |
|*************************************/
/*************************************************************|
|                                                             |
|  Ouvrir fichier nom_fichier avec le mode correspondant TOVC |
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

    affichage_entete_TOVC(nom_fichier);

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

    affichage_entete_LOVC(nom_fichier);

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

/*************************************|
|                                     |
|           FICHIER TYPE TOF          |
|                                     |
|*************************************/
/************************************************************|
|                                                            |
|  Ouvrir fichier nom_fichier avec le mode correspondant TOF |
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
|                                            |
|********************************************/
void LireDir_TOF(fichier_TOF *f, int i, Tampon_TOF *buf)
{
    rewind(f->fichier);
    fseek(f->fichier, sizeof(entete_TOF) + (i - 1) * sizeof(Tampon_TOF), SEEK_SET);
    fread(buf, sizeof(Tampon_TOF), 1, f->fichier);
}

/*********************************************|
|                                             |
|      Ecrire buf dans le i eme bloc TOF      |
|                                             |
|*********************************************/
void EcrireDir_TOF(fichier_TOF *f, int i, Tampon_TOF buf)
{
    rewind(f->fichier);
    fseek(f->fichier, sizeof(entete_TOF) + (i - 1) * sizeof(Tampon_TOF), SEEK_SET);
    fwrite(&buf, sizeof(Tampon_TOF), 1, f->fichier);
}

/********************************************|
|                                            |
|  Retoure la i ème valeur del'entete  TOF   |
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

    /************************************************|
    |  Identifiant  |     Prix    |     Supprimer    |
    |   (5 bytes)   |  (integer)  |     (1 bytes)    |
    |************************************************/

    affichage_entete_TOF(nom_fichier);

    while (i <= Entete_TOF(&f, 1))
    {
        LireDir_TOF(&f, i, &Buf);
        j = 0;
        printf("\n\n\n*************************************************\n");
        printf("*                                               *\n");
        printf("*            Le bloc numero : %i                 *\n", i);
        printf("*        rempli a %i / %i enregistrements         *\n", Buf.nombre_enreg, MAX_ENREG);
        printf("*                                               *\n");
        printf("*************************************************\n");

        while (j < Buf.nombre_enreg)
        {
            printf("\n\n.........................\n");
            printf(".                       .\n");
            printf(".  Materiel numero : %i  .\n", counter);
            printf(".                       .\n");
            printf(".........................\n");
            printf("identifiant: %s\n", Buf.tab[j].Identifiant);
            printf("prix: %i DA\n", Buf.tab[j].Prix);
            if (Buf.tab[j].supprimer == 0)
                printf("Supprime? : NON\n");
            else
                printf("Supprime? : OUI\n");

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
|    FICHIER Index Type TOF avec      |
|   structure d'enreg differente      |
|                                     |
|*************************************/
/********************************************|
|                                            |
|      Lire le i eme bloc dans buf Index     |
|                                            |
|********************************************/
void LireDir_Index_TOF(fichier_TOF *f, int i, Tampon_INDEX *buf)
{
    rewind(f->fichier);
    fseek(f->fichier, sizeof(entete_TOF) + (i - 1) * sizeof(Tampon_INDEX), SEEK_SET);
    fread(buf, sizeof(Tampon_INDEX), 1, f->fichier);
}

/*********************************************|
|                                             |
|     Ecrire buf dans le i eme bloc Index     |
|                                             |
|*********************************************/
void EcrireDir_Index_TOF(fichier_TOF *f, int i, Tampon_INDEX *buf)
{
    rewind(f->fichier);
    fseek(f->fichier, sizeof(entete_TOF) + (i - 1) * sizeof(Tampon_INDEX), SEEK_SET);
    fwrite(buf, sizeof(Tampon_INDEX), 1, f->fichier);
}

/**********************************************|
|                                              |
|     affichier le contenu d'un fichier TOF    |
|         avec structure Tampon_Index          |
|                                              |
|**********************************************/
void afficher_fichier_Index_TOF(char nom_fichier[])
{

    Sauvegarde_Table_Index_TOF(nom_fichier); // en cas ou le fichier Index n'a pas encore ete cree

    fichier_TOF f;
    Ouvrir_TOF(&f, nom_fichier, 'A');
    int i = 1,        // parcours bloc par bloc
        j = 0,        // parcours de position dans le bloc
        counter = 0;  // numero de l'enregistrement dans le bloc
    Tampon_INDEX Buf; // contenu d'un bloc dans un buffer Index TOF

    /************************************************|
    |  Identifiant  |    numBloc  |   Deplacement    |
    |   (5 bytes)   |  (integer)  |    (integer)     |
    |************************************************/

    affichage_entete_TOF(nom_fichier);

    while (i <= Entete_TOF(&f, 1)) // tant qu'on n'est pas arrive a la fin du fichier
    {
        LireDir_Index_TOF(&f, i, &Buf);
        j = 0;
        printf("\n\n\n*************************************************\n");
        printf("*                                               *\n");
        printf("*            Le bloc numero : %i                 *\n", i);
        printf("*        rempli a %i / %i enregistrements         *\n", Buf.nombre_enreg, MAX_ENREG);
        printf("*                                               *\n");
        printf("*************************************************\n");

        while (j < Buf.nombre_enreg) // tant que on est pas arrive a la fin du bloc numero i
        {
            printf("\n\n.........................\n");
            printf(".                       .\n");
            printf(".  Materiel numero : %i  .\n", counter);
            printf(".                       .\n");
            printf(".........................\n");
            printf("identifiant: %s\n", Buf.tab_INDEX[j].Identifiant);
            printf("Numero du Bloc: %i\n", Buf.tab_INDEX[j].NumBloc);
            printf("Deplacement dans le Bloc : %i\n", Buf.tab_INDEX[j].Deplacement);
            counter++;
            j++;
        }
        i++;
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
        if (strcmp(Fonctionne, "f") == 0)
            printf("Fonctionne : OUI\n");
        else
            printf("Fonctionne : NON\n");
        printf("prix: %.6s DA\n", Prix);
        printf("taille de description: %.3s\n", Taille);
        printf("description: %s\n", Description);

        l = TAILLE_IDENTIFIANT + TAILLE_FONCTIONNEMENT + TAILLE_MATERIEL + TAILLE_PRIX + TAILLE_TAILLE + strlen(Description);
        char Enreg[l];
        concatenate(Enreg, Identifiant, Fonctionne, Materiel, Prix, Taille, Description);
        printf("l'element sera insere sous cette forme: %s\n", Enreg);
        Ecrire_chaine_TOVnC(F, Enreg, &i, &j, &buf);

        if (strcmp(Fonctionne, "f") == 0) // generer le champs fonctionnement (suppression) en alternative
            strcpy(Fonctionne, "n");      // afin d'equilibrer les fichiers LOVC et TOVC apres fragmentation
        else                              // si le materiel precedent fonctionnait alors le prochain ne fonctionnera pas
            strcpy(Fonctionne, "f");      // si le materiel precedent ne fonctionnait pas alors le prochain fonctionnera
    }
    EcrireDir_TOVnC(F, i, buf); // ecrire le dernier buffer meme si il n'etait pas plein
    Fermer_TOVnC(F);            // fermer le fichier
}

/*










*/

/****************************************************|
|                                                    |
|   Recherche dun materiel selon son identifiant     |
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
/***********************************************|
|                                               |
|    copier la chaine "Chaine" dans  Le Buf     |
|          a partir de la pos "pos"             |
|                                               |
|***********************************************/

void Insert_string_TOVnC(Tampon_TOVnC *Buf, int *pos, char chaine[]) // module à utiliser pour l'insertion
{
    int i = 0;
    while (i < strlen(chaine))
    {
        Buf->tableau[*pos] = chaine[i];
        *pos = *pos + 1;
        i++;
    }
}

/*













*/

/***********************************************|
|                                               |
|   collecter les informations necessaires      |
|      sur le nouveau materiel a inserer        |
|                                               |
|***********************************************/
void Demande_Information_Utilisateur(char *Fonctionnement, char *Materiel, char *Prix, char *Description)
{
    int answers, trouv, counter;

    /*__________________________________
    |    CHAMPS 02 : FONCTIONNEMENT     |
    |___________________________________*/
    printf("\n\n---------------- Collection des infos : 2-Etat fonctionnement -------------------\n");
    printf("|    1 - en marche\n");
    printf("|    2 - en panne\n");
    printf("|    votre choix: ");
    scanf("%i", &answers); // le numero du materiel
    while (answers < 1 || answers > 2)
    {
        printf("|    choix inexistant, veuillez entrer 1 ou 2: ");
        scanf("%i", &answers);
    }
    printf("---------------------------------------------------------------------------------\n\n");
    if (answers == 1)
        strcpy(Fonctionnement, "f");
    else
        strcpy(Fonctionnement, "n");

    /*_____________________________
    |    CHAMPS 03 : MATERIEL     |
    |____________________________*/
    printf("\n\n---------------- Collection des infos : 3-Type du materiel -------------------\n");
    printf("|    -> Le type materiel : \n\n");                // demander le type du materiel
    for (counter = 1; counter <= NB_TYPE_MATERIEL; counter++) // la liste des matreiel a proposer sur l'utilisateur
    {
        printf("|    %i - %s\n", counter, MATERIAL_LIST[counter - 1]);
    }
    printf("\n|    votre choix: ");
    scanf("%i", &answers); // le numero du materiel
    while (answers < 1 || answers > NB_TYPE_MATERIEL)
    {
        printf("|    numero inexistant, veuillez rentrer un autre entre [%i, %i]: ", 1, NB_TYPE_MATERIEL);
        scanf("%i", &answers);
    }
    printf("------------------------------------------------------------------------------\n\n");
    strcpy(Materiel, MATERIAL_LIST[answers - 1]); // remplir le champs materiel

    /*_____________________________
    |    CHAMPS 04 : PRIX         |
    |____________________________*/
    printf("\n\n---------------- Collection des infos : 4- Prix -------------------\n");
    printf("|    -> Le prix d'achat du materiel : "); // demander le prix
    scanf("%i", &answers);                            // demander le prix
    Generer_Chaine(Prix, TAILLE_PRIX, answers);
    printf("-------------------------------------------------------------------\n\n");
    /*_____________________________
    |   CHAMPS 05 : DESCRIPTION   |
    |____________________________*/
    printf("\n\n---------------- Collection des infos : 5- Description -------------------\n");
    printf("|    -> La Description : "); // demander la description
    scanf(" %[^\n]", Description);       // demander la description
    printf("---------------------------------------------------------------------------------------------\n\n");
}

/*













*/

/********************************************************|
|                                                        |
|   insertion d’un matériel dans le fichier de données   |
|                                                        |
|********************************************************/
void Insertion_TOVnC(char nom_fichier[]) // procédure pour inserer une chaine dans un fichier , la chaine represente le materiel
{

    int i, j,            // l'emplacement ou on va inserer le matereil(i:le bloc , j: la posistion)
        k, counter,      // des variables seront utilisés pour le decalage
        taille_materiel, // la taille de tout l'enregistrement a inserer
        taille_chaines,  // la taille des chaine qui vont deborder et seront inseres dans le prochain buffer
        trouv,           // boolean utilisé pour la recherche du materiel qu'on veut inserer
        answers,         // pour recevoir les donnees temporels de l'utilisateur sous forme d'entier
        stop = 0;        // booleen pour continuer l'operation de l'insertion

    char Identifiant[TAILLE_IDENTIFIANT + 1],        // numero d'identifiant(cle)
        Fonctionne[TAILLE_FONCTIONNEMENT + 1] = "f", // fonctionne = 'f', le materiel marche, fonctionne = 'n' sinon
        Materiel[TAILLE_MATERIEL],                   // le type du materiel
        Prix[TAILLE_PRIX + 1],                       // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],                   // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1],     // la description (caracteristiques) du materiel
        Destination[B],                              // la chaine complete a inserer
        Chaine_debordantes[B];                       /// les chaine qui seront exclues du buffer en cas de decalage et seront inserees dans les buffers qui suivent

    fichier_TOVnC f;
    Tbloc_TOVnC Buf, Buf_temp;

    // collecter les informations necessaires sur le nouveau mateirel a insere
    /*_____________________________
    |    CHAMPS 01 : IDENTIFIANT  |
    |____________________________*/
    printf("\n\n---------------- Collection des infos : 1-Identifiant -------------------\n");
    printf("|    -> L'identifiant : "); // demander l'identifiant
    scanf("%i", &answers);              // recevoir l'identifiant
    printf("-------------------------------------------------------------------------\n");
    Generer_Chaine(Identifiant, TAILLE_IDENTIFIANT, answers); // generer la chaine identifiant pour la rechercher dans la table d'index

    Recherche_TOVnC(nom_fichier, Identifiant, &trouv, &i, &j); // rechercher l'idenfiant

    if (trouv) // si l'identifiant existe deja dans la table d'index et donc dans le fichier index egalement
    {          // on insere pas le nouveau materiel

        printf("\n\n-------------------------------------------------------------------------\n");
        printf("| l'identifiant exste deja dans le fichier, aucune insertion n'a eu lieu |\n");
        printf("-------------------------------------------------------------------------\n\n");
    }

    else // si l'identifiant n'existe pas deja dans le fichier la chaine doit être inséré dans le bloc i à la position j
    {

        Demande_Information_Utilisateur(Fonctionne, Materiel, Prix, Description); // collecter les informations de l'utilisateur sur le materiel a inserer

        Ouvrir_TOVnC(&f, nom_fichier, 'A'); // ouvrir le fichier ou inserer
        /***************************************************************************************************|
        | Identifiant | champs fonctionne | Type materiel |    Prix   |   taille   | Description (variable) |
        |  (5 bytes)  |   (1 bytes)       |  (12 bytes)   | (6 bytes) |  (3 bytes) |  (max sur 273 bytes)   |
        |***************************************************************************************************/

        Generer_Chaine(Taille, TAILLE_TAILLE, strlen(Description));                             // (calculer) generer le champ Taille de description
        concatenate(Destination, Identifiant, Fonctionne, Materiel, Prix, Taille, Description); // concatener tous les champs dans la chaine destination

        taille_materiel = TAILLE_IDENTIFIANT + TAILLE_FONCTIONNEMENT + TAILLE_MATERIEL - 1 + TAILLE_PRIX + TAILLE_TAILLE + strlen(Description); // la taille de l'enreg

        while (!stop && i <= Entete_TOVnC(&f, 1)) // tant que toutes les chaine n'ont pas etees inserees et on n'est pas arrive a la fin du fichier
        {
            LireDir_TOVnC(&f, i, &Buf); // on lit le bloc ou la chaine doit etre inserer

            /************************************************************************************************************************|
            |                                                                                                                        |
            |     si l'espace libre dans le Buf peut contenir le materiel, on décale si nécessaire puis on insere le materiel        |
            |                                                                                                                        |
            |************************************************************************************************************************/
            if ((B - Buf.nb) >= taille_materiel)
            { // l'éspace libre est plus grand ou egale à la taille du materiel qu'on veut l'inserer
                k = Buf.nb + taille_materiel - 1;
                counter = 0;
                while (counter < Buf.nb - j)
                {
                    Buf.tableau[k] = Buf.tableau[k - taille_materiel]; // on fait le decalage
                    k--;                                               // des materiaux
                    counter++;                                         // qui viennent juste apres le materiel qu'on va inserer
                }
                Insert_string_TOVnC(&Buf, &j, Destination); // insertion du materiel
                Buf.nb += taille_materiel;                  // mettre a jour le nombre de caracteres dans le bloc i
                EcrireDir_TOVnC(&f, i, Buf);                // ecrire le buffer
                stop = 1;                                   // arreter le process de l'insertion
            }

            /************************************************************************************************************************|
            |                                                                                                                        |
            |             si l'espace libre ne peut pas contenir le materiel [(taille du materiel) > (B-buf.nb)]                     |
            |                                                                                                                        |
            |************************************************************************************************************************/
            else
            {

                /************************************************************************************************************************|
                |                                                                                                                        |
                |     si la taille du materiel + position ou il faut l'inserer(j) > B , on prend le materiel qu'on veut l'inserer        |
                |                et les materiels qui viennent apres comme une nouvelle chaine à inserer                                 |
                |                                                                                                                        |
                |************************************************************************************************************************/

                if ((j + taille_materiel) > B)
                {
                    taille_chaines = (Buf.nb - j);                                       // la taille des enregistrements qui viennent juste apres le meteril qu'on veut l'inserer
                    Buf.nb = j;                                                          // mise à jour de la position libre dans un bloc (buf.nb)
                    extraire_chaine_TOVnC(Chaine_debordantes, &j, taille_chaines, &Buf); // on fait sortir ces derniers enregistrements du bloc pour les inserer avec le materiel
                                                                                         // qu'on veut l'inserer
                    EcrireDir_TOVnC(&f, i, Buf);                                         // ecrire le Buffer i dans la MS
                    i = i + 1;                                                           //  l'insertion se fera à la prochaine itération du TQ , dans le prochain bloc
                    j = 0;                                                               // à la premiere position

                    /***************************************************************************************************|
                    |                                                                                                   |
                    |     si la taille de la chaine a inserer + les chaine debordante depasse a taille max d'un bloc    |
                    |                               lors ils doivent  etre repartis en 2 blocs distincts                |
                    |                                                                                                   |
                    |***************************************************************************************************/
                    if (taille_materiel + taille_chaines > B)
                    {
                        LireDir_TOVnC(&f, i, &Buf_temp);                    // lire le prochain bloc dans un buf temporaire pour faire le decalage
                        Ecrire_chaine_TOVnC(&f, Destination, &i, &j, &Buf); // ecrire la chaine destination dans le buf (elle sera toute seule la bas)
                        EcrireDir_TOVnC(&f, i, Buf);                        // ecrire le buf ou la chaine (nouveau materiel) inseree existe (toute seule)
                        while (i <= Entete_TOVnC(&f, 1))
                        {
                            printf("en cours ....");
                        }
                    }

                    /***************************************************************************************************|
                    |                                                                                                   |
                    |         sinon si la somme de leur taille peut etre contenue dans un seul bloc, donc on peut       |
                    |        les fusionner et les declarer comme la prochaine chaine qu'on desir inserer dans i+1       |
                    |                                                                                                   |
                    |***************************************************************************************************/
                    else
                    {
                        strcat(Destination, Chaine_debordantes); // nouveau materiel à inserer (chaine) dans le prochain bloc = materiel qu'on veut inserer (chaine)+ les materierls
                                                                 // qui viennent  apres ce dernier(Chaine_debordantes)
                    }
                }

                /************************************************************************************************************************|
                |                                                                                                                        |
                |                       si la taille du materiel + position ou il faut l'inserer(j) =< B                                 |
                |               on prend les chaines qui viennent apres comme la nouvelle chaine a inserer apres                         |
                |                                                                                                                        |
                |************************************************************************************************************************/
                else
                {
                    taille_chaines = (Buf.nb - j); // la taille des chaines qui vont deborder (les chaine qui viennent aprés la pos ou la nouvel chaine doit etre inseree)
                    extraire_chaine_TOVnC(Chaine_debordantes, &j, taille_chaines, &Buf);
                    j -= taille_chaines;                        // faire retourner le j en arriere (a cause de son avancement dans extraire_chaine_TOVnC)
                    Insert_string_TOVnC(&Buf, &j, Destination); // on insere le materiel
                    Buf.nb = j;                                 // mise à jour la position libre (buf.nb)
                    EcrireDir_TOVnC(&f, i, Buf);                // ecrire le buffer dans la MS
                    strcpy(Destination, Chaine_debordantes);    // le nouveau materiel qui va etre inserer dans le prochain bloc (chaine reçoit Chaine_debordantes)
                    i++;                                        // aller au bloc prochain
                    j = 0;                                      // se mettre au debut du bloc prochain

                    /*_____________________________________________________________________________________________________
                            REMARQUE: on a voulu laiser de vide dans le bloc pour faciliter les prochaines insertions
                                                        ( évitant les décalages  )
                     ______________________________________________________________________________________________________*/
                }
            }
        }

        /************************************************************************************************************************|
        |                                                                                                                        |
        |               si on est arrive a la fin du fichier, on rajoute un nouveau bloc contenant la chaine                     |
        |                                                                                                                        |
        |************************************************************************************************************************/

        if (i > Entete_TOVnC(&f, 1) && !stop)
        {
            i = Alloc_bloc_TOVnC(&f);                // allouer un nouveau bloc
            strcpy(Buf.tableau, Chaine_debordantes); // insertion chaine
            Buf.nb = taille_chaines;                 // mise à jour de la position libre dans un bloc (buf.nb)
            EcrireDir_TOVnC(&f, i, Buf);             // ecrire le buffer
        }

        Aff_Entete_TOVnC(&f, 2, Entete_TOVnC(&f, 2) + taille_materiel); // Entete_TOVnC(f,2) : nb d'insertion  , on incrémente le compteur d'insertions
        Fermer_TOVnC(&f);                                               // fermer le fichier finally
    }
}

/*










*/

/**********************************************|
|                                              |
|      Supprimer un materiel logiquement       |
|            selon son identifiant             |
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
    {
        printf("\n\n----------------------------------------------------------------\n");
        printf("|   le materiel n'existe pas, aucune suppression n'a eu lieu    |\n"); // alors informer mon audience qu'aucune suppression n'a eu lieu
        printf("----------------------------------------------------------------\n\n");
    }
}

/*










*/

/**********************************************|
|                                              |
|       reorganiser un fichier TOVnC selon     |
|          le champs de fonctionnement         |
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
|                                                |
|************************************************/
void Inserer_Enreg_TOF(fichier_TOF *f, Tenreg_TOF Enregistrement_TOF, int *i, int *j, Tampon_TOF *Buf)
{
    if (*j >= MAX_ENREG)
    {
        Buf->nombre_enreg = MAX_ENREG;
        EcrireDir_TOF(f, *i, *Buf); // ecrire le buf i
        *i = Alloc_bloc_TOF(f);     // nouveau bloc + mise a jour de l'entete
        *j = 0;
    }
    strcpy(Buf->tab[*j].Identifiant, Enregistrement_TOF.Identifiant); // mise a jour du Buf: champs identifiant
    Buf->tab[*j].Prix = Enregistrement_TOF.Prix;                      // mise a jour du Buf: champs Prix
    Buf->tab[*j].supprimer = Enregistrement_TOF.supprimer;            // mise a jour du Buf: champs Supprimer

    *j = *j + 1;                                // aller a la prochaine pos libre dans le Buf
    Buf->nombre_enreg = *j;                     // mise a jour du Buf: nombre d'enreg dans le buf
    Aff_Entete_TOF(f, 2, Entete_TOF(f, 2) + 1); // mise a jour de l'entete: nombre d'enregistrement inseres++
}

/*










*/

/************************************************|
|                                                |
|  Generer fichiers TOF a partir du fichier TOVC |
|     "nom_fichier" selon le champ materiel      |
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

        strcpy(Enregistrement_TOF.Identifiant, Identifiant); // mise a jour de l'enreg a inserer: champ identifiant
        Enregistrement_TOF.Prix = atoi(Prix);                // mise a jour de l'enreg a inserer: champ Prix
        Enregistrement_TOF.supprimer = 0;                    // mise a jour de l'enreg a inserer: champ Supprimer

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
        EcrireDir_TOF(&(Files[k].f), Files[k].i, (Files[k].Buf));
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
|                                                  |
|**************************************************/
void Choix_affichage_fichier_materiel()
{
    int counter;
    char Materiel[TAILLE_MATERIEL];
    char nom_fichier[MAX_NOM_FICHIER];

    printf("    -> Le fichier a afficher:  \n\n");            // demander le type du materiel
    for (counter = 1; counter <= NB_TYPE_MATERIEL; counter++) // la liste des matreiel a proposer sur l'utilisateur
    {
        printf("     %i - %s\n", counter, MATERIAL_LIST[counter - 1]);
    }
    printf("\n    votre choix: ");
    scanf("%i", &counter); // le numero du materiel
    while (counter < 1 || counter > NB_TYPE_MATERIEL)
    {
        printf("    numero inexistant, veuillez entrer un autre entre [%i, %i]: ", 1, NB_TYPE_MATERIEL);
        scanf("%i", &counter);
    }
    strcpy(Materiel, MATERIAL_LIST[counter - 1]); // remplir le champs materiel

    sprintf(nom_fichier, "Materiel_en_marche_%s_TOF.bin\0", MATERIAL_LIST[counter - 1]); // generer le nom du fichier selon le nom du materiel
    printf("    Nom fichier: %s", nom_fichier);

    afficher_fichier_TOF(nom_fichier);
}
/*










*/
/**************************************************|
|                                                  |
|     Requette a intervalle par rapport au prix    |
|    [Prix_Min , Prix_Min] dans un fichier LOVC    |
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

/************************************************************|
|                                                            |
|    Inserer un nouvel element dans TOVnC a la fin pour      |
|           qu'ils deviennent non ordonne TnOVnC             |
|                                                            |
|************************************************************/
void Insertion_TnOVnC(char nom_fichier[])
{
    int i, j,            // l'emplacement ou on va inserer le matereil(i:le bloc , j: la posistion)
        k,               // l'emplacement dans la table index
        answers,         // pour recevoir les donnees temporels de l'utilisateur sous forme d'entier
        taille_materiel, // la taille de tout l'enregistrement a inserer
        trouv;           // boolean utilisé pour la recherche du materiel qu'on veut inserer

    char Identifiant[TAILLE_IDENTIFIANT + 1],        // numero d'identifiant(cle)
        Fonctionne[TAILLE_FONCTIONNEMENT + 1] = "f", // fonctionne = 'f', le materiel marche, fonctionne = 'n' sinon
        Materiel[TAILLE_MATERIEL],                   // le type du materiel
        Prix[TAILLE_PRIX + 1],                       // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],                   // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1],     // la description (caracteristiques) du materiel
        Destination[B + 1];                          // la chaine complete a inserer

    Tenreg_INDEX enregistrement_index; // l'enregistrement qui sera insere dans la tabel index <identifiant , i , j>

    fichier_TOVnC f;
    Tbloc_TOVnC Buf;

    // collecter les informations necessaires sur le nouveau mateirel a insere
    /*_____________________________
    |    CHAMPS 01 : IDENTIFIANT  |
    |____________________________*/
    printf("\n\n---------------- Collection des infos : 1-Identifiant -------------------\n");
    printf("|    -> L'identifiant : "); // demander l'identifiant
    scanf("%i", &answers);              // recevoir l'identifiant
    printf("-------------------------------------------------------------------------\n");
    Generer_Chaine(Identifiant, TAILLE_IDENTIFIANT, answers); // generer la chaine identifiant pour la rechercher dans la table d'index

    Recherche_Dichotomique_Table_Index_TOF(Identifiant, &trouv, &k); // rechercher de l'identifiant dans la table d'index

    if (trouv) // si l'identifiant existe deja dans la table d'index et donc dans le fichier index egalement
    {          // on insere pas le nouvel element

        printf("\n\n--------------------------------------------------------------------------\n");
        printf("| l'identifiant existe deja dans le fichier, aucune insertion n'a eu lieu |\n");
        printf("--------------------------------------------------------------------------\n\n");
    }

    else // si l'identifiant n'existe pas deja dans la table d'index donc aussi dans le fichier
    {    // on insere la chaine dans la table d'index et a la fin du fichier TOVnC egalement

        Demande_Information_Utilisateur(Fonctionne, Materiel, Prix, Description);

        Ouvrir_TOVnC(&f, nom_fichier, 'A'); // ouvrir le fichier ou inserer
        /***************************************************************************************************|
        | Identifiant | champs fonctionne | Type materiel |    Prix   |   taille   | Description (variable) |
        |  (5 bytes)  |     (1 bytes)     |  (12 bytes)   | (6 bytes) |  (3 bytes) |  (max sur 273 bytes)   |
        |***************************************************************************************************/

        Generer_Chaine(Taille, TAILLE_TAILLE, strlen(Description));                             // (calculer) generer le champ Taille de description
        concatenate(Destination, Identifiant, Fonctionne, Materiel, Prix, Taille, Description); // concatener tous les champs dans la chaine destination

        taille_materiel = TAILLE_IDENTIFIANT + TAILLE_FONCTIONNEMENT + TAILLE_MATERIEL - 1 + TAILLE_PRIX + TAILLE_TAILLE + strlen(Description); // la taille de l'enreg

        /*************************************************************************|
        |                                                                         |
        |      preparation de l'enregistrement a inserer dans la table d'index    |
        |                                                                         |                                                                                                                      |
        |*************************************************************************/
        strcpy(enregistrement_index.Identifiant, Identifiant);  // remplir le champs identifiant du nouvel enreg de l'index
        enregistrement_index.NumBloc = Entete_TOVnC(&f, 1);     // remplir le champs numero du bloc du nouve enreg de l'index
        enregistrement_index.Deplacement = Entete_TOVnC(&f, 3); // remplir le champs deplacement du nouvel enreg de l'index
        Insertion_Table_Index(enregistrement_index, k);         // inserer dans la table d'index

        printf("\n\n-------------------------------------------------------------------------------\n");
        printf("|   < l'identifiant , i=%i , j=%i>    ont ete inserees dans la table d'index  |\n", enregistrement_index.NumBloc, enregistrement_index.Deplacement);
        printf("-------------------------------------------------------------------------------\n\n");

        /********************************************************************************************|
        |                                                                                            |
        |           Entete_TOVnC(&f, 1) correspond au dernier bloc dans le fichier TOVnC  et         |
        |     Entete_TOVnC(&f, 3) a la 1ere position libe dans le dernier bloc du fichier TOVnC      |
        |                                                                                            |                                                                                                                      |
        |********************************************************************************************/
        i = Entete_TOVnC(&f, 1);                            // le dernier bloc dans le fichier TOVnC
        j = Entete_TOVnC(&f, 3);                            // la 1ere pos libre dans le dernier fichier TOVnC
        Ecrire_chaine_TOVnC(&f, Destination, &i, &j, &Buf); // inserer le nouvel element a la fin du fichier TOVnC
        EcrireDir_TOVnC(&f, i, Buf);                        // ecrire le dernier bloc dans le fichier
        Fermer_TOVnC(&f);                                   // fermer le fichier TOVnC

        printf("\n\n-------------------------------------------------------------------------------\n");
        printf("| l'identifiant et ses infos ont ete inserees a la fin du fichier TOVnC       |\n");
        printf("-------------------------------------------------------------------------------\n\n");

        /*________________________________________________________________________________________|
        |                                                                                         |
        |   table index ne sera pas sauvegardee dans le fichier index meme apres une insertion    |
        |         elle sera sauvegardee apres la fin de toutes les executions dans main.c         |
        |         donc pas d'appel a la fonction :Sauvegarde_Table_Index_TOF() not yet :)         |                                                           |                                                                                                                      |
        |_________________________________________________________________________________________*/
    }
}
/*










*/
/**************************************************|
|                                                  |
|      LES FONCTIONS ET PROCEDURE DES INDEX        |
|             TABLE ET FICHIER INDEX               |
|                                                  |
|**************************************************/
/**************************************************|
|                                                  |
|      Creer la table et le fichier d'Index a      |
|  partir d'un fichier TOVnC "nom_fichier_TOVnC"   |
|                                                  |
|**************************************************/
void Creer_Index(char nom_fichier_TOVnC[])
{
    fichier_TOVnC F;                             // le fichier TOVnC
    char Identifiant[TAILLE_IDENTIFIANT + 1],    // la plus petite cle dans un bloc (plus petit identifiant)   // la cle courrant dont on s'est arrete dans le parcours
        Materiel[TAILLE_MATERIEL],               // le type du materiel
        Fonctionne[TAILLE_FONCTIONNEMENT + 1],   // fonctionne = 'f', le materiel marche, fonctionne = 'n' sinon
        Prix[TAILLE_PRIX + 1],                   // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],               // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1]; // la description (caracteristiques) du materiel

    Tenreg_INDEX indexEnreg;
    Tampon_TOVnC Buf;

    int i = 1, // i:  parcours bloc par bloc fichier TOVnC
        j = 0, // j: parcours de position dans le bloc de fichier TOVnC
        k = 0, // pour le parcour de la table d'index
        j1;    // pour la sauvegarde de la position avant l'extraction

    Ouvrir_TOVnC(&F, nom_fichier_TOVnC, 'A');

    while (i <= Entete_TOVnC(&F, 1))
    {
        LireDir_TOVnC(&F, i, &Buf); // lire le fichier TOVnC
        j = 0;

        while (j < Buf.nb)
        {
            j1 = j; // sauvgarde position

            /***************************************************************************************************|
            | Identifiant | champs fonctionne | Type materiel |    Prix   |   taille   | Description (variable) |
            |  (5 bytes)  |   (1 bytes)       |  (12 bytes)   | (6 bytes) |  (3 bytes) |  (max sur 273 bytes)   |
            |***************************************************************************************************/
            extraire_chaine_TOVnC(Identifiant, &j, TAILLE_IDENTIFIANT, &Buf);
            extraire_chaine_TOVnC(Fonctionne, &j, TAILLE_FONCTIONNEMENT, &Buf);
            extraire_chaine_TOVnC(Materiel, &j, TAILLE_MATERIEL - 1, &Buf);
            extraire_chaine_TOVnC(Prix, &j, TAILLE_PRIX, &Buf);
            extraire_chaine_TOVnC(Taille, &j, TAILLE_TAILLE, &Buf);
            extraire_chaine_TOVnC(Description, &j, atoi(Taille), &Buf);

            if (strcmp(Fonctionne, "f") == 0)
            {
                /*____________________________________
                |                                    |
                | remplissage d'enregistrement index |
                |____________________________________*/
                strcpy(indexEnreg.Identifiant, Identifiant); // remplir le champs: Identifiant (cle unique) dans l'enregistrement Index
                indexEnreg.NumBloc = i;                      // remplir le champs: Numero du bloc dans l'enregistrement Index
                indexEnreg.Deplacement = j1;                 // remplir le champs: Deplacement dans bloc dans l'enregistrement Index

                /*____________________________________
                |                                    |
                |   remplissage de la table d'index  |
                |____________________________________*/

                Index.table_Index[k] = indexEnreg; // affecter l'enregistrement prepare a la pos,k dan sla table d'index
                k++;                               // incrementer la pos libre dans k pour la prochaine insertion dans la table
                Index.nombre_enreg_inseres = k;    // mettre a jour le nombre d'enreg inseres dans la table d'index
            }
        }

        i++; // passer au prochain bloc
    }

    /*____________________________________
    |                                    |
    |       creation fichier Index       |
    |___________________________________*/
    /*_______________________________________________________________________________________________________________|
    |                                                                                                                |
    |   table index ne sera pas encore sauvegardee donc la creation du fichier index ne sera pas encore effectuee    |
    |                 elle sera sauvegardee apres la fin de toutes les executions dans main.c                        |
    |        Pourquoi? : pour diminuer la complexite , on sauvgardera lorsqu'on quitte le progrmme :)                |
    |         donc pas d'appel a la fonction :Sauvegarde_Table_Index_TOF() not the right time yet :)                 |                                                           |                                                                                                                      |
    |________________________________________________________________________________________________________________*/
}

/*










*/
/**************************************************|
|                                                  |
|      Creer la table et le fichier d'Index a      |
|      partir d'un fichier TOVnC "nom_fichier"     |
|                                                  |
|**************************************************/
void Afficher_Table_Index()
{
    int k; // pour le parcours de la table d'index
    printf("Nombre d'enregs dans table Index: %i", Index.nombre_enreg_inseres);
    for (k = 0; k < Index.nombre_enreg_inseres; k++)
    {
        printf("\n________________________________________________________________________");
        printf("\n|                                                                       ");
        printf("\n|   L'identifiant: %s", Index.table_Index[k].Identifiant);
        printf("\n|   Numero du Bloc: %i", Index.table_Index[k].NumBloc);
        printf("\n|   Deplacement dans le Bloc: %i", Index.table_Index[k].Deplacement);
    }
    printf("\n|________________________________________________________________________");
}
/*














*/

/************************************************************|
|                                                            |
|    Recherche dichotomique d'un identifiant dans une table  |
|        d'index, retourner trouv son enpmacement k          |
|                                                            |
|************************************************************/
void Recherche_Dichotomique_Table_Index_TOF(char Cle[], int *trouv, int *k)
{
    fichier_TOF fichier_Index;
    fichier_TOVnC f;

    /*_________________________________________________
    |                                                 |
    |  si la table d'index n'a pas encore ete generee |
    |_________________________________________________*/
    if (Index.nombre_enreg_inseres == 0)
    {
        Ouvrir_TOF(&fichier_Index, FICHIER_INDEX, 'A');
        /*______________________________________________________________________________________________
        |                                                                                              |
        |   si fichier Index a deja ete genere ,alors charger la table d'index a partir de celui ci    |
        |______________________________________________________________________________________________*/
        if (fichier_Index.fichier != NULL)
        {
            Chargement_Table_Index_TOF(FICHIER_INDEX); // charger la table d'index a partir du fichier d'index
        }

        /*______________________________________________________________________________________________
        |                                                                                              |
        |  s'il n'a pas encore ete genere, on cree la table d'index a partir du fichier original TOVnC |
        |______________________________________________________________________________________________*/
        else
        {
            Creer_Index(FICHIER_ORIGINAL); // creer la table d'index a partir du fichier original TOVnC
        }
    }

    /*____________________________________________________________________________________________
    |                                                                                            |
    |   Maintenant qu'on a assure la presence de la table d'index, commençons le travail :)      |
    |____________________________________________________________________________________________*/

    *trouv = 0;                               // booleen pour indiquer si on a trouve l'identifiant ou pas
    int inf = 0,                              // l'inf dans la table d'index pendant la recherche dichotomique
        sup = Index.nombre_enreg_inseres - 1; // le sup dans la table d'index pendant la recherche dichotomique

    while (!(*trouv) && inf <= sup)
    {

        *k = (inf + sup) / 2;                                    // diviser la recherche sur 2 intervalles
        if (strcmp(Index.table_Index[*k].Identifiant, Cle) == 0) // si la cle recherche = la cle courante de la pos k
        {                                                        // donc la cle existe deja dans la table d'index
            *trouv = 1;                                          // dans l'emplacement *k
        }                                                        // donc dans le fichier TOVnC egalement (i,j) peuvent etre extraits de l'enreg k dans la tabel d'index

        else // si la cle recherche est differente de la cle courrante
        {    // decouper l'espace de recherche again

            if (strcmp(Cle, Index.table_Index[*k].Identifiant) > 0) // si la cle recherche > la cle courante
            {                                                       // regler l'inf
                inf = *k + 1;
            }
            else // si la cle recherche < la cle courante
            {    // regler le sup
                sup = *k - 1;
            }
        }
    }
    if (inf > sup)
    {
        *k = inf;
    }
}
/*














*/

/************************************************************|
|                                                            |
|     Insertion d'un enregistrement_index dans la table      |
|                  d'index a la position k                   |
|                                                            |
|************************************************************/
void Insertion_Table_Index(Tenreg_INDEX enregistrement_index, int k)
{
    int temp = Index.nombre_enreg_inseres; // variable temp pour faire le decalage avec

    if (temp < MAX_ENREG_INDEX) // si la table d'index nest pas encore pleine
    {

        while (temp > k)
        {
            Index.table_Index[temp] = Index.table_Index[temp - 1]; // decaler tous les elements entre [k,dernier enreg] avec un oas
            temp--;                                                // revenir a l'arriere
        }

        Index.table_Index[k] = enregistrement_index; // finalement inserer le nouvel enreg dans sa place prebvenue k

        Index.nombre_enreg_inseres++; // mettre  a jour le nombre d'enregistrements dans la table d'index
    }

    else
    {
        printf("\n\n-----------------------------------------------------------------------------------\n");
        printf("|  la table d'index est pleine, aucune insertion dans la table d'index n'a eu lieu |\n");
        printf("-----------------------------------------------------------------------------------\n\n");
    }
}

/*














*/

/************************************************************|
|                                                            |
|   Chargement de la table index du fichier "nom_fichier"    |
|        de tupe TOF vers la MC dans la variable Index       |
|                                                            |
|************************************************************/
void Chargement_Table_Index_TOF(char nom_fichier_index[])
{
    fichier_TOF fichier_Index;
    Ouvrir_TOF(&fichier_Index, nom_fichier_index, 'A'); // ouvrir le fichier index d'ou charger
    int i = 1,                                          // pour le parcours du fichier Index par bloc
        j = 0,                                          // pour le parcours inter-bloc dans le fichier Index
        ind = 0;                                        // pour le parcours et le remplissage de la table d'index
    Tampon_INDEX Buf;                                   // le buffer speciale pour les lecture de MS vers MC deu fichier Index

    while (i <= Entete_TOF(&fichier_Index, 1)) // tant qu'on est pas arrive a la fin du fichier
    {
        LireDir_Index_TOF(&fichier_Index, i, &Buf); // lire le bloc courant
        j = 0;                                      // placer le deplacement dans le bloc dans sa 1ere position
        while (j < Buf.nombre_enreg)                // tant que on est pas arrive a la fin du bloc
        {
            strcpy(Index.table_Index[ind].Identifiant, Buf.tab_INDEX[j].Identifiant); // mise a jour de l'identifiant dans la table d'index
            Index.table_Index[ind].NumBloc = i;                                       // mise a jour de la 1ere coordonne de l'adresse de l'identifiant (numero de bloc) dans la table d'index
            Index.table_Index[ind].Deplacement = j;                                   // mise a jour de la 2eme coordonne de l'adresse de l'identifiant (deplacement dans le bloc) dans la table d'index
            ind++;                                                                    // mise a jour du prochain placement vide dans la table d'index pour inserer
            j++;                                                                      // mise a jour du prochain enregistrement a lire dans le bloc i dans le fichier index
            Index.nombre_enreg_inseres = ind;                                         // mise a jour du nombre d'enregs inseres dans la table d'index
        }
        i++; // passer au prochain bloc
    }

    Fermer_TOF(&fichier_Index);
}
/*














*/

/************************************************************|
|                                                            |
|       Sauvegarde de la table d'index dans un fichier       |
|               "nom_fichier" de type TOF                    |
|                                                            |
|************************************************************/
void Sauvegarde_Table_Index_TOF(char nom_fichier_index[])
{

    fichier_TOF fichier_Index;
    Ouvrir_TOF(&fichier_Index, nom_fichier_index, 'N'); // ouvrir le fichier index d'ou charger
    int i = Alloc_bloc_TOF(&fichier_Index),             // pour le parcours du fichier Index par bloc
        j = 0,                                          // pour le parcours inter-bloc dans le fichier Index
        ind = 0;                                        // pour le parcours et le remplissage de la table d'index
    Tampon_INDEX Buf;                                   // le buffer speciale pour les lecture de MS vers MC deu fichier Index

    while (ind < Index.nombre_enreg_inseres) // tant qu'on a pas parcourru tous les enregs de la table d'index
    {
        if (j >= MAX_ENREG)
        {
            Buf.nombre_enreg = MAX_ENREG;
            EcrireDir_Index_TOF(&fichier_Index, i, &Buf); // ecrire le buf i                                                                                      // incrementer le i
            i = Alloc_bloc_TOF(&fichier_Index);           // nouveau bloc + mise a jour de l'entete
            j = 0;                                        // remettre la pos dans le bloc au debut
        }
        strcpy(Buf.tab_INDEX[j].Identifiant, Index.table_Index[ind].Identifiant); // mise a jour du Buf: champs identifiant
        Buf.tab_INDEX[j].NumBloc = Index.table_Index[ind].NumBloc;                // mise a jour du Buf: champs Prix
        Buf.tab_INDEX[j].Deplacement = Index.table_Index[ind].Deplacement;        // mise a jour du Buf: champs Supprimer

        j++;                  // aller a la prochaine pos libre dans le Buf
        Buf.nombre_enreg = j; // mise a jour du Buf: nombre d'enreg dans le buf
        ind++;                // aller a la prochaine position dans la table d'index
    }
    EcrireDir_Index_TOF(&fichier_Index, i, &Buf);                  // ecrire le dernier bloc dans la MS
    Aff_Entete_TOF(&fichier_Index, 2, Index.nombre_enreg_inseres); // mise a jour de l'entete: nombre d'enregistrement inseres
    Fermer_TOF(&fichier_Index);                                    // fermer le fichier afin de sauvegarder l'entete
}
/*














*/

int main(void)
{
    Index.nombre_enreg_inseres = 0;

    int int_answers, quit = 0;
    char char_answers[B];
    int trouv, i, j, k;

    while (!quit)
    {

        printf("\n\n    *************************************************\n");
        printf("    *                                               *\n");
        printf("    *   Bienvenu sur notre programme fascinant!     *\n");
        printf("    *                                               *\n");
        printf("    *************************************************\n\n");

        printf("    veuillez consulter nos diverses fonctions et en choisir une:\n\n");
        printf("    01-  Chargement initial aleatoire d'un fichier TOVnC\n");
        printf("    02-  Affichage du fichier materiel original TOVnC\n");
        printf("    03-  Recherche dans le fichier TOVnC selon l'identifiant\n");
        printf("    04-  Insertion dans le fichier TOVnC\n");
        printf("    05-  Suppression dans le fichier TOVnC selon l'identifiant\n");
        printf("    06-  Fragmentation du fcihier TOVnC en TOVC et LOVC selon le fonctionnnement du materiel\n");
        printf("    07-  Affichage du fichier materiel en marche TOVC\n");
        printf("    08-  Affichage du fichier materiel en panne LOVC\n");
        printf("    09-  Generation fichiers TOF a partir du fichier TOVnC selon le champs materiel\n");
        printf("    10-  Afficher fichier TOF resultant des generations\n");
        printf("    11-  Requette a interval selon le prix dans le fichier LOVC (Materiel en panne)\n");
        printf("    12-  Creation table Index et fichier index de TOVnC\n");
        printf("    13-  Afficher Table Index\n");
        printf("    14-  Afficher fichier Index strcuture sous forme TOF\n");
        printf("    15-  Insertion materiel a la fin du fichier TOVnC,dans la table index et fichier index\n");
        printf("    16-  Quitter le programme");

        // le choix des options
        printf("\n\n    saisissez le numero de votre option: ");
        scanf("%i", &int_answers);
        while (int_answers < 1 || int_answers > 16)
        {
            printf("    pas d'option correspondante a un tel numero, saisissez un autre: ");
            scanf("%i", &int_answers);
        }

        switch (int_answers)
        {
        case 1:

            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *       Chargement initial aleatoire d'un       *\n");
            printf("    *           fichier TOVnC de materiel           *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");

            printf("    Nombre de materiel initial que vous voulez generer: ");
            scanf("%i", &int_answers);
            printf("    Nombre de materiel: %i", int_answers);
            Chargement_initial_TOVnC(FICHIER_ORIGINAL, int_answers);
            break;

        case 2:

            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *         Affichage du fichier materiel         *\n");
            printf("    *                original TOVnC                 *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            afficher_fichier_TOVnC(FICHIER_ORIGINAL);
            break;

        case 3:

            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *    Recherche d'un materiel dans le fichier    *\n");
            printf("    *     original TOVnC selon son identifiant      *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            printf("    -> L'identifiant a rechercher: ");                   // demander l'identifiant
            scanf("%i", &int_answers);                                       // recevoir l'identifiant
            Generer_Chaine(char_answers, TAILLE_IDENTIFIANT, int_answers);   // le generer sous forme d'une chaine sur la taille 5 chars
            printf("    -> L'identifant a rechercher : %s", char_answers);   // confirmer l'identifiant a supprimer en l'affichant
            Recherche_TOVnC(FICHIER_ORIGINAL, char_answers, &trouv, &i, &j); // le rechercher
            break;

        case 4:

            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *    Insertion d'un materiel dans le fichier    *\n");
            printf("    *     original TOVnC selon son identifiant      *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            Insertion_TOVnC(FICHIER_ORIGINAL);
            break;

        case 5:

            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *   Suppression d'un materiel dans le fichier   *\n");
            printf("    *     original TOVnC selon son identifiant     *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            printf("    -> L'identifiant a supprimer: ");                  // demander l'identifiant
            scanf("%i", &int_answers);                                     // recevoir l'identifiant
            Generer_Chaine(char_answers, TAILLE_IDENTIFIANT, int_answers); // le generer sous forme d'une chaine sur la taille 5 chars
            printf("    -> L'identifiant a supprimer: %s", char_answers);  // confirmer l'identifiant a supprimer en l'affichant
            Suppression_TOVnC(FICHIER_ORIGINAL, char_answers);             // le supprimer
            break;

        case 6:

            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *   Fragmentation du fcihier original TOVnC en  *\n");
            printf("    *            fichier TOVC et LOVC               *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            Reorganisation_TOVnC(FICHIER_ORIGINAL, FICHIER_MATERIEL_FONCTIONNE, FICHIER_MATERIEL_NON_FONCTIONNE);
            break;

        case 7:

            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *        Affichage du fichier de materiel       *\n");
            printf("    *                en marche TOVC                 *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            afficher_fichier_TOVC(FICHIER_MATERIEL_FONCTIONNE);
            break;

        case 8:

            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *       Affichage du fichier de materiel        *\n");
            printf("    *                en panne LOVC                  *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            afficher_fichier_LOVC(FICHIER_MATERIEL_NON_FONCTIONNE);
            break;

        case 9:

            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *      Generation fichiers TOF a partir du      *\n");
            printf("    *     fichier TOVnC selon le champs materiel    *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            Generation_fichiers_Materiel(FICHIER_MATERIEL_FONCTIONNE);
            break;

        case 10:

            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *     Afficher fichier TOF resultant des        *\n");
            printf("    *                 generations                   *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            Choix_affichage_fichier_materiel();
            break;

        case 11:

            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *       Requette a interval selon le prix       *\n");
            printf("    *              dans le fichier LOVC             *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            printf("    -> Prix Min: "); // demander l'identifiant
            scanf("%i", &i);
            printf("    -> Prix Max: "); // demander l'identifiant
            scanf("%i", &j);
            if (i > j) // si prix min> prix max permuter les 2 prix
            {
                k = i;
                i = j;
                j = k;
            }
            Requette_intervalle_LOVC(FICHIER_MATERIEL_NON_FONCTIONNE, i, j, &k);
            break;

        case 12:

            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *        Creation table Index et fichier        *\n");
            printf("    *                index de TOVnC                 *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            Creer_Index(FICHIER_ORIGINAL);
            break;

        case 13:

            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *              Afficher Table Index             *\n");
            printf("    *                                               *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            Afficher_Table_Index(Index);
            break;

        case 14:

            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *       Afficher fichier Index strcuture        *\n");
            printf("    *                sous forme TOF                 *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            afficher_fichier_Index_TOF(FICHIER_INDEX);
            break;

        case 15:

            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *  Insertion materiel a la fin du fichier TOVnC *\n");
            printf("    *      dans la table index et fichier index     *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            Insertion_TnOVnC(FICHIER_ORIGINAL);
            break;

        case 16:
            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *             QUITTER LE PROGRAMME              *\n");
            printf("    * SAUVEGARDE TABLE INDEX DANS FICHIER INDEX ... *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");

            /*________________________________________________________________________________________________________________|
            |                                                                                                                 |
            |             Rappelez-vous du commentaire indique dans l'insertion TOVnC et dans la creation d'index?            |
            |      On a insiste a ne pas sauvegarder la table d'index dans le fichier index jusqu'a la fin du programme       |
            | car le fichier index n'est utile que pour la sauvegarde apres fin d'execution du programme pour y revenir apres |
            |     AND THATS THE RIGHT TIME TO DO SO, WE REACHED THE END OF OUR PROGRAM SAUVEGARDONS LA AVANT DE QUITTER !     |                                                           |                                                                                                                      |
            |________________________________________________________________________________________________________________*/

            Sauvegarde_Table_Index_TOF(FICHIER_INDEX);
            quit = 1;
            break;
        }
    }

    return 0;
}