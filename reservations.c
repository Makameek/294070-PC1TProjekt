/*
    Modul s logikou rezervací (manuální vytvoření, automatické vytvoření , zrušení/posun)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tables.h"
#include "reservations.h"

static void confirmReservation(Restaurant* r, Table* t, const char* timeStr) {   //Pomocná funkce (v2): zapíše rezervaci do stolu
    t->isReserved = 1;
    t->reservationNumber = generateReservationCode(r);
    strcpy(t->time, timeStr);

    printf("\nRezervovan stul c. %d\n", t->id);
    printf("Vas rezervacni kod je: %d\n", t->reservationNumber);
    saveTables(r);
}

void manualReservation(Restaurant* r) {     //Funkce pro manuální zadání rezervace s kontrolou dostupnosti
    int personCount;
    char timeStr[TIME_LEN];
    int chosenTableId;
    printf("Pocet osob: ");
    scanf("%d", &personCount);
    printf("Cas prichodu: ");
    scanf("%9s", timeStr);

    printTables(r);
    printf("Rezervovat stul cislo: ");
    scanf("%d", &chosenTableId);
    chosenTableId--;    //Odečíst 1 kvůli prvnímu indexu 0 v poli

    if (chosenTableId < 0 || chosenTableId >= r->currentTableCount) {   //Kontrola, že stůl existuje
        printf("\nTakovy stul neexistuje");
        r->upperUIScale = r->upperUIScale + 4;
        return;
    }

	Table* t = &r->tables[chosenTableId];                   //Ukazatel na vybraný stůl - ten kontrolujeme, popř. zapisujeme rezervaci

    if (!t->isReserved && t->capacity >= personCount) {   //Kontrola obsazenosti a velikosti stolu
        confirmReservation(r, t, timeStr);
        r->upperUIScale = r->upperUIScale + 6;
    }
    else if (t->isReserved) {
        printf("\nStul c. %d je jiz zarezervovany", t->id);
        r->upperUIScale = r->upperUIScale + 4;
    }
    else {
        printf("\nStul c. %d ma malo mist pro %d osob", t->id, personCount);
        r->upperUIScale = r->upperUIScale + 4;
    }
}

void autoReservation(Restaurant* r) {
    int personCount;
    char timeStr[TIME_LEN];
    printf("Pocet osob: ");
    scanf("%d", &personCount);
    printf("Cas prichodu: ");
    scanf("%9s", timeStr);

    Table* bestTable = NULL;    //Ukazatel na nejlepší stůl, NULL = zatím žádný nemáme
    int minWaste = 999;         // Hledáme stůl, kde propadne nejméně míst

    for (Table* t = r->tables; t < r->tables + r->currentTableCount; t++) {  //Proiteruje všechny stoly, hledá volný stůl, který by byl zaplněný největší částí 
        if (!t->isReserved && t->capacity >= personCount) {
            int waste = t->capacity - personCount;
            if (waste < minWaste) {
                minWaste = waste;
                bestTable = t;
            }
        }
    }

    if (bestTable != NULL) {
        confirmReservation(r, bestTable, timeStr);
        r->upperUIScale = 5;
    }
    else {
        printf("\nPro %d osob nemame zadny volny stul.\n", personCount);
        r->upperUIScale = 4;
    }
}

void modifyReservation(Restaurant* r) {     //Funkce pro úpravu času nebo mazání rezervace
    int code, choice;

	printTables(r);

    printf("Zadejte rezervacni kod: ");
    scanf("%d", &code);

    Table* found = NULL;
    for (int i = 0; i < r->currentTableCount; i++) {
        if (r->tables[i].isReserved && r->tables[i].reservationNumber == code) {   //Pro jistotu kontrola i zda je stůl zarezervovaný
            found = &r->tables[i];
            break;
        }
    }

    if (found == NULL) {
        printf("Rezervace s kodem %d nenalezena\n", code);
        r->upperUIScale += 2;
        return;
    }

    printf("Rezervace nalezena (Stul %d, Cas: %s).\n", found->id, found->time);
    printf("1. Zrusit rezervaci\n2. Zmenit cas\nVolba: ");
    scanf("%d", &choice);

    if (choice == 1) {
        found->isReserved = 0;
        found->reservationNumber = 0;
        strcpy(found->time, "-");

        system("cls");                                  //Překreslení celé obrazovky
        printTables(r);                                 //Vykreslení stolů
        printf("Rezervace %d byla zrusena\n", code);    //Potvrzení
        r->upperUIScale++;
    }
    else if (choice == 2) {
        printf("Novy cas: ");
        scanf("%9s", found->time);

        system("cls");
        printTables(r);
        printf("Cas rezervace %d byl zmenen\n", code);
        r->upperUIScale++;
    }
    else {
        printf("Neplatna volba\n");
        r->upperUIScale = 6;
    }
    saveTables(r);
}

int generateReservationCode(Restaurant* r) {        //Funkce pro generování rezervačního kódu
    int codeCandidate = rand() % 9000 + 1000;
    for (int i = 0; i < r->currentTableCount; i++) {
        if (codeCandidate == r->tables[i].reservationNumber) {
            return generateReservationCode(r);      //Rekurzivně volej funkci, pokud by náhodný kód měl být duplicitní
        }
    }
    return codeCandidate;
}