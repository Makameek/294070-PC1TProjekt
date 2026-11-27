#ifndef RESERVATIONS_H
#define RESERVATIONS_H

#include "tables.h"

void manualReservation(Restaurant* r);      // Manuální rezervace s kontrolou dostupnosti
void autoReservation(Restaurant* r);        // Automatický výběr stolu podle počtu osob
void modifyReservation(Restaurant* r);      // Zrušení nebo posunutí rezervace
int generateReservationCode(Restaurant* r);             // Generování unikátního rezervačního kódu
int largestFreeTable(const Restaurant* r);	//Funkce pro zjisteni mist u nejvetsiho volneho stolu - check proti nekonecnym loopum
int readReservationInput(Restaurant* r, int* personCount, char* timeStr); //Fn

#endif
