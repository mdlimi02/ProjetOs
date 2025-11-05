#define _XOPEN_SOURCE 700  // Pour utiliser ucontext
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#define STACK_SIZE 1024 * 1024  // 1 Mo pour la pile

ucontext_t main_context, new_context;
void *stack;

// Fonction exécutée dans le contexte
void fonction() {
    printf("Fonction exécutée sur la nouvelle pile !\n");

    // Libérer la pile avant de quitter
    free(stack);
    printf("Pile libérée.\n");

    // Terminer proprement
    exit(0);
}

int main() {
    // Allouer une pile dynamique
    stack = malloc(STACK_SIZE);
    if (!stack) {
        perror("Échec d'allocation de la pile");
        return 1;
    }

    printf("Pile allouée.\n");

    // Initialiser le contexte
    getcontext(&new_context);
    new_context.uc_stack.ss_sp = stack;
    new_context.uc_stack.ss_size = STACK_SIZE;
    new_context.uc_link = NULL; // Pas de retour après cette fonction

    // Définir la fonction à exécuter dans ce contexte
    makecontext(&new_context, fonction, 0);

    // Changer de contexte
    swapcontext(&main_context, &new_context);

    // (Ne revient jamais ici, car `exit(0)` est appelé dans `fonction`)
    return 0;
}
