#ifndef TIMER_H
#define TIMER_H


#define QUARTZ 0x1234DD
#define IDT_SIZE 256
#include <stdbool.h>
#include "segment.h"
#include "cpu.h"
#include <inttypes.h>
#include <unistd.h>




extern void traitant_IT_32(void) ;

struct entree_idt {
    uint32_t mot1;
    uint32_t mot2;
};

void ecrit_temps(char* date,int taille);
void tic_PIT(void);
void init_traitant_IT();
void test_tp2();
void masque_IRQ(uint32_t num_IRQ, bool masque);
void freq_horloge(void);
uint32_t nbr_secondes();



#endif // TIMER_H