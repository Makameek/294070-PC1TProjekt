/*
    Modul uživatelského rozhraní (menu, výběr dne)
*/

#include <stdio.h>
#include "tables.h"
#include "ui.h"

void changeDay(Restaurant* r) {         //Funkce pro změnu dne a načtení nového souboru
    printf("Pro spravu rezervaci vyberte den: ");
    scanf("%19s", r->currentDay);
    loadTables(r, r->currentDay);       //Pokud soubor neexistuje, vytvoří se ze šablony

    r->upperUIScale = 1;
}

void printMenu(Restaurant* r) {         //Funkce pro vykreslení menu
	for (; r->upperUIScale < 14; r->upperUIScale++) {   //Pro lepší vzled menu - odsazení od horního okraje
        printf("\n");
    }

    printf("\n MENU \n");
    printf("\n Zvoleny den: %s\n", r->currentDay);
    printf("1. Zobrazit stoly\n");
    printf("2. Vytvorit rezervaci\n");
    printf("3. Automaticka rezervace\n");
    printf("4. Spravovat rezervaci (Zrusit/Posunout)\n");
    printf("5. Zmenit den\n");
    printf("0. Ukoncit\n");
    printf("Volba: ");
}
