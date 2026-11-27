/*
    Modul uživatelského rozhraní (menu, výběr dne)
*/

#include <stdio.h>
#include "tables.h"
#include "ui.h"

void changeDay(Restaurant* r) {         //Funkce pro změnu dne a načtení nového souboru
    const char* num = "0123456789";
    char input[20];
    int day, month, year, c;

    while (1) {
        printf("Pro spravu rezervaci zadejte den (DD.MM.RRRR): ");
        scanf("%19s", input);
        
        if (strspn(input, num) == 2 && input[2] == '.' &&           //kontrola syntaxe vstupu; když neprojde, opakuj
            strspn(input + 3, num) == 2 && input[5] == '.' &&
            strspn(input + 6, num) == 4 && input[10] == '\0' &&
            sscanf(input, "%d.%d.%d", &day, &month, &year) == 3 &&
            day >= 1 && day <= 31 && month >= 1 && month <= 12) {
            break;
        }
        system("cls");
        printf("Neplatne datum, zkuste to znovu.\n");
        r->upperUIScale = 1;
    }

    snprintf(r->currentDay, sizeof r->currentDay, "%02d%02d%04d", day, month, year);
    loadTables(r, r->currentDay);       //Pokud soubor neexistuje, vytvoří se ze šablony

    r->upperUIScale += 1;
}

void printMenu(Restaurant* r) {         //Funkce pro vykreslení menu
	for (; r->upperUIScale < 19; r->upperUIScale++) {   //Pro lepší vzled menu - odsazení od horního okraje
        printf("\n");
    }

    printf("\n MENU \n");
    printf("\n Zvoleny den: %.2s.%.2s.%.4s\n", r->currentDay, r->currentDay + 2, r->currentDay + 4); //Vypis do přehlednějšího formátu
    printf("1. Zobrazit stoly\n");
    printf("2. Vytvorit rezervaci\n");
    printf("3. Automaticka rezervace\n");
    printf("4. Spravovat rezervaci (Zrusit/Posunout)\n");
    printf("5. Zmenit den\n");
    printf("0. Ukoncit\n");
    printf("Volba: ");
}
