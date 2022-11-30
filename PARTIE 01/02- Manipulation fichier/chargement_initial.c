#include <stdio.h>
#include <stdlib.h>
#include "lib.h"

// n est le nombre de valeur (enregistrements) qu'on veut inserer
void Chargement_initial_TOVnC(char nom_fichier[], int n)
// chargement initial du fichier TOF
{
    fichier_TOVnC *F;
    Ouvrir_TOVnC(F, FICHIER_ORIGINAL, 'N');
    Tampon buf;
    int i, j;
    for (i = 0; i < n; i++)
    {
        char IDENTIFIANT[TAILLE_IDENTIFIANT];                                // la chaine ou l'identifiant du materiel est stocke
        Generer_Chaine(IDENTIFIANT, TAILLE_IDENTIFIANT, i);                  // generer l'identifiant unique (numero successives pour garder l'ordre)
        j = Randomizeed_Numbers(0, NB_TYPE_MATERIEL - 1);                    // generer l'index du type de materiel aleatoirement
        char *MATERIAL = MATERIAL_LIST[j];                                   // tirer le type du materiel de la liste  des materiels
        char *PRIX;                                                          // la chaine ou le prix du materiel est stocke
        Generer_Chaine(PRIX, TAILLE_PRIX, Randomizeed_Numbers(0, PRIX_MAX)); // generer le prix du materiel aleartoirement
        printf("\n Entrez les caracteristiques de votre materiel: ");        // demander la description du materiel de l'utilisateur
        char *DESCRIPTION;                                                   // la chaine ou l'identifiant du materiel est stocke
        scanf("%s", DESCRIPTION);                                            // Lire la description de l'utilisateur
        printf("l'identifiant: %s\n material: %s\n prix: %s\n description: %s\n", IDENTIFIANT, MATERIAL, PRIX, DESCRIPTION);
    }
}

int main(void)
{
    Chargement_initial_TOVnC(FICHIER_ORIGINAL, 3);
}
