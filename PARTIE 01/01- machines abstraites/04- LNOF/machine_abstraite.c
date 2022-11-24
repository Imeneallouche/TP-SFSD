#include <stdio.h>
#include "lib.h"

/*************************************************************|
|                                                             |
|  Ouvrir fichier nom_fichier avec le mode correspondant LnOF |
|                                                             |
|*************************************************************/
void Ouvrir_LnOF(fichier_LNOF *fichier, char nom_fichier[], const char mode)
{
    fichier->en_tete = malloc(sizeof(entete_LNOF)); // allouer dynamiquement une zone en memoire centrale pour l'entete

    if (tolower(mode) == 'n') // si le mode est nouveau ie : le fichier n'existe pas deja alors
    {
        fichier->fichier = fopen(nom_fichier, "wb+"); // ouvrir un fichier en mode ecriture binaire

        fichier->en_tete->num_Bloc_entete = -1;  // initialiser l'adresse de la tete a -1 (NILL)
        fichier->en_tete->enreg_inseres = 0;     // initialiser le nombre d'enregistrement inseres a 0
        fichier->en_tete->enreg_supprimes = 0;   // initialiser le nombre d'enregistrements supprimes a 0
        fichier->en_tete->num_dernier_bloc = -1; // initialiser l'adresse du dernier bloc a -1 (NILL)

        fwrite(fichier->en_tete, sizeof(entete_LNOF), 1, fichier->fichier); // ecrire l'en_tete ds le fichier binaire
    }

    else if (tolower(mode) == 'a') // si le mode est ancien ie:le fichier existe
    {
        fichier->fichier = fopen(nom_fichier, "rb+"); // ouvrir le fichier en mode lecture binaire
        if (fichier->fichier == NULL)
            printf("\n\t\t<<Le fichier n'existe pas...>>"); // si erreur d'ouverture
        else                                                // sinon
        {
            fread(fichier->en_tete, sizeof(entete_LNOF), 1, fichier->fichier);
        } // recuperer le contenu de l'entete ds la variable en_tete
    }

    else
        printf("le mode errone.");
}

/*************************************|
|                                     |
|       Fermer le fichier LnOF        |
|                                     |
|*************************************/
void Fermer_LnOF(fichier_LNOF *fichier)
{
    rewind(fichier->fichier);                                           // on se positionne au debut de fichier
    fwrite(fichier->en_tete, sizeof(entete_LNOF), 1, fichier->fichier); // on enregistre les modifications effectuees sur l'entete
    fclose(fichier->fichier);                                           // on ferme le fichier
    free(fichier->en_tete);                                             // liberer la zone en_tete reservees
}

/*********************************************|
|                                             |
|      Ecrire buf dans le i eme bloc LnOF     |
|                                             |
|********************************************/
void EcrireDir_LnOF(fichier_LNOF *fichier, int i, TBloc_LnOF *buf, int *cpt_ecr) // ecriture directe du contenu de buf dans le fichier a la position i
{
    rewind(fichier->fichier);
    fseek(fichier->fichier, sizeof(entete_LNOF) + i * sizeof(TBloc_LnOF), SEEK_SET); // se positionner a la place exacte
    fwrite(buf, sizeof(TBloc_LnOF), 1, fichier->fichier);                            // ecriture
    (*cpt_ecr) = (*cpt_ecr) + 1;
}

/********************************************|
|                                            |
|      Lire le i eme bloc dans buf LnOF      |
|                                            |
|********************************************/
void LireDir_LnOF(fichier_LNOF *fichier, int i, TBloc_LnOF *buf, int *cpt_lect) // lecture directe du contenu de fichier a la position i dans le buf
{
    rewind(fichier->fichier);
    fseek(fichier->fichier, sizeof(entete_LNOF) + i * sizeof(TBloc_LnOF), SEEK_SET); // se positionner a la place exacte
    fread(buf, sizeof(TBloc_LnOF), 1, fichier->fichier);                             // lecture
    (*cpt_lect) = (*cpt_lect) + 1;
}

/*********************************************|
|                                             |
|  Retoure la i ème valeur de l'entete  LnOF  |
|                                             |
|*********************************************/
void aff_entete_LnOF(fichier_LNOF *fichier, int num_caract, int val) // affecter a la caracteristique num_caract la val
{
    if (num_caract == 1)
        fichier->en_tete->num_Bloc_entete = val;
    else if (num_caract == 2)
        fichier->en_tete->enreg_inseres = val;
    else if (num_caract == 3)
        fichier->en_tete->enreg_supprimes = val;
    else if (num_caract == 4)
        fichier->en_tete->num_dernier_bloc = val;
}

/********************************************|
|                                            |
|  Retoure la i ème valeur del'entete  LnOF  |
|                                            |
|********************************************/
int entete_LnOF(fichier_LNOF *fichier, int num_caract) // retourner la cracterstique num_caract ds val
{
    if (num_caract == 1)
        return fichier->en_tete->num_Bloc_entete;
    else if (num_caract == 2)
        return fichier->en_tete->enreg_inseres;
    else if (num_caract == 3)
        return fichier->en_tete->enreg_supprimes;
    else if (num_caract == 4)
        return fichier->en_tete->num_dernier_bloc;
    else
        printf("\n\t\t<<Le numero errone>>");
}

/********************************************|
|                                            |
|   Retourne le numéro du nouveau bloc LnOF  |
|                                            |
|********************************************/
void alloc_bloc_LnOF(fichier_LNOF *fichier, char nom_fichier[], TBloc_LnOF *buf) // initialise un buffer
{
    int i;
    (*buf).suivant = -1;     // initialiser le champs suivant a NIL
    (*buf).nombre_enreg = 0; // initialiser le nombre de blocs a 0
    for (i = 0; i < maxBloc; i++)
    {
        (*buf).tab[i].supprimer = 1; // initialiser tout les champs efface de tableau a vrai
    }
}