#include <cpu.h>
#include <inttypes.h>
#include "ecran.h"
#include "timer.h"
#include "process.h"
#include "stdio.h"
#include "sleep.h"
#include "tinyalloc.h"
// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display

// une fonction bien connue
// uint32_t fact(uint32_t n)
// {
//     uint32_t res;
//     if (n <= 1) {
//         res = 1;
//     } else {
//         res = fact(n - 1) * n;
//     }
//     return res;
// }




void kernel_start(void)
{


efface_ecran();
freq_horloge();
masque_IRQ(0,false);
init_traitant_IT(32,traitant_IT_32);




cree_processus(idle, "idle"); 
cree_processus(proc1,"proc1");
cree_processus(proc2,"proc2");
cree_processus(proc3,"proc3");



actif=liste_activable; //LE proc actif est le premmier processus de la liste activabe !


idle();




   
}

