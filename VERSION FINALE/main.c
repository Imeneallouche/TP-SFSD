#include <stdio.h>
#include "lib.h"

int main(void)
{
    int int_answers;
    char char_answers[B];
    int trouv, i, j;

    while (1)
    {

        printf("    *************************************************\n");
        printf("    *                                               *\n");
        printf("    *   Bienvenu sur notre programme fascinant!     *\n");
        printf("    *                                               *\n");
        printf("    *************************************************\n\n\n");

        printf("    veuillez consulter nos diverses fonctions et en choisir une:\n");
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
        printf("    14-  Afficher fichier Index\n");

        // le choix des options
        printf("\n    saisissez le numéro de votre option: ");
        scanf(" %i", &int_answers);
        while (int_answers < 1 || int_answers > 11)
        {
            printf("pas d'option correspondante à un tel numéro saisissez un autre: ");
            scanf(" %i", &int_answers);
        }

        switch (int_answers)
        {
        case 1:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *       Chargement initial aleatoire d'un       *\n");
            printf("    *           fichier TOVnC de materiel           *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");

            printf("    Nombre de materiel initial que vous voulez generer: ");
            scanf("%i", &int_answers);
            Chargement_initial_TOVnC(FICHIER_ORIGINAL, int_answers);
            break;

        case 2:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *         Affichage du fichier materiel         *\n");
            printf("    *                original TOVnC                 *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            afficher_fichier_TOVnC(FICHIER_ORIGINAL);
            break;

        case 3:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *    Recherche d'un materiel dans le fichier    *\n");
            printf("    *     original TOVnC selon son identifiant      *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            printf("|    -> L'identifiant a rechercher: "); // demander l'identifiant
            scanf("%i", int_answers);                       // recevoir l'identifiant
            Generer_Chaine(char_answers, TAILLE_IDENTIFIANT, int_answers);
            Recherche_TOVnC(FICHIER_ORIGINAL, char_answers, &trouv, &i, &j);
            break;

        case 4:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *    Insertion d'un materiel dans le fichier    *\n");
            printf("    *     original TOVnC selon son identifiant      *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            Insertion_TOVnC(FICHIER_ORIGINAL);
            break;

        case 5:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *   Suppression d'un materiel dans le fichier   *\n");
            printf("    *     original TOVnC selon son identifiant     *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            printf("|    -> L'identifiant a supprimer: "); // demander l'identifiant
            scanf("%i", int_answers);                      // recevoir l'identifiant
            Generer_Chaine(char_answers, TAILLE_IDENTIFIANT, int_answers);
            Suppression_TOVnC(FICHIER_ORIGINAL, char_answers);
            break;

        case 6:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *   Fragmentation du fcihier original TOVnC en  *\n");
            printf("    *            fichier TOVC et LOVC               *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            Reorganisation_TOVnC(FICHIER_ORIGINAL, FICHIER_MATERIEL_FONCTIONNE, FICHIER_MATERIEL_NON_FONCTIONNE);
            break;

        case 7:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *        Affichage du fichier de materiel       *\n");
            printf("    *                en marche TOVC                 *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            afficher_fichier_TOVC(FICHIER_MATERIEL_FONCTIONNE);
            break;

        case 8:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *       Affichage du fichier de materiel        *\n");
            printf("    *                en panne LOVC                  *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            afficher_fichier_LOVC(FICHIER_MATERIEL_NON_FONCTIONNE);
            break;

        case 9:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *      Generation fichiers TOF a partir du      *\n");
            printf("    *     fichier TOVnC selon le champs materiel    *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            Generation_fichiers_Materiel(FICHIER_MATERIEL_FONCTIONNE);
            break;

        case 10:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *     Afficher fichier TOF resultant des        *\n");
            printf("    *                 generations                   *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            printf("|    -> Le fichier a afficher:  ");                           // demander le type du materiel
            for (int_answers = 1; int_answers <= NB_TYPE_MATERIEL; int_answers++) // la liste des matreiel a proposer sur l'utilisateur
            {
                printf("    %i - %s\n", int_answers, MATERIAL_LIST[int_answers - 1]);
            }
            printf("    votre choix: ");
            scanf("%i", int_answers); // le numero du materiel
            while (int_answers < 1 || int_answers > NB_TYPE_MATERIEL)
            {
                printf("    numero inexistant, veuillez entrer un autre entre [%i, %i]: ", 1, NB_TYPE_MATERIEL);
                scanf("%i", int_answers);
            }
            strcpy(char_answers, MATERIAL_LIST[int_answers - 1]); // remplir le champs materiel
            afficher_fichier_TOF("again");
            break;

        case 11:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *       Requette a interval selon le prix       *\n");
            printf("    *              dans le fichier LOVC             *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            printf("");
            break;

        case 12:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *       Affichage du fichier de materiel        *\n");
            printf("    *                en panne LOVC                  *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            break;
        }
    }

    return 0;
}
