#ifndef PROCESS_H
#define PROCESS_H


#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include "segment.h"
#include "cpu.h"
#include "start.h"
#include <unistd.h>


#define TAILLE_TABLE_PROCESS 8




extern void ctx_sw(uint32_t*,uint32_t*);
extern void (*fonctions_processus[TAILLE_TABLE_PROCESS])(void);

typedef enum {
    ELU,
    ACTIVABLE,
    ENDORMI
} Etat;

const char* etat_to_string(Etat etat);
void affiche_etat();

typedef struct Processus{
    int32_t pid;
    char name[9];
    Etat etat; 
    uint32_t contexte[5]; 
    uint32_t pile[512];
    uint32_t wake;

    struct Processus *suiv; //pointeur vers le suivant 

}Processus;


extern Processus *tab[TAILLE_TABLE_PROCESS]; // Table des processus
extern Processus *liste_activable; //Pointeur sur les processus activables 
extern Processus *liste_queue;



extern Processus *actif;
void init_table_process();
int32_t mon_pid(void);
char *mon_nom(void);


void idle(void);
void proc1(void);
void proc2(void);
void proc3(void);
void proc4(void);
void proc5(void);
void proc6(void);
void proc7(void);
void proc8(void);

int32_t cree_processus(void (*code)(void),char *nom);
void ajoute_queue(Processus *proc);
void affiche_liste(Processus *liste);
Processus* extract_elu();
void ordonnance();
void wake_up_processus();









#endif // PROCESS_H