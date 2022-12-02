#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
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

            rewind(f->fichier); /// positionnement au début du fichier
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
    free(f);
}

/********************************************|
|                                            |
|    Lire le i eme bloc dans buf TOVnC       |
|                                            |
|********************************************/
void LireDir_TOVnC(fichier_TOVnC *f, int i, Tbloc_TOVnC *buf)
{
    /// on se jitionne au début du i ème bloc puis on le lit dans buf
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
    if (i == 2)
        return (f->entete.nbr_caract_insert);
    if (i == 3)
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
    /// on se positionne au début du i ème bloc puis on écrit dans fichier->f
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
    if (i == 2)
        f->entete.nbr_caract_insert = val;
    if (i == 3)
        f->entete.nbr_caract_supp = val;
    else
        printf("Parametre inexistant dans l'entete\n");
}

/**********************************************|
|                                              |
|   Retourne le numéro du nouveau bloc TOVnC   |
|                                              |
|**********************************************/
int Alloc_bloc_TOVnC(fichier_TOVnC *f)
{
    int i = Entete_TOVnC(f, 1);    // le nombre de bloc = le numero du dernier bloc -1
    Aff_Entete_TOVnC(f, 1, i + 1); // mettre a jour le nombre de bloc dans l'entete
    return i;                      // le nombre anciens de bloc = numero du dernier bloc
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
    if (i == 2) // nombre d'enregistrements inseres
        f->entete.enreg_inseres = val;
    if (i == 3) // nombre d'enregistrements supprimes
        f->entete.enreg_supprimes = val;
    else
        printf("Parametre inexistant dans l'entete\n");
}

/********************************************|
|                                            |
|   Retourne le numéro du nouveau bloc TOF   |
|                                            |
|********************************************/
int Alloc_bloc_TOF(fichier_TOF *f)
{
    int i = Entete_TOF(*f, 1);   // le nombre de bloc = le numero du dernier bloc -1
    Aff_Entete_TOF(f, 1, i + 1); // mettre a jour le nombre de bloc dans l'entete
    return i;                    // le nombre anciens de bloc = numero du dernier bloc
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

            rewind(f->fichier); /// positionnement au début du fichier
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
    /// on se jitionne au début du i ème bloc puis on le lit dans buf
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
    if (i == 2)
        return (f->entete.pos_libre_dernier_bloc);
    if (i == 3)
        return (f->entete.nbr_caract_insert);
    if (i == 4)
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
    /// on se positionne au début du i ème bloc puis on écrit dans fichier->f
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
    if (i == 2)
        f->entete.pos_libre_dernier_bloc = val;
    if (i == 3)
        f->entete.nbr_caract_insert = val;
    if (i == 4)
        f->entete.nbr_caract_supp = val;
    else
        printf("Parametre inexistant dans l'entete\n");
}

/********************************************|
|                                            |
|  Retourne le numéro du nouveau bloc TOVC   |
|                                            |
|********************************************/
int Alloc_bloc_TOVC(fichier_TOVC *f)
{
    int i = Entete_TOVC(f, 1);    // le nombre de bloc = le numero du dernier bloc -1
    Aff_Entete_TOVC(f, 1, i + 1); // mettre a jour le nombre de bloc dans l'entete
    return i;                     // le nombre anciens de bloc = numero du dernier bloc
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
|                                                           , |
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
        printf("Mode d'ouverture erronné\n");
    }
}

/**************************************|
|                                      |
|       Fermer le fichier TOVC         |
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
|      Lire le i eme bloc dans buf TOVC       |
|                                         ,   |
|*********************************************/
void LireDir_LOVC(fichier_LOVC *f, int i, Tampon_LOVC *buf, int *cpt_lect)
{
    // positionnement au debut du bloc numero i
    fseek(f->fichier, (sizeof(Entete_LOVC) + sizeof(Tbloc_LOVC) * (i - 1)), SEEK_SET);
    // lecture d'un bloc de caractere correspondant a la taille du bloc dans le buffer
    fread(buf, sizeof(Tampon_LOVC), 1, f->fichier);
    (*cpt_lect) = (*cpt_lect) + 1;
}

