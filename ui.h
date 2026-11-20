#ifndef UI_H
#define UI_H

#include "tables.h"

void changeDay(Restaurant* r);      // Změna dne a načtení nového souboru - volá se vždy na začátku a po volbě
void printMenu(Restaurant* r);      // Vykreslení hlavního menu - zarovnává se upperUIScale, aby neběhalo nahoru a dolu

#endif
