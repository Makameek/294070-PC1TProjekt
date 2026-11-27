/*
    Modul s logikou rezervací (manuální vytvoření, automatické vytvoření , zrušení/posun)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tables.h"
#include "reservations.h"

static void confirmReservation(Restaurant* r, Table* t, const char* timeStr) {   //zapíše rezervaci ke stolu
    t->isReserved = 1;
    t->reservationNumber = generateReservationCode(r);
    strcpy(t->time, timeStr);

    printf("\nRezervovan stul c. %d\n", t->id);
    printf("Vas rezervacni kod je: %d\n", t->reservationNumber);
    saveTables(r);
}

int readReservationInput(Restaurant* r, int* personCount, char* timeStr) {   //načte počet osob a čas příchodu; vrací 0, pokud není žádný volný stůl
    int c;
    const char* num = "0123456789";         //Stock ciselne pole na kontrolu syntaxe 

    if (largestFreeTable(r) == 0) {            //Kontrola, zda je vůbec volný stůl
        printf("Vsechny stoly jsou obsazeny, nelze provest rezervaci.\n");
        r->upperUIScale += 1;
        return 0;
    }

    while (1) {
        while ((c = getchar()) != '\n' && c != EOF);                    //Vyprazdnit zasobnik - jinak nekonecny whileloop
        printf("Pocet osob: ");
        scanf("%d", personCount);
        if (*personCount < 0) {
            system("cls");
            r->upperUIScale = 1;
            printf("Neplatny pocet osob, zkuste to znovu.\n");
            continue;
        }

        if (*personCount > largestFreeTable(r)) {               //Check proti zacykleni, jestli je vubec misto pro tolik osob
            system("cls");
            r->upperUIScale = 1;
            printf("Neni k dispozici stul pro %d osob, zkuste to znovu.\n", *personCount);
            continue;
        }

        printf("Cas prichodu (HH:MM): ");
        scanf("%5s", timeStr);
        if (!(strspn(timeStr, num) == 2 && timeStr[2] == ':' &&           //kontrola syntaxe vstupu; když neprojde, opakuj
            strspn(timeStr + 3, num) == 2)) {
            system("cls");
            r->upperUIScale = 1;
            printf("Neplatny cas prichodu, zkuste to znovu.\n");
            continue;
        }
        return 1;
    }
}

void manualReservation(Restaurant* r) {     //Funkce pro manuální zadání rezervace s kontrolou dostupnosti
    int personCount;
    char timeStr[TIME_LEN];
    int chosenTableId;

    if (!readReservationInput(r, &personCount, timeStr)) {
        return;
    }

    system("cls");
    r->upperUIScale = 0;

    while (1) {
        printf("Zvoleny pocet osob: %d, Zvoleny cas prichodu: %s\n", personCount, timeStr);
        printTables(r);
        printf("Rezervovat stul cislo: ");
        scanf("%d", &chosenTableId);
        chosenTableId--;    //Odečíst 1 kvůli prvnímu indexu 0 v poli

        if (chosenTableId < 0 || chosenTableId >= r->currentTableCount) {   //Kontrola, že stůl existuje
            system("cls");
            r->upperUIScale = 1;
            printf("Takovy stul neexistuje\n");
        }
        else {
            Table* t = &r->tables[chosenTableId];                   //Ukazatel na vybraný stůl - ten kontrolujeme, popř. zapisujeme rezervaci

            if (!t->isReserved && t->capacity >= personCount) {   //Kontrola obsazenosti a velikosti stolu
                confirmReservation(r, t, timeStr);
                r->upperUIScale = r->upperUIScale + 5;
                break;
            }
            else if (t->isReserved) {
                system("cls");
                r->upperUIScale = 1;
                printf("Stul c. %d je jiz zarezervovany\n", t->id);
            }
            else {
                system("cls");
                r->upperUIScale = 1;
                printf("Stul c. %d ma malo mist pro %d osob\n", t->id, personCount);
            }
        }
    }
    return;
}

void autoReservation(Restaurant* r) {
    int personCount;
    char timeStr[TIME_LEN];

    if (!readReservationInput(r, &personCount, timeStr)) {
        return;
    }

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

    confirmReservation(r, bestTable, timeStr);
    r->upperUIScale = 5;
    return;
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
        r->upperUIScale = 0;
        printTables(r);                                 //Vykreslení stolů
        printf("Rezervace %d byla zrusena\n", code);    //Potvrzení
        r->upperUIScale += 1;
    }
    else if (choice == 2) {
        printf("Novy cas: ");
        scanf("%9s", found->time);

        system("cls");
        r->upperUIScale = 0;
        printTables(r);
        printf("Cas rezervace %d byl zmenen", code);
        r->upperUIScale += 1;
    }
    else {
        printf("Neplatna volba");
        r->upperUIScale += 5;
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

int largestFreeTable(const Restaurant* r) {
    int max = 0;

    for (int i = 0; i < r->tableCapacity; i++) {
        if (!r->tables[i].isReserved && r->tables[i].capacity > max) {
            max = r->tables[i].capacity;
        }
    }
    return max;
}