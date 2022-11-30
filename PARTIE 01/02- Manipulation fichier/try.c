#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include "lib.h"
void Generer_identifiant(char chaine[], int length, int number)
{
    for (int i = length - 1; i >= 0; i--)
    {
        chaine[i] = number % 10 + '0';
        number = number / 10;
    }
    chaine[length] = '\0';
}

int main(void)
{
    char ident[TAILLE_IDENTIFIANT];
    Generer_identifiant(ident, 5, 98);
    printf("%s", ident);
}