#include "tinyalloc.h"
#include "timer.h"
#include "sleep.h"
#include "process.h"




Processus *tab[TAILLE_TABLE_PROCESS];
Processus *liste_activable=NULL; //lliste des activables
Processus *liste_queue=NULL;  //liste des anciens activables
Processus *actif=NULL;
Processus *ancien_actif=NULL;


int nb_processus = 0;


void attente(int delay) {
    for (volatile int i = 0; i < delay * 1000000; i++) {
        // Boucle vide pour simuler un délai
    }
}



void (*fonctions_processus[TAILLE_TABLE_PROCESS])(void) = {
    idle, proc1
};




int32_t cree_processus(void (*code)(void), char *nom){
   
    // Allocation de mémoire pour le processus
    Processus *proc = (Processus *)malloc(sizeof(Processus));
    if (proc == NULL) {
        printf("Erreur d'allocation mémoire\n");
        return -1;
    }

    // Initialisation du processus
    proc->pid = nb_processus;
    strcpy(proc->name, nom);

    // Si ce n'est pas idle, configure la pile et le contexte
    if (strcmp("idle", nom) != 0) {
        proc->etat = ACTIVABLE;
        proc->contexte[1] = (uint32_t)&proc->pile[511];
        proc->pile[511] = (uint32_t)code;
    }else{
        proc->etat=ELU;
    }

    
    ajoute_queue(proc);
    nb_processus++;
    return proc->pid;

}


void ajoute_queue(Processus *proc) {
    proc->etat=ACTIVABLE; 
    proc->suiv=NULL; //normalement deja à NULL avec extract

    if (liste_activable == NULL) {
        liste_activable = proc;
        liste_queue=liste_activable;
    } else {
        liste_queue->suiv=proc;
        liste_queue=proc;

    }
    
}


Processus* extract_elu(){
    
    if (liste_activable==NULL){
        return NULL; // aucun process
    }else{
        Processus *elu=liste_activable;
        liste_activable=liste_activable->suiv;

        elu->suiv=NULL;
        elu->etat=ACTIVABLE;
        actif=elu;
        return elu;
    }
     
}

void wake_up_processus() {

    while (liste_sleep != NULL && liste_sleep->wake <= nbr_secondes()) {
        Processus *temp = liste_sleep;  // Pointer sur le premier processus
        temp->etat = ACTIVABLE;        // Marquer comme activable
        ajoute_queue(temp);            // Ajouter à la liste des activables
        supprimer(temp, &liste_sleep); // Supprimer de la liste des dormants
    }
}

void ordonnance() {
    Processus *ancien_actif = actif;  // Sauvegarder le processus actif actuel

    // Réveiller les processus endormis et les remettre dans la liste des activables
    wake_up_processus();

    // Élire le prochain processus de la liste des activables
    actif = extract_elu();

    // Si aucun processus activable n'a été trouvé, repasser à `idle`
    if (actif == NULL) {
        actif = ancien_actif;  // Repasser à l'ancien actif qui devrait être `idle`
    }

    // Mettre à jour l'état de l'ancien actif
    if (ancien_actif != NULL && ancien_actif != actif) {
        if (ancien_actif->etat == ENDORMI) {
            // L'ancien actif est déjà dans la liste des endormis via wake_up_processus
        } else {
            // Remettre l'ancien actif dans la liste activable
            ancien_actif->etat = ACTIVABLE;
            ajoute_queue(ancien_actif);
        }
    }

    // Marquer le nouveau processus comme élu
    if (actif != NULL) {
        actif->etat = ELU;
    }

    // Effectuer le changement de contexte
    if (ancien_actif != NULL && actif != NULL && ancien_actif != actif) {
        ctx_sw(ancien_actif->contexte, actif->contexte);
    }
}




const char* etat_to_string(Etat etat) {
    switch (etat) {
        case ELU:
            return "ELU";
        case ACTIVABLE:
            return "ACTIVABLE";
        default:
            return "INCONNU";
    }
}

void affiche_etat(){
    for (int i=0;i<TAILLE_TABLE_PROCESS;i++){
        printf("%s->",etat_to_string(tab[i]->etat));
    }
}

void affiche_liste(Processus *liste){

    Processus *temp=liste;
    while(temp){
        printf("%s %d\n", temp->name, temp->pid);
        temp=temp->suiv;
    }
}





void idle(void) {
    for (;;) {
        
        sti();  // Active les interruptions
        hlt();  // Met le processeur en veille jusqu'à la prochaine interruption
        cli();  // Désactive les interruptions
    }
}

void proc1(void) {
    for (;;) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(2);
      
    }
}

void proc2(void) {
    for (;;) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(3);
    }
}

void proc3(void) {
    for (;;) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(5);
    }
}








int32_t mon_pid(void) {
    if (actif != NULL) {
        return actif->pid;
    }
    return -1;  // Valeur par défaut si aucun processus n'est actif
}

char* mon_nom(void) {
    if (actif != NULL) {
        return actif->name;
    }
    return "";  // Valeur par défaut si aucun processus n'est actif
}