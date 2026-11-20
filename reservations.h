#ifndef RESERVATIONS_H
#define RESERVATIONS_H

#include "tables.h"

void manualReservation(Restaurant* r);      // Manuální rezervace s kontrolou dostupnosti
void autoReservation(Restaurant* r);        // Automatický výběr stolu podle počtu osob
void modifyReservation(Restaurant* r);      // Zrušení nebo posunutí rezervace
int generateReservationCode(Restaurant* r);             // Generování unikátního rezervačního kódu

#endif
