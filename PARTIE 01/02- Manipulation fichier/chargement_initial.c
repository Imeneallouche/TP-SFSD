#include <stdio.h>
#include "lib.h"

// n est le nombre de valeur (enregistrements) qu'on veut inserer
void Chargement_initial_TOF(char nom_fichier[], int n, float u)
// chargement initial du fichier TOF
{
    fichier_TOF F;
    Tampon buf;
    typeval valeur;
    Tenreg enregistrement;
    Ouvrir_TOF(&F, nom_fichier, 'N');
    int i = 1, j = 0, cpt_ecr;

    for (int k = 0; k < n; k++)
    {
        // enregistrement.cle = extract_nbr(k + 1);
        enregistrement.cle = k;
        // enregistrement.valeur = extract_nbr(k + 1);
        printf("entrez la valeur numero %i : ", k + 1);
        scanf("%i", &valeur);
        enregistrement.supprimer = 0;
        if (j < u * maxBloc)
        {
            buf.tab[j] = enregistrement;
            j++;
        }
        else
        {
            buf.nombre_enreg = j;
            EcrireDir_TOF(F, i, &buf, &cpt_ecr);
            buf.tab[0] = enregistrement;
            i++;

            j = 1;
        }
        // nb+=pas;
    }

    buf.nombre_enreg = j;
    EcrireDir_TOF(F, i, &buf, &cpt_ecr);
    Aff_Entete_TOF(&F, 1, i); // nombre de blocs dans le fichier
    Aff_Entete_TOF(&F, 2, n); // nombre d'enregistrements inseres dans le fichier
    Aff_Entete_TOF(&F, 3, 0); // nombre d'enregistrement supprimes dans le fichier
    Fermer_TOF(F);
}