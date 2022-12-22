#include <stdio.h>
#include "lib.h"

int main(void)
{
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
        printf("    11-  Requette a interval selon le prix dans le fichier LOVC (fichiers en panne)\n");
        printf("    12-  Creation table Index et fichier index de TOVnC\n");
        printf("    13-  Afficher Table Index\n");
        printf("    14-  Afficher fichier Index\n");

        // le choix des options
        printf("\n    saisissez le numéro de votre option: ");
        int option;
        scanf(" %i", &option);
        while (option < 1 || option > 11)
        {
            printf("pas d'option correspondante à un tel numéro saisissez un autre: ");
            scanf(" %i", &option);
        }

        switch (option)
        {
        case 1:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *       Chargement initial aleatoire d'un       *\n");
            printf("    *           fichier TOVnC de materiel           *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");

            int nombre_enreg_initial;
            printf("    Nombre de materiel initial que vous voulez generer: ");
            scanf("%i", &nombre_enreg_initial);
            Chargement_initial_TOVnC(FICHIER_ORIGINAL, nombre_enreg_initial);
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
            char *Identifiant_a_rechercher;
            int trouv, i, j;
            printf("");
            scanf("%s", Identifiant_a_rechercher);
            Recherche_TOVnC(FICHIER_ORIGINAL, Identifiant_a_rechercher, &trouv, &i, &j);
            break;

        case 4:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *    Insertion d'un materiel dans le fichier    *\n");
            printf("    *     original TOVnC selon son identifiant      *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            break;

        case 5:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *   Suppression d'un materiel dans le fichier   *\n");
            printf("    *     original TOVnC selon son identifiant     *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            break;

        case 6:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *   Fragmentation du fcihier original TOVnC en  *\n");
            printf("    *            fichier TOVC et LOVC               *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            break;

        case 7:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *        Affichage du fichier de materiel       *\n");
            printf("    *                en marche TOVC                 *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            break;

        case 8:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *       Affichage du fichier de materiel        *\n");
            printf("    *                en panne LOVC                  *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            break;

        case 9:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *       Affichage du fichier de materiel        *\n");
            printf("    *                en panne LOVC                  *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            break;

        case 10:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *       Affichage du fichier de materiel        *\n");
            printf("    *                en panne LOVC                  *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            break;

        case 11:

            printf("    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *       Affichage du fichier de materiel        *\n");
            printf("    *                en panne LOVC                  *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
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
