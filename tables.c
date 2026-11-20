/*
    Modul pro práci se stoly a soubory
    Použity "deprecated" funkce fscanf, sprintf apod. -> v preprocesoru je potřeba _CRT_SECURE_NO_WARNINGS
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tables.h"

int initRestaurant(Restaurant* r) {
	r->tables = (Table*)malloc(INITIAL_TABLES * sizeof(Table));     //Nech si přidělit dostatek paměti pro počet stolů (ten je definovaný v hlavičce) - tzn. uvnitř proměnné je ukazatel na pole s dostatečnou přidělenou pamětí
    if (r->tables == NULL) {
        printf("Chyba: nedostatek pameti\n");                       //Sike
        return 0;
    }
    r->tableCapacity = INITIAL_TABLES;                              //Nastavit defaultni hodnoty
    r->currentTableCount = 0;
    r->currentFile[0] = '\0';
    r->currentDay[0] = '\0';
    r->upperUIScale = 0;
    return 1;
}

void freeRestaurant(Restaurant* r) {
    free(r->tables);
    r->tables = NULL;
    r->tableCapacity = 0;
    r->currentTableCount = 0;
}

static int growTables(Restaurant* r) {          // Pomocná funkce: zvětší pole, pokud je plné
    if (r->currentTableCount < r->tableCapacity) return 1;

    int newCapacity = r->tableCapacity * 2;
    Table* temp = (Table*)realloc(r->tables, newCapacity * sizeof(Table));
    if (temp == NULL) {
        printf("Chyba: nedostatek pameti\n");
        return 0;                               // Puvodni pole zustava platne
    }
    r->tables = temp;
    r->tableCapacity = newCapacity;
    return 1;
}

void createFileFromTemplate(char* currentFile) {    //Funkce pro vytvoření textového souboru
    FILE* templateFile = fopen("templateDay.txt", "r");
    if (templateFile == NULL) {
        printf("Chyba: sablona neni pristupna\n");
        return;
    }

    FILE* newFile = fopen(currentFile, "w");
    if (newFile == NULL) {
        printf("Chyba: nelze vytvorit cilovy soubor %s.\n", currentFile);
        fclose(templateFile);       //Zavrit template, pokud vytvoření neprojde
        return;
    }

    int id, capacity, isReserved, reservationNumber;
    char time[TIME_LEN];            // Cas je retezec (napr. "-"), ne cislo

    // Čtení ze šablony a okamžitý zápis do nového souboru
    while (fscanf(templateFile, "%d %d %d %d %9s", &id, &capacity, &isReserved, &reservationNumber, time) == 5) {
        fprintf(newFile, "%d %d %d %d %s\n", id, capacity, isReserved, reservationNumber, time);
    }
    fclose(templateFile);
    fclose(newFile);
}

void loadTables(Restaurant* r, char* date) {        //Funkce pro načtení stolů z .txt
    sprintf(r->currentFile, "%s.txt", date);        //Vepsání konkrétního dne do názvu souboru
    FILE* file = fopen(r->currentFile, "r");        //Otevři soubor v režimu read

    if (file == NULL) {                             //Pokud soubor neexistuje, vytvoř ho a zkus to znovu
        createFileFromTemplate(r->currentFile);
        file = fopen(r->currentFile, "r");
        if (file == NULL) {
            printf("Nepodarilo se vytvorit novy textovy soubor");
            r->currentTableCount = 0;
            return;
        }
    }

    r->currentTableCount = 0;

    while (growTables(r)) {                         //Načti jednotlivé stoly do dynamického pole
        Table* t = &r->tables[r->currentTableCount];
        if (fscanf(file, "%d %d %d %d %9s",
            &t->id, &t->capacity, &t->isReserved,
            &t->reservationNumber, t->time) != 5) {
            break;
        }
        r->currentTableCount++;
    }
    fclose(file);
}

void saveTables(Restaurant* r) {                    //Funkce pro uložení stolů do .txt
    FILE* file = fopen(r->currentFile, "w");
    if (file == NULL) {
        printf("Nelze zapsat do souboru %s!\n", r->currentFile);
        return;
    }

    for (Table* t = r->tables; t < r->tables + r->currentTableCount; t++) {   //Průchod pole pomocí ukazatele
        fprintf(file, "%d %d %d %d %s\n",
            t->id, t->capacity, t->isReserved,
            t->reservationNumber, t->time);
    }

    fclose(file);
}

void printTables(Restaurant* r) {                   //Funkce pro výpis jednotlivých stolů
    printf("Stav stolu (%s) \n", r->currentFile);
    for (int i = 0; i < r->currentTableCount; i++) {
        Table* t = &r->tables[i];
        printf("Stul %d | Kapacita: %d | ", t->id, t->capacity);
        if (t->isReserved) {
            printf("REZERVOVANO (Kod: %d, Cas: %s)\n", t->reservationNumber, t->time);
        }
        else {
            printf("VOLNO\n");
        }
    }
    printf("--------------------------\n");

    r->upperUIScale = r->upperUIScale + 8;
}


