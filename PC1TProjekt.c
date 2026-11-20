/*
    Program na rezervace stolů v restauraci
    Projekt do předmětu PC1T
    Autor: Martin Smítka

    Program je rozdělen do modulů:
      tables.c/.h        - struktury, dynamické pole stolů, práce se soubory
      reservations.c/.h  - vytvoření, automatický výběr, zrušení/posun rezervace
      ui.c/.h            - menu a výběr dne

    Použil jsem deprecated funkce pro čtení ze souborů, jako je fscanf, apod., což vyžadovalo přidání _CRT_SECURE_NO_WARNINGS
    do preprocesoru, aby se zabránilo varování o bezpečnosti.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tables.h"
#include "reservations.h"
#include "ui.h"

int main(void) {
	srand((unsigned int)time(NULL));    //Inicializace generátoru náhodných čísel pro rezervační kódy - via reservations.c

    Restaurant restaurant;              //Jediná struktura, která se předává funkcím ukazatelem
    Restaurant* r = &restaurant;        
    int userChoice;                     //Co si uživatel vybral z menu

    if (!initRestaurant(r)) return 1;   //Alokace paměti pro stoly

    changeDay(r);                       //Prvotní načtení .txt souboru

    while (1) {
        printMenu(r);

        if (scanf("%d", &userChoice) != 1) break;   // Pokud uživatel zadá neplatný vstup, ukončí program. Viz specifikace scanf
        system("cls");
        r->upperUIScale = 0;

        switch (userChoice) {
        case 1: printTables(r); break;
        case 2: manualReservation(r); break;
        case 3: autoReservation(r); break;
        case 4: modifyReservation(r); break;
        case 5: changeDay(r); break;
        case 0:
            printf("Konec programu.\n");
            freeRestaurant(r);
            return 0;
        default:
            printf("Neplatna volba.\n");
        }
    }

    freeRestaurant(r);                  //Uvolnění paměti i při ukončení neplatným vstupem
    return 0;
}
