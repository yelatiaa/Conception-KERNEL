#include <stdint.h>
#include <stdio.h>
#include "cpu.h"
#include <string.h>
#include "ecran.h"

#define VIDEO_MEMORY_BASE 0XB8000 //adresse de la memoire video


uint8_t lig = 0;
uint8_t col = 0;



uint16_t *ptr_mem(uint32_t lig, uint32_t col){
    /*
    Renvoie un poointeur sur la case memoire crsp
    aux donnée fournie
    */
   uint16_t offset=2*(lig*NUM_COLS+col);
   return (uint16_t *)(VIDEO_MEMORY_BASE+offset);
   /*on le traite comme un pointeur sur une zone 
    mémoire qui contient des valeurs de type
    uint16 car chaque case memoire dans la memoire
    video contient deux octets 1 pour ASCII et 1 pour
    les attributs 
    */
}


// Fonction ecrit_car qui utilise ptr_mem pour écrire un caractère à l'écran
void ecrit_car(uint32_t lig, uint32_t col, char c, uint8_t couleur_texte, uint8_t couleur_fond, uint8_t clignotement) {
    // Récupérer l'adresse mémoire de la position (lig, col) à l'aide de ptr_mem
    uint16_t *adresse = ptr_mem(lig, col);

    // Créer l'octet d'attributs pour la couleur du texte, du fond et le clignotement
    uint8_t attributs = (clignotement << 7) | (couleur_fond << 4) | (couleur_texte);

    // Combiner le caractère et les attributs dans une seule valeur de 16 bits
    *adresse = (attributs << 8) | (uint8_t)c;
}


void efface_ecran(void){

    for(int i=0;i<NUM_ROWS;i++){
        for(int k=0;k<NUM_COLS;k++){            
            ecrit_car(i, k, ' ', 15, 0, 0); 

        }
    }
    place_curseur(0,0);
}



void place_curseur(uint32_t lign, uint32_t coln) {
    // Calculer la position du curseur
    uint16_t pos_curseur = coln + lign * NUM_COLS;

    // Extraire la partie basse et la partie haute
    uint8_t adresse_basse = pos_curseur & 0xFF;
    uint8_t adresse_haute = (pos_curseur >> 8) & 0xFF;

    // Envoyer la partie basse de la position du curseur
    outb(0x0F, 0x3D4);
    outb(adresse_basse, 0x3D5);

    // Envoyer la partie haute de la position du curseur
    outb(0x0E, 0x3D4);  // Corrigé ici
    outb(adresse_haute, 0x3D5);

    // Mettre à jour les variables globales `lig` et `col`
    lig = lign;
    col = coln;  

}


void traite_car(char c){

    switch (c){

        case '\b':
            if (col>0){
            place_curseur(lig,col-1);
            }
            break;            

        case '\t':

        if (col>71){
            int reste=(col+8)%NUM_COLS;
            place_curseur(lig+1,reste);
           
            }else{  
                ecrit_car(0, 0, 'D', 5, 2, 0);              
                place_curseur(lig,col+8);
            }
            break;

        case '\n':
            place_curseur(lig+1,0);  
            break;


        case '\f':
            efface_ecran();
            place_curseur(0,0);
            break;
        
        case '\r':
            place_curseur(lig,0);
            break;
        }  

    }






void console_putbytes(const char *s, int len) {
    for (int i = 0; i < len; i++) {
        char c = s[i];

        // Gérer le caractère en appelant `traite_car`
        traite_car(c);

        // Avancer la position en fonction du caractère traité
        if (c != '\n' && c != '\r' && c != '\b') { 
            ecrit_car(lig, col, c, 5, 2, 0); // Écrire le caractère
            col++; // Avancer la colonne
            if (col >= NUM_COLS) { 
                col = 0; 
                lig++;
            }
        }

        // Gérer le défilement si nécessaire
        if (lig >= NUM_ROWS) {
            defilement();
            lig = NUM_ROWS - 1; // Ajuster à la dernière ligne
        }
    }

    // Mettre à jour la position du curseur
    place_curseur(lig, col);
}


void bonjour() {
    efface_ecran(); // Nettoyer l'écran

    for (int i = 0; i < 200; i++) { // Affiche 100 lignes pour tester le défilement
        char buffer[50];
        snprintf(buffer, sizeof(buffer), "Bonjour, je suis %d", i);
        console_putbytes(buffer, strlen(buffer));
        console_putbytes("\n", 1); // Saut de ligne
    }
}

void defilement(void) {
    uint16_t *video_memory = (uint16_t *)VIDEO_MEMORY_BASE;

    // Déplacer le contenu de l'écran d'une ligne vers le haut
    memmove(video_memory, video_memory + NUM_COLS, (NUM_ROWS - 1) * NUM_COLS * 2);

    // Effacer la dernière ligne (remplir avec espaces blancs)
    for (int i = 0; i < NUM_COLS; i++) {
        ecrit_car(lig,col,video_memory[i],0,15,0);
    }
}







void test_tp1(){
    efface_ecran();
    ecrit_car(0, 0, 'A', 5, 2, 0);
    ecrit_car(1, 0, 'B', 5, 2, 0);
    ecrit_car(24, 0, 'F', 5, 2, 0);
    ecrit_car(23, 0, 'Y', 5, 2, 0);
    ecrit_car(23, 78, 'G', 5, 2, 0);
    ecrit_car(79, 0, 'G', 5, 2, 0);
    ecrit_car(40, 40, 'G', 5, 2, 0);
    ecrit_car(78, 0, 'G', 5, 2, 0);
    ecrit_car(13, 48, 's', 5, 2, 0);

    
    printf("fces ioc<b nqaebfiovabqevesqc");
    printf("fces ioc<b nqaebfiovabqevesqc");
    printf("fces ioc<b nqaebfiovabqevesqc");
    printf("fces ioc<b nqaebfiovabqevesqc");

    place_curseur(23,0);
   
    


    // traite_car('');
}

// int main(void){
//     place_curseur(40,40);
//     get_pos_curseur();




//     return 0;
// }
