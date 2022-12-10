#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lib.h"
#include <stdbool.h>

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
    Ouvrir_TOVnC(&f, nom_fichier, 'N');
    printf("\n\n\n*************************************************\n");
    printf("*                                               *\n");
    printf("*       caracteristiques du fichier             *\n");
    printf("*                                               *\n");
    printf("*************************************************\n\n\n");
    printf(" -> Nombre de Blocs : %d\n", Entete_TOVnC(&f, 1));
    printf(" -> Nombre caracteres inseres : %d\n", Entete_TOVnC(&f, 2));
    printf(" -> Nombre caracteres supprimes : %d\n", Entete_TOVnC(&f, 3));
}

/**************************************************|
|                                                  |
|    extraire une chaine de taille "taille" dans   |
|      la chaine destination a partir pos j        |
|                                                  |
|**************************************************/
void extraire_chaine_TOVnC(char destination[], int *j, int taille, Tampon_TOVnC *Buf)
{
    int k = 0;
    sprintf(destination, "%s", "");
    for (k = 0; k < taille; k++)             // boucle de taille iterations correspondant a la longueur de la chaine
    {                                        // indice de parcours de la chaine resultata et j celui du tableau
        destination[k] = Buf->tableau[(*j)]; // recuperation du caractere dans la position k de la chaine
        (*j)++;                              // deplacement d'une position dans le buffer
    }                                        // fin de boucle
    destination[k] = '\0';                   // fin de la chaine obtenue
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
        Supprime[TAILLE_SUPPRIMER + 1],          // supprimer='f' l'element n'a pas ete supprime supprimer='t' sinon
        Materiel[TAILLE_MATERIEL],               // le type du materiel
        Fonctionne[TAILLE_FONCTIONNEMENT + 1],   // fonctionne = "f", le materiel marche, fonctionne = "n" sinon
        Prix[TAILLE_PRIX + 1],                   // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],               // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1]; // la description (caracteristiques) du materiel

    /**************************************************************************************************************|
    | Identifiant | champs supprime | Type materiel | fonctionne |    Prix   |   taille   | Description (variable) |
    |  (5 bytes)  |   (1 bytes)     |  (12 bytes)   |  (1 bytes) | (6 bytes) |  (3 bytes) |  (max sur 272 bytes)   |
    |**************************************************************************************************************/
    while (i <= Entete_TOVnC(&f, 1))
    {
        printf("\n\n\n*************************************************\n");
        printf("*                                               *\n");
        printf("*            Le bloc numero : %i                 *\n", i);
        printf("*                                               *\n");
        printf("*************************************************\n");

        LireDir_TOVnC(&f, i, &Buf);
        while (j < Buf.nb)
        {
            extraire_chaine_TOVnC(Identifiant, &j, TAILLE_IDENTIFIANT, &Buf);
            extraire_chaine_TOVnC(Supprime, &j, TAILLE_SUPPRIMER, &Buf);
            extraire_chaine_TOVnC(Materiel, &j, TAILLE_MATERIEL - 1, &Buf);
            extraire_chaine_TOVnC(Fonctionne, &j, TAILLE_FONCTIONNEMENT, &Buf);
            extraire_chaine_TOVnC(Prix, &j, TAILLE_PRIX, &Buf);
            extraire_chaine_TOVnC(Taille, &j, TAILLE_TAILLE, &Buf);
            extraire_chaine_TOVnC(Description, &j, atoi(Taille), &Buf);

            if (strcmp(Supprime, "f") == 0)
            {
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
                printf("prix: %.6s\n", Prix);
                printf("taille de description: %.3s\n", Taille);
                printf("description: %s\n", Description);
                counter++;
            }
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
            fread(&(f->entete), sizeof(entete_TOF), 1, f->fichier);
    }
    else if (tolower(mode) == 'n')
    {
        f->fichier = fopen(nom_fichier, "wb+");
        Aff_Entete_TOF(f, 1, 0); // mettre le nombre de blocs à 0
        Aff_Entete_TOF(f, 2, 0); // mettre le nombre d'enregistrements inseres à 0
        Aff_Entete_TOF(f, 3, 0); // mettre le nombre d'enregistrements supprimes à 0
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
void Fermer_TOF(fichier_TOF f)
{
    fseek(f.fichier, 0, SEEK_SET);
    fwrite(&(f.entete), sizeof(entete_TOF), 1, f.fichier);
    fclose(f.fichier);
}

/********************************************|
|                                            |
|      Lire le i eme bloc dans buf TOF       |
|                                            |
|********************************************/
void LireDir_TOF(fichier_TOF f, int i, Tampon_TOF *buf, int *cpt_lect)
{
    rewind(f.fichier);
    fseek(f.fichier, sizeof(fichier_TOF) + (i) * sizeof(Tampon_TOF), SEEK_SET);
    fread(buf, sizeof(Tampon_TOF), 1, f.fichier);
    (*cpt_lect) = (*cpt_lect) + 1;
}

/*********************************************|
|                                             |
|      Ecrire buf dans le i eme bloc TOF      |
|                                             |
|*********************************************/
void EcrireDir_TOF(fichier_TOF f, int i, Tampon_TOF *buf, int *cpt_ecr)
{
    rewind(f.fichier);
    fseek(f.fichier, sizeof(entete_TOF) + (i) * sizeof(Tampon_TOF), SEEK_SET);
    fwrite(buf, sizeof(Tampon_TOF), 1, f.fichier);
    (*cpt_ecr) = (*cpt_ecr) + 1;
}

/********************************************|
|                                            |
|  Retoure la i ème valeur del'entete  TOF   |
|                                            |
|********************************************/
int Entete_TOF(fichier_TOF f, int i)
{
    if (i == 1) // nombre de blocs total
        return f.entete.blocs_total;
    else if (i == 2) // nombre d'enregistrements inseres
        return f.entete.enreg_inseres;
    else if (i == 3) // nombre d'enregistrements supprimes
        return f.entete.enreg_supprimes;
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
|                                            |
|********************************************/
int Alloc_bloc_TOF(fichier_TOF *f)
{
    int i = Entete_TOF(*f, 1);   // le nombre de bloc = le numero du dernier bloc -1
    Aff_Entete_TOF(f, 1, i + 1); // mettre a jour le nombre de bloc dans l'entete
    return Entete_TOF(*f, 1);    // le nombre anciens de bloc = numero du dernier bloc
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
void Ouvrir_LOVC(fichier_LOVC *f, const char nomFichier[], char mode)
{
    f = malloc(sizeof(fichier_LOVC)); // allocation  de la structure

    if (tolower(mode) == 'a') // mode ancien
    {
        f->fichier = fopen(nomFichier, "rb+");
        if (f->fichier == NULL)
            printf("error interrupted the program ... check file's name");
        else
            fread(&(f->entete), sizeof(Entete_LOVC), 1, f->fichier); // chargement de l'entete
    }

    else if (tolower(mode) == 'n')
    {
        f->fichier = fopen(nomFichier, "wb+");
        aff_entete_LOVC(f, 1, -1); // mise a NIL de l'adresse du premier bloc tant qu'il est vide
        aff_entete_LOVC(f, 2, -1); // metter l'adresse du dernier bloc a NIL tant que le fichier est vide
        aff_entete_LOVC(f, 3, 0);  // initialiser le nombre de caractères inseres a 0
        aff_entete_LOVC(f, 4, 0);  // initialiser le nombre de caractères supprimes a 0
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
    free(f);                                                  // libere l'espace memoire occupe par la structure
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
    LireDir_LOVC(fichier, entete_LOVC(fichier, 2), buf);      // lecture du bloc correspondant a la queue
    buf->suivant = entete_LOVC(fichier, 1) + 1;               // mise a jour dui suivant de la queue au bloc correspondant a la nouvelle queue
    ecrireDir_LOVC(fichier, entete_LOVC(fichier, 2), buf);    // ecriture du bloc de queue dans le fichier
    aff_entete_LOVC(fichier, 2, entete_LOVC(fichier, 1) + 1); // mise a jour du numero du bloc correspondant a la nouvelle queue dan sl'entete
    buf->suivant = -1;                                        // mise a jour du suivant a nill
    sprintf(buf->tab, "%s", "");                              // vider la chaine du buffer
    return entete_LOVC(fichier, 2);
}
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
void concatenate(char *destination, char *identifiant, char *supprime, char *materiel, char *fonctionne, char *prix, char *taille, char *description)
{
    /**************************************************************************************************************|
    | Identifiant | champs supprime | Type materiel | fonctionne |    Prix   |   taille   | Description (variable) |
    |  (5 bytes)  |   (1 bytes)     |  (12 bytes)   |  (1 bytes) | (6 bytes) |  (3 bytes) |  (max sur 272 bytes)   |
    |**************************************************************************************************************/
    sprintf(destination, "%s", "");                          // vider le tableau des caractères
    strncat(destination, identifiant, TAILLE_IDENTIFIANT);   // destination+=identifiant
    strncat(destination, supprime, TAILLE_SUPPRIMER);        // destination+=supprime
    strncat(destination, materiel, TAILLE_MATERIEL);         // destination+=materiel
    strncat(destination, fonctionne, TAILLE_FONCTIONNEMENT); // destination+=fonctionne
    strncat(destination, prix, TAILLE_PRIX);                 // destination+=prix
    strncat(destination, taille, TAILLE_TAILLE);             // destination+=taille
    strcat(destination, description);                        // destination+=description
}

/**********************************************|
|                                              |
| Ecrire la chaine d'identifiant cle a partir  |
|       du bloc i a la pos j dans Buf          |
|                                              |
|**********************************************/
void Ecrire_chaine_TOVnC(fichier_TOVnC *F, char chaine[], char cle[], int *i, int *j, Tampon_TOVnC *Buf)
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
    strcpy(Buf->cleMax, cle);                                    // mise a jour du bloc: la cle max
}

/**********************************************|
|                                              |
| Ecrire la chaine d'identifiant cle a partir  |
|       du bloc i a la pos j dans Buf          |
|                                              |
|**********************************************/
void Ecrire_Chaine_TOVC(fichier_TOVC *fichier, int *i, int *j, char chaine[], int *cpt, Tampon_TOVC *buf)
{
    int k;                               // variable pour parcourir la chaine caractere par caractere
    (*cpt) = 0;                          // nombre de bloc ajoutes
    for (k = 0; k < strlen(chaine); k++) // k pour le deplacement dans la chaine
    {                                    // parcours
        if ((*j) <= B - 1)               // si je suis toujours dans le bloc conserne
        {
            buf->tableau[*j] = chaine[k]; // affectation des caracteres de la chaine dans le buffer un a un
            (*j)++;                       // deplacement dans le buffer
        }
        else                                   // si la chaine a inserer depasse le buffer
        {                                      // nouveau bloc
            EcrireDir_TOVC(fichier, *i, *buf); // ecriture du precedent buffer dans le fichier
            alloc_bloc(fichier);               // alocation d'un nouveau bloc afin de recevoir le reste de la chaine
            buf->tableau[0] = chaine[k];       // ecrtiture du kiem caractere de la chaine dans la position 0
            (*j) = 1;                          // passage a la position 1
            (*i) = Alloc_bloc_TOVC(fichier);   // deplacement dans les bloc du ficher
            (*cpt)++;                          // incrementation du nombre de bloc alloues
        }
    }
    buf->tableau[*j] = '\0'; // fin de la chaine
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
        Supprime[TAILLE_SUPPRIMER + 1] = "f",        // supprimer='f' l'element n'a pas ete supprime supprimer='t' sinon
        Materiel[TAILLE_MATERIEL + 1],               // le type du materiel
        Fonctionne[TAILLE_FONCTIONNEMENT + 1] = "f", // fonctionne = 'f', le materiel marche, fonctionne = 'n' sinon
        Prix[TAILLE_PRIX + 1],                       // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],                   // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1];     // la description (caracteristiques) du materiel

    for (k = 0; k < n; k++)
    {                                                                            // l'identifiant= multiple de 10 pour garder l'ordre et possibilite d'insertion
        Generer_Chaine(Identifiant, TAILLE_IDENTIFIANT, 10 * k);                 // generer l'identifiant sous forme de chaine sur 5 positions
        strcpy(Materiel, MATERIAL_LIST[Random_Number(0, NB_TYPE_MATERIEL - 1)]); // tirer un materiel de la liste  des materiels selon index genere aleartoirement
        Generer_Chaine(Prix, TAILLE_PRIX, Random_Number(0, PRIX_MAX));           // generer le prix du materiel aleartoirement

        printf("\n\n.........................\n");
        printf(".                       .\n");
        printf(". element numero : %i    .\n", k);
        printf(".                       .\n");
        printf(".........................\n");
        printf("identifiant: %.5s\n", Identifiant);
        printf("materiel: %.11s\n", Materiel);
        printf("prix: %.6s\n", Prix);

        printf("Description de votre materiel: ");                  // demander la description du materiel de l'utilisateur
        scanf(" %[^\n]", Description);                              // Lire la description de l'utilisateur
        Generer_Chaine(Taille, TAILLE_TAILLE, strlen(Description)); // taille du champs de la description
        printf("taille de description: %.3s\n", Taille);
        printf("description: %s\n", Description);

        l = TAILLE_IDENTIFIANT + TAILLE_SUPPRIMER + TAILLE_MATERIEL + TAILLE_FONCTIONNEMENT + TAILLE_PRIX + TAILLE_TAILLE + strlen(Description);
        char Enreg[l];
        concatenate(Enreg, Identifiant, Supprime, Materiel, Fonctionne, Prix, Taille, Description);
        printf("l'element sera insere sous cette forme: %s\n", Enreg);
        Ecrire_chaine_TOVnC(F, Enreg, Identifiant, &i, &j, &buf);
        if (strcmp(Fonctionne, "f") == 0) // generer le champs fonctionnement en alternatif
            strcpy(Fonctionne, "n");      // si le precedent fonctionnait, le prochain ne fonctionne pas
        else                              // si le precedent ne fonctionnait pas , le prochain fonctionne
            strcpy(Fonctionne, "f");      // pour equilibrer les deux fichiers genere par la reorganisation
    }
    EcrireDir_TOVnC(F, i, buf); // ecrire le dernier buffer meme si il n'etait pas plein
    Fermer_TOVnC(F);            // fermer le fichier
}

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
        Cle_Max[TAILLE_IDENTIFIANT + 1],         // la plus grande cle dans un bloc (plus grand identifiant)
        Cle_Courrante[TAILLE_IDENTIFIANT + 1],   // la cle courrant dont on s'est arrete dans le parcours
        Identifiant[TAILLE_IDENTIFIANT + 1],     // numero d'identifiant(cle)
        Supprime[TAILLE_SUPPRIMER + 1],          // supprimer='f' l'element n'a pas ete supprime supprimer='t' sinon
        Materiel[TAILLE_MATERIEL],               // le type du materiel
        Fonctionne[TAILLE_FONCTIONNEMENT + 1],   // fonctionne = 'f', le materiel marche, fonctionne = 'n' sinon
        Prix[TAILLE_PRIX + 1],                   // le ptix du materiel
        Taille[TAILLE_TAILLE + 1],               // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION + 1]; // la description (caracteristiques) du materiel
    Tbloc_TOVnC Buf;                             // un buffer pour charger un bloc de MS vers MC
    *trouv = 0;                                  // initialiser trouv vers faux
    while (!(*trouv) && binf <= bsup && !stop)
    { // recherche dichotomique entre les bloc tq materiel non trouve et recherche possible(cle_courante<cle_recherche)

        *i = (binf + bsup) / 2;                                      // numero de bloc a parcourir
        *j = 0;                                                      // la premiere position dans le bloc
        LireDir_TOVnC(&f, *i, &Buf);                                 // lire le buffer
        temp_j = *j;                                                 // sauvegarder j avant de se deplacer
        extraire_chaine_TOVnC(Cle_Min, j, TAILLE_IDENTIFIANT, &Buf); // extraire la plus petite cle (premiere cle) de du bloc i
        strcpy(Cle_Courrante, Cle_Min);                              // mettre a jour la cle courrante
        strcpy(Cle_Max, Buf.cleMax);                                 // lire la cle max qui est dans le tableau

        if (atoi(Identifiant_Recherche) >= atoi(Cle_Min) && atoi(Identifiant_Recherche) <= atoi(Cle_Max)) // si la cle à recherchee est entre Cle_Min et Cle_Max du bloc
        {                                                                                                 // alors on recherche la cle dans ce bloc
            while (!(*trouv) && *j < Buf.nb && !stop)                                                     // stop = cle_courante > cle_recherche
            {                                                                                             // recherche sequentielle dans le bloc
                extraire_chaine_TOVnC(Supprime, j, TAILLE_SUPPRIMER, &Buf);                               // recuperer un le champs de suppression
                if (strcmp(Identifiant_Recherche, Cle_Courrante) == 0 && strcmp(Supprime, "f") == 0)      // la cle est donc trouveee dans le bloc i
                {
                    extraire_chaine_TOVnC(Materiel, j, TAILLE_MATERIEL - 1, &Buf);     // extraire le materiel
                    extraire_chaine_TOVnC(Fonctionne, j, TAILLE_FONCTIONNEMENT, &Buf); // extraire son etat de fonctionnement
                    extraire_chaine_TOVnC(Prix, j, TAILLE_PRIX, &Buf);                 // extraire son prix
                    extraire_chaine_TOVnC(Taille, j, TAILLE_TAILLE, &Buf);             // extraire la taille de la description
                    extraire_chaine_TOVnC(Description, j, atoi(Taille), &Buf);         // extraire la description
                    *trouv = 1;                                                        // mettre trouve a vrai pour arreter la recherche
                    *j = temp_j;                                                       // faire revenir j a la position du premier champs avant decalage (l'identifiant)
                    printf("\n\n---------------- L'enregistrement trouve dans le bloc %d a la position %d ------------------\n", *i, *j);
                    printf("|    -> L'identifiant : %s\n", Cle_Courrante);                     // afficher la cle
                    printf("|    -> Le type materiel : %s\n", Materiel);                       // afficher le materiel
                    if (strcmp(Fonctionne, "f") == 0)                                          // si fonctionne est a vrai
                        printf("|    -> Fonctionnement du materiel: En marche\n", Fonctionne); // le materiel est en marche
                    else                                                                       // sinon (fonctionne="n")
                        printf("|    -> Fonctionnement du materiel: En panne\n", Fonctionne);  // le materiel est en panne
                    printf("|    -> Le prix d'achat du materiel : %s\n", Prix);                // afficher le prix
                    printf("|    -> La Description : %s\n", Description);                      // afficher la description
                    printf("---------------------------------------------------------------------------------------------\n\n");
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
    /**************************************************************************************************************|
    | Identifiant | champs supprime | Type materiel | fonctionne |    Prix   |   taille   | Description (variable) |
    |  (5 bytes)  |   (1 bytes)     |  (12 bytes)   |  (1 bytes) | (6 bytes) |  (3 bytes) |  (max sur 272 bytes)   |
    |**************************************************************************************************************/
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
        buf.tableau[j] = 't';               // remettre le champs supprimer a Vrai

        j += TAILLE_SUPPRIMER + TAILLE_MATERIEL + TAILLE_FONCTIONNEMENT + TAILLE_PRIX; // sauter tous les champs jusqèa champs taille
        extraire_chaine_TOVnC(Taille, &j, TAILLE_TAILLE, &buf);                        // extraire le champs taille de description
        int Nombre_chars_supprime = TAILLE_IDENTIFIANT + TAILLE_SUPPRIMER + TAILLE_MATERIEL - 1 + TAILLE_FONCTIONNEMENT + TAILLE_PRIX + atoi(Taille);

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

    int i = 1,  // le parcours des blocs du fichier original TOVnC
        j = 0,  // le parcours de position du fichier original TOVnC
        i1 = 1, // le parcours des blocs du fichier original TOVC
        j1 = 0, // le parcours de position du fichier original TOVC
        i2 = 1, // le parcours des blocs du fichier original LOVC
        j2 = 0; // le parcours de position du fichier original LOVC

    char Identifiant[TAILLE_IDENTIFIANT + 1],    // numero d'identifiant(cle)
        Supprime[TAILLE_SUPPRIMER + 1],          // supprimer='f' l'element n'a pas ete supprime supprimer='t' sinon
        Materiel[TAILLE_MATERIEL],               // le type du materiel
        Fonctionne[TAILLE_FONCTIONNEMENT + 1],   // fonctionne = 'f', le materiel marche, fonctionne = 'n' sinon
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
            extraire_chaine_TOVnC(Supprime, &j, TAILLE_SUPPRIMER, &Buf);        // extraire le champs Supprimer
            extraire_chaine_TOVnC(Materiel, &j, TAILLE_MATERIEL - 1, &Buf);     // extraire le champs materiel
            extraire_chaine_TOVnC(Fonctionne, &j, TAILLE_FONCTIONNEMENT, &Buf); // extraire le champs d'etat de fonctionnement
            extraire_chaine_TOVnC(Prix, &j, TAILLE_PRIX, &Buf);                 // extraire le champs du prix
            extraire_chaine_TOVnC(Taille, &j, TAILLE_TAILLE, &Buf);             // extraire le champs taille de description
            extraire_chaine_TOVnC(Description, &j, atoi(Taille), &Buf);         // extraire le champs description

            if (strcmp(Supprime, "f") == 0) // si le materiel n'a pas ete supprime logiquement
            {
                concatenate(Destination, Identifiant, "", Materiel, "", Prix, Taille, Description); // concatener les champs: identifiant, materiel, prix, taille et description

                if (strcmp(Fonctionne, "f") == 0) // si le materiel fonctionne
                {                                 // alors le mettre dans le fichier des materiaux en marche (TOVC)
                    printf("\n\nTOVC file");
                    printf("\n %s", Destination);
                }
                else // si le materiel ne fonctionne pas
                {    // alors le mettre dans le fichier des materiaux en panne (LOVC)
                    printf("\n\nin the LOVC file");
                    printf("\n %s", Destination);
                }
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

int main(void)
{
    printf("a printing is needed");
    // Reorganisation_TOVnC(FICHIER_ORIGINAL, FICHIER_MATERIEL_FONCTIONNE, FICHIER_MATERIEL_NON_FONCTIONNE);
    int trouv, i, j;
    Recherche_TOVnC(FICHIER_ORIGINAL, "00010", &trouv, &i, &j);
    Suppression_TOVnC(FICHIER_ORIGINAL, "00010");
    Recherche_TOVnC(FICHIER_ORIGINAL, "00010", &trouv, &i, &j);
}

/*fichier TOVC en marche*/
/********************************************************************************
| Identifiant |Type materiel |   Prix   |   taille   | Description (variable) |
|  (5 bytes)  |(12 bytes)   | (6 bytes) |  (3 bytes) |  (max sur 272 bytes)    |
|*****************************************************************************/

// les fichiers qui seront générés
/*************************
| Identifiant |  Prix    |
|  (5 bytes)  |(6 bytes) /
|************************/
/*« Materiel_en_marche_typeMateriel_TOF.bin ».*/
/*« Materiel_en_marche_typeMateriel_TOF.bin ».*/
/*« Materiel_en_marche_typeMateriel_TOF.bin ».*/
/*« Materiel_en_marche_typeMateriel_TOF.bin ».*/
/*« Materiel_en_marche_typeMateriel_TOF.bin ».*/
/*« Materiel_en_marche_typeMateriel_TOF.bin ».*/

// 1-ouvrir 6 fichier mode nouveau
// 2- parcourir TOVC
// 3- verifier type materiel
// 4- affecter au fichier correspondant
/*







*/
/*fichier LOVC en panne*/
/**********************************************************************************
| Identifiant |Type materiel |   Prix     |   taille   | Description (variable) |
|  (5 bytes)  |(12 bytes)   | (6 bytes)  |  (3 bytes) |  (max sur 272 bytes)    |
|*****************************************************************************/
// 1- afficher tous les materiaux dont le prix [a,b] (1- parcours_LOVC_sequentielle)
// 2- afficher le montant global
//    [ initialiser un compteur : counter=0, counter+=Prix]

/*







*/
/*fichier index TOF / Table index TOF*/
// 1-  why not taking each cle max de chaque bloc de TOVnC as a key f la table d'index et fichier index
