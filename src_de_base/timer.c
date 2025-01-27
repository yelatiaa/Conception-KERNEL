//realise le travail concret du traitant
#include <stdio.h>
#include "ecran.h"
#include "cpu.h"
#include "timer.h"
#include "process.h"

#define IDT_BASE_ADDRESS 0x1000 
#define QUARTZ 0x1234DD
#define CLOCKFREQ 50



uint32_t secondes=0;

void ecrit_temps(char* date,int taille){
    uint8_t templig=lig;
    uint8_t tempcol=col;



    lig=0;
    col=72;
    console_putbytes(date,taille);

    //on remet le curseur au bon endroiit
    
    lig=templig;
    col=tempcol;


}

void tic_PIT(void){
    //gere les interruptions générées par le Programmable Interval Timer 

    outb(0x20,0x20);
    secondes++;
    

    uint32_t heures=secondes/3600;
    uint32_t minutes=(secondes % 3600) / 60;
    uint32_t secs=secondes%60;

    char buffer[9];
    sprintf(buffer, "%02u:%02u:%02u", heures, minutes, secs);
    ecrit_temps(buffer,8);
    // on appelle l'ordonnance pour changer de processus à chaque interuptuions
    ordonnance();
}


struct entree_idt idt[256];

void init_traitant_IT(uint32_t num_IT, void (*traitant)(void)) {
    uint32_t adresse_traitant = (uint32_t) traitant;

    // Construire les mots de l'entrée IDT
    uint32_t mot1 = (KERNEL_CS << 16) | (adresse_traitant & 0xFFFF);  // 16 bits de KERNEL_CS et 16 bits de l'adresse basse
    uint32_t mot2 = (((adresse_traitant >> 16) & 0xFFFF)<<16)|0x8E00;  // 16 bits de l'adresse haute et 0x8E00 pour le type d'interruption

    // Placer les mots dans l'entrée IDT correspondante
    idt[num_IT].mot1 = mot1;
    idt[num_IT].mot2 = mot2;
}



void freq_horloge(void){
    //on annnce quon ca envoyer la vlauer de reglage de la frequence
    outb(0x34,0x43);
    outb((QUARTZ/CLOCKFREQ)& 0xFF,0x40);
    outb((QUARTZ/CLOCKFREQ)>>8,0x40);

}


void masque_IRQ(uint32_t num_IRQ, bool masque){
    uint8_t tab_masque=inb(0x21);
    
    if (masque){
        tab_masque|=(1<<num_IRQ);
    }else{
        tab_masque &= ~(1<<num_IRQ);
    }

    outb(tab_masque,0x21);

}







void test_tp2(){
    
    efface_ecran();
    freq_horloge();

    masque_IRQ(0,false);
    init_traitant_IT(32,traitant_IT_32);

     
    sti();
    
    
}


uint32_t nbr_secondes(){
    return secondes;
}