/**********************************************|
|                                              |
|      Ecrire buf dans le i eme bloc TOVC      |
|                                              |
|*********************************************/
void ecrireDir_LOVC(fichier_LOVC *f, int i, Tampon_LOVC *buf, int *cpt_ecr)
{

    // positionnement au debut du bloc num�ro i
    fseek(f->fichier, sizeof(Entete_LOVC) + sizeof(Tbloc_LOVC) * (i - 1), SEEK_SET);
    // ecriture du contenu du buffer dans le bloc numero i du fichier
    fwrite(buf, sizeof(Tampon_LOVC), 1, f->fichier);
    (*cpt_ecr) = (*cpt_ecr) + 1;
}

/*********************************************|
|                                             |
|  Retoure la i ème valeur del'entete  TOVC   |
|                                             |
|*********************************************/
int entete_LOVC(fichier_LOVC *f, int i)
{
    if (i == 1)
        return (f->entete.Num_premier_bloc); // l'adresse du premier bloc
    if (i == 2)
        return (f->entete.Num_dernier_bloc);
    if (i == 3)
        return (f->entete.pos_libre_dernier_bloc); // la position libre dans le dernier bloc
    if (i == 4)
        return (f->entete.nb_chars_inseres); // numero du bloc representatnt la tete du fichier
    if (i == 5)
        return (f->entete.nb_chars_supprimes); // nombre de caracteres inseres (effaces non inclus)
    else
    {
        printf("Parametre inexistant dans l'entete\n");
        return -1;
    }
}

/**********************************************|
|                                              |
|  modifie la i ème valeur de l'entete  TOVC   |
|                                              |
|**********************************************/
void aff_entete_LOVC(fichier_LOVC *f, int i, int val)
{
    if (i == 1)
        f->entete.Num_premier_bloc = val; // le numero du premier bloc
    if (i == 2)
        f->entete.Num_dernier_bloc = val; // le numero du dernier bloc
    if (i == 3)
        f->entete.pos_libre_dernier_bloc = val; // la position libre dans le denier bloc
    if (i == 4)
        f->entete.nb_chars_inseres = val; // nombre de caractères inseres dans le fichier
    if (i == 5)
        f->entete.nb_chars_supprimes = val; // nombre de caractères supprimes dans le fichier
    else
        printf("Parametre inexistant dans l'entete\n");
}

/*********************************************|
|                                             |
|   Retourne le numéro du nouveau bloc TOVC   |
|                                             |
|*********************************************/
int alloc_bloc_LOVC(fichier_LOVC *fichier, int *cpt_lect, int *cpt_ecr, Tampon_LOVC *buf)
{
    LireDir_LOVC(fichier, entete_LOVC(fichier, 2), buf, cpt_lect);  // lecture du bloc correspondant a la queue
    buf->suivant = entete_LOVC(fichier, 1) + 1;                     // mise a jour dui suvant de la queue au bloc correspondant a la nouvelle queue
    ecrireDir_LOVC(fichier, entete_LOVC(fichier, 2), buf, cpt_ecr); // ecriture du bloc de queue dans le fichier
    aff_entete_LOVC(fichier, 2, entete_LOVC(fichier, 1) + 1);       // mise a jour du numero du bloc correspondant a la nouvelle queue dan sl'entete
    buf->suivant = -1;                                              // mise a jour du suivant a nill
    sprintf(buf->tab, "%s", "");                                    // vider la chaine du buffer
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
    for (int i = length - 1; i >= 0; i--)
    {
        chaine[i] = number % 10 + '0';
        number = number / 10;
    }
}

void concatenate(char destination[], char *identifiant, char *materiel, char fonctionne, char *prix, char *taille, char *description)
{
    destination[0] = '\0';
    strncat(destination, identifiant, TAILLE_IDENTIFIANT);
    strncat(destination, materiel, TAILLE_MATERIEL);
    strncat(destination, &fonctionne, TAILLE_FONCTIONNEMENT);
    strncat(destination, prix, TAILLE_PRIX);
    strncat(destination, taille, TAILLE_TAILLE);
    strcat(destination, description);
}

