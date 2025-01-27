#ifndef SLEEP_H
#define SLEEP_H
#include "process.h"
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include "segment.h"
#include "cpu.h"
#include "start.h"
#include <unistd.h>




extern Processus *liste_sleep;


void dors(uint32_t nbr_sec);
void ajoute_sleep(Processus *proc);
void supprimer(Processus *proc,Processus **liste);

#endif //SLEEP_H