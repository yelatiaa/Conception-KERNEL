#include "process.h"
#include "tinyalloc.h"
#include "sleep.h"
#include "timer.h"


Processus *liste_sleep=NULL;


void dors(uint32_t nbr_sec){
    
    if(actif->pid==0){
        return;
    }
    

    actif->etat=ENDORMI;
    actif->wake=nbr_secondes()+nbr_sec; //heure à laquelle il dois se reveiller
    ajoute_sleep(actif); //ajout a la liste des dormeurs 

    sti();  // Active les interruptions
    hlt();  // Met le processeur en veille jusqu'à la prochaine interruption
    cli();

    ordonnance();
    
    
    
    
    


}






void ajoute_sleep(Processus *proc) {
    // Vérification si le pointeur est nul
    if (proc == NULL || proc->pid == 0) {
        return; // Ne pas ajouter `idle` à la liste des dormants
    }

    // Cas où la liste est vide
    if (liste_sleep == NULL) {
        proc->suiv = NULL; // La liste est vide, donc proc devient la tête.
        liste_sleep = proc;
        return;
    }

    // Cas où l'insertion est en tête
    if (liste_sleep->wake > proc->wake) {
        proc->suiv = liste_sleep; // Insérer proc avant liste_sleep.
        liste_sleep = proc;
        return;
    }

    // Trouver la bonne place pour l'insertion triée
    Processus *prec = liste_sleep;
    Processus *curr = liste_sleep->suiv;

    while (curr != NULL && curr->wake <= proc->wake) {
        prec = curr;
        curr = curr->suiv;
    }

    // Insérer proc entre prec et curr
    prec->suiv = proc;
    proc->suiv = curr;
}





void supprimer(Processus *proc,Processus **liste_proc) {

   
    if (*liste_proc == NULL) {
        // Si la liste est vide, rien à faire
        return;
    }

    if(*liste_proc==proc){
        (*liste_proc)=(*liste_proc)->suiv;
    }

    Processus *prec=*liste_proc;
    Processus *cour=prec;

    while(cour!=NULL){ //le denrier elem peut etre celui a supprimer

        if (cour==proc){
            Processus *temp=cour;
            prec->suiv=cour->suiv;
            free(temp->name);
            free(temp);
        }
        prec=cour;
        cour=cour->suiv;

    }
    
}

   
    
