#include <stdio.h>
#include "lib.h"

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
        printf("    13-  Chargement de table index\n");
        printf("    14-  Afficher Table Index\n");
        printf("    15-  Afficher fichier Index strcuture sous forme TOF\n");
        printf("    16-  Insertion materiel a la fin du fichier TOVnC,dans la table index et fichier index\n");
        printf("    17-  Quitter le programme");

        // le choix des options
        printf("\n\n    saisissez le numero de votre option: ");
        scanf(" %i", &int_answers);
        while (int_answers < 1 || int_answers > 17)
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
            printf("    *        Chargement de la table d'Index         *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            Chargement_Table_Index_TOF(FICHIER_INDEX);
            break;

        case 14:

            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *              Afficher Table Index             *\n");
            printf("    *                                               *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            Afficher_Table_Index(Index);
            break;

        case 15:

            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *       Afficher fichier Index strcuture        *\n");
            printf("    *                sous forme TOF                 *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            afficher_fichier_Index_TOF(FICHIER_INDEX);
            break;

        case 16:

            printf("\n\n    *************************************************\n");
            printf("    *                                               *\n");
            printf("    *  Insertion materiel a la fin du fichier TOVnC *\n");
            printf("    *      dans la table index et fichier index     *\n");
            printf("    *                                               *\n");
            printf("    *************************************************\n\n");
            Insertion_TnOVnC(FICHIER_ORIGINAL);
            break;

        case 17:
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