void Ecrire_chaine_TOVnC(char nom_fichier, char chaine[], int *i, int *j, Tampon_TOVnC *Buf)
{
    fichier_TOVnC *F;
    if (j + strlen(chaine) > B) // inserer le nouvel element dans un nouveau bloc
    {
        // 3-write the bloc
        i = Alloc_bloc_TOVnC(F);
        j = 0;
        // we will insert the string here
        // update the j ou hiya rayha
        // 2- update the bloc: la premiere pos libre
        // 3- update the bloc: la cle max
        // 3- update l'entete: number of chars inserted
    }
    else // inserer dans le bloc courant
    {
        // 1- inserer dans la premiere pos libre
        strcat(Buf->tableau, chaine);
        // 2- update the bloc: la premiere pos libre
        // 3- update the bloc: la cle max
        // 3- update l'entete: number of chars inserted
    }
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
    Ouvrir_TOVnC(F, FICHIER_ORIGINAL, 'N');
    Tampon_TOVnC buf;
    int *i = 0, // le numero du bloc pour le parcours entre bloc
        *j = 0, // la position dans le bloc pour le parcours interbloc
        k,      // le numero de l'element insere de 1 a n
        l;      // la longueur total de l'enregistrement ajoute

    char Identifiant[TAILLE_IDENTIFIANT],    // numero d'identifiant(cle)
        Supprimer = 'f',                     // supprimer='f' l'element n'a pas ete supprime supprimer='t' sinon
        Materiel[TAILLE_MATERIEL],           // le type du materiel
        Fonctionne,                          // fonctionne = 'f', le materiel marche, fonctionne = 'n' sinon
        Prix[TAILLE_PRIX],                   // le ptix du materiel
        Taille[TAILLE_TAILLE],               // taille du champs description
        Description[TAILLE_MAX_DESCRIPTION]; // la description (caracteristiques) du materiel

    for (k = 0; k < n; k++)
    {
        int cle = 10 * k;                                                        // l'identifiant= multiple de 10 pour garder l'ordre et possibilité d'insertion
        Generer_Chaine(Identifiant, TAILLE_IDENTIFIANT, cle);                    // generer l'identifiant sous forme de chaine sur 5 positions
        strcpy(Materiel, MATERIAL_LIST[Random_Number(0, NB_TYPE_MATERIEL - 1)]); // tirer un materiel de la liste  des materiels selon index genere aleartoirement
        Generer_Chaine(Prix, TAILLE_PRIX, Random_Number(0, PRIX_MAX));           // generer le prix du materiel aleartoirement

        printf("\n\n.........................\n");
        printf(".                       .\n");
        printf(". element numero : %i    .\n", k);
        printf(".                       .\n");
        printf(".........................\n");
        printf("identifiant: %.5s\n", Identifiant);
        printf("materiel: %.12s\n", Materiel);
        printf("prix: %.6s\n", Prix);
        printf("Description de votre materiel: ");                  // demander la description du materiel de l'utilisateur
        scanf(" %[^\n]", Description);                              // Lire la description de l'utilisateur
        Generer_Chaine(Taille, TAILLE_TAILLE, strlen(Description)); // taille du champs de la description
        printf("taille de description: %.3s\n", Taille);
        printf("description: %s\n", Description);

        l = TAILLE_IDENTIFIANT + strlen(Materiel) + TAILLE_FONCTIONNEMENT + TAILLE_PRIX + TAILLE_TAILLE + strlen(Description);
        char Enreg[l];
        concatenate(Enreg, Identifiant, Materiel, Fonctionne, Prix, Taille, Description);
        printf("l'element sera insere sous cette forme: %s\n", Enreg);
        // Ecrire_chaine_TOVnC();
    }
}

int main(void)
{
    printf("a printing is needed");
    Chargement_initial_TOVnC(FICHIER_ORIGINAL, 3);
}