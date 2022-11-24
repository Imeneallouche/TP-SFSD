#include <stdio.h>
#include "lib.h"

/************************************************************
 |                                                          |
 |               Ouvrir fichier de nom_fichier              |
 |                                                          |
 ************************************************************/
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

/************************************************************
 |                                                          |
 |       Procedure de fermeture d'un fichier LOVC           |
 |                                                          |
 ************************************************************/
void fermer_LOVC(fichier_LOVC *f)
{
    rewind(f->fichier);                                       // repositionnement du curseur en debut de fichier
    fwrite(&(f->entete), sizeof(Entete_LOVC), 1, f->fichier); // sauvegarde l'entete en debut du fichier
    fclose(f->fichier);                                       // fermeture du fichier
    free(f);                                                  // libere l'espace memoire occupe par la structure
}

/************************************************************
 |                                                          |
 |       Procedure de lecture d'un bloc methode LOVC        |
 |                                                          |
 ************************************************************/
void LireDir_LOVC(fichier_LOVC *f, int i, Buffer_LOVC *buf, int *cpt_lect)
{
    // positionnement au debut du bloc numero i
    fseek(f->fichier, (sizeof(Entete_LOVC) + sizeof(Tbloc_LOVC) * (i - 1)), SEEK_SET);
    // lecture d'un bloc de caractere correspondant a la taille du bloc dans le buffer
    fread(buf, sizeof(Buffer_LOVC), 1, f->fichier);
    (*cpt_lect) = (*cpt_lect) + 1;
}

/************************************************************
 |                                                          |
 |       Procedure d'ecriture d'un bloc methode LOVC        |
 |                                                          |
 ************************************************************/
void ecrireDir_LOVC(fichier_LOVC *f, int i, Buffer_LOVC *buf, int *cpt_ecr)
{

    // positionnement au debut du bloc num�ro i
    fseek(f->fichier, sizeof(Entete_LOVC) + sizeof(Tbloc_LOVC) * (i - 1), SEEK_SET);
    // ecriture du contenu du buffer dans le bloc numero i du fichier
    fwrite(buf, sizeof(Buffer_LOVC), 1, f->fichier);
    (*cpt_ecr) = (*cpt_ecr) + 1;
}

/************************************************************
 |                                                          |
 |      Fonction de lecture de l'entete methode LOVC        |
 |                                                          |
 ************************************************************/
int entete_LOVC(fichier_LOVC *f, int i)
{
    if (i == 1)
        return (f->entete.Num_premier_bloc); // l'adresse du premier bloc
    else if (i == 2)
        return (f->entete.pos_libre_dernier_bloc); // la position libre dans le dernier bloc
    else if (i == 3)
        return (f->entete.nb_chars_inseres); // numero du bloc representatnt la tete du fichier
    else if (i == 4)
        return (f->entete.nb_chars_supprimes); // nombre de caracteres inseres (effaces non inclus)
    else
        retrun NULL;
}

/************************************************************
 |                                                          |
 |    Procedure de modification de l'entete methode LOVC    |
 |                                                          |
 ***********************************************************/
void aff_entete_LOVC(fichier_LOVC *f, int i, int val)
{
    if (i == 1)
        f->entete.Num_premier_bloc = val; // le numero du premier bloc
    else if (i == 2)
        f->entete.pos_libre_dernier_bloc = val; // la position libre dans le denier bloc
    else if (i == 3)
        f->entete.nb_chars_inseres = val; // nombre de caractères inseres dans le fichier
    else if (i == 4)
        f->entete.nb_chars_supprimes = val; // nombre de caractères supprimes dans le fichier
}

/************************************************************
 |                                                          |
 |         allocation d'un nouveau bloc methode LOVC        |
 |                                                          |
 ***********************************************************/

int alloc_bloc_LOVC(fichier_LOVC *f)
{
    Buffer_LOVC *buf;
    aff_entete_LOVC(f, 1, entete_LOVC(f, 1) + 1); // incrementation du nombre de bloc alloues

    return entete_LOVC(f, 1); /* retourner le numero du bloc alloue (egal au nombre de blocs)
                                 car l'allocation se fait toujours a la fin du fichier */
}

/*
void alloc_bloc_LOVC(fichier_LOVC *fichier, int *cpt_lect, int *cpt_ecr)
{

    Buffer_LOVC *buf;                                          // le nouveau buffer
    LireDir_LOVC(fichier, entete(fichier, 3), buf, cpt_lect);  // lecture du bloc correspondant a la queue
    buf->suivant = entete(fichier, 1) + 1;                     // mise a jour dui suvant de la queue au bloc correspondant a la nouvelle queue
    ecrireDir_LOVC(fichier, entete(fichier, 3), buf, cpt_ecr); // ecriture du bloc de queue dans le fichier
    aff_entete_LOVC(fichier, 3, entete(fichier, 1) + 1);       // mise a jour du numero du bloc correspondant a la nouvelle queue dan sl'entete
    buf->suivant = -1;                                         // mise a jour du suivant a nill
    sprintf(buf->tab, "%s", "");                               // vider la chaine du buffer
    ecrireDir_LOVC(fichier, entete(fichier, 3), buf, cpt_ecr); // ecriture du buffer dans le bloc representatnt la nouvelle queue
    aff_entete(fichier, 1, entete(fichier, 1) + 1);            // incrementation du nombre de bloc alloues
}
*/