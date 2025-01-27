#ifndef ECRAN_H
#define ECRAN_H

#include <stdint.h>

extern uint8_t lig;
extern uint8_t col;
#define NUM_COLS 80
#define NUM_ROWS 25



extern uint16_t *ptr_mem(uint32_t lig, uint32_t col); // Déclaration de la fonction

void test_tp1();  // Déclare la fonction de test

void ecrit_car(uint32_t lig, uint32_t col, char c, uint8_t couleur_texte, uint8_t couleur_fond, uint8_t clignotement);

void efface_ecran(void);
void place_curseur(uint32_t lign, uint32_t coln);
void traite_car(char c);
void defilement(void);
void console_putbytes(const char *s,int len);
void bonjour();

#endif // ECRAN_H
