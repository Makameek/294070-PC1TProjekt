#ifndef TABLES_H
#define TABLES_H

#define INITIAL_TABLES 10   // Počáteční kapacita dynamického pole stolů (pole se podle potřeby zvětšuje) - dáno šablonou, která má 10 stolů (...)
#define TIME_LEN 10         // Defaultní velikost proměnných pro časové údaje
#define FILE_LEN 50         // Délka názvu souboru
#define DAY_LEN 20          // Délka názvu dne

typedef struct {            // Struktura popisující jeden stůl
    int id;
    int capacity;
    int isReserved;
    int reservationNumber;
    char time[TIME_LEN];
} Table;

typedef struct {                // Struktura popisující restauraci v aktuálně zvoleném dni (nahrazuje globální proměnné)
    Table* tables;              // Dynamické pole stolů
    int currentTableCount;      // Počet stolů v restauraci, tj. načtených z .txt
    int tableCapacity;          // Kolik stolů se do alokovaného pole vejde
    char currentFile[FILE_LEN]; // String pro název .txt, který se načítá podle dne
    char currentDay[DAY_LEN];   
    int upperUIScale;           // Odsazení hlavního menu
} Restaurant;

int initRestaurant(Restaurant* r);                      // Alokuje pole stolů, vrací 1 při úspěchu
void freeRestaurant(Restaurant* r);                     // Uvolní pole stolů

void createFileFromTemplate(char* currentFile);         // Vytvoření souboru dne ze šablony
void loadTables(Restaurant* r, char* date);             // Načtení stolů z .txt
void saveTables(Restaurant* r);                         // Uložení stolů do .txt
void printTables(Restaurant* r);                        // Výpis stolů

#endif
