# Projet de Conception d'un Kernel Simplifié

Ce projet consiste à concevoir un kernel simplifié avec gestion de l'ordonnancement pour `n` processus, affichage de l'écran, et gestion du timer.

---

## **Objectifs Principaux**

1. **Gestion de l'ordonnancement :** 
   - Implémentation d'un ordonnanceur basé sur l'algorithme du tourniquet (round-robin).
   - Gestion de la transition de contexte entre plusieurs processus.
   - Création d'une structure pour chaque processus (PID, état, pile, registres, etc.).

2. **Affichage de l'écran :**
   - Gestion de l'affichage en mode texte (CGA).
   - Manipulation de la mémoire vidéo pour afficher du texte.
   - Contrôle du curseur et des couleurs de texte et de fond.

3. **Gestion du timer :**
   - Utilisation d'interruptions pour générer des ticks horloge.
   - Synchronisation des tâches avec le timer pour l'ordonnancement.

---

## **Structure du Projet**

### **Dossiers et Fichiers Principaux :**
  - `kernel.c` : Point d'entrée du kernel.
  - `scheduler.c` : Implémentation de l'ordonnancement.
  - `screen.c` : Gestion de l'écran.
  - `timer.c` : Gestion des interruptions du timer.
  - `kernel.h` : Définitions globales pour le kernel.
  - `scheduler.h` : Prototypes et structures liés à l'ordonnancement.
  - `screen.h` : Prototypes pour la gestion de l'écran.
  - `timer.h` : Déclarations liées au timer.
- `Makefile` : Fichier pour compiler le projet.
- `README.md` : Documentation du projet.

---

## **Fonctionnalités**

### **1. Ordonnancement**
- Implémentation d'une file de processus actifs.
- Fonction `ordonnance()` pour basculer entre les processus.
- Changement de contexte via la fonction `ctx_sw`.

### **2. Affichage**
- Contrôle direct de la mémoire vidéo (adresse 0xB8000).
- Affichage de texte et gestion du curseur avec des ports E/S.
- Prise en charge des couleurs pour le texte (8 bits pour texte et fond).

### **3. Timer**
- Initialisation du timer via le PIT (Programmable Interval Timer).
- Gestion des interruptions via une routine ISR.

---

## **Comment Compiler et Exécuter**

1. Cloner le projet :
   ```bash
   https://github.com/yelatiaa/Conception-KERNEL
   cd Conception-KERNEL
