
#include <stdio.h>
#include <stdlib.h>
#include "liste.h"

void print_int(void * ptr)
{
    int *ptrInt = (int *) ptr;
    printf("%d ", *ptrInt);
}

int main(void){
    printf("=== Test du module Liste ===\n\n");
    
    // Test 1: Création d'une liste vide
    printf("Test 1: Creation liste vide\n");
    node_t *liste = list_create();
    printf("Liste creee: %s\n\n", liste == NULL ? "OK (NULL)" : "Erreur");
    
    // Test 2: Insertions et appends
    printf("Test 2: Insertions et appends\n");
    int a = 10;
    int b = 20;
    int c = 30;
    int d = 60;
    int e = 45;
    
    liste = list_insert(liste, &a);
    printf("Apres insert(10): ");
    list_print(liste, print_int);
    printf("\n");
    
    liste = list_append(liste, &b);
    printf("Apres append(20): ");
    list_print(liste, print_int);
    printf("\n");
    
    liste = list_append(liste, &d);
    printf("Apres append(60): ");
    list_print(liste, print_int);
    printf("\n");
    
    liste = list_insert(liste, &c);
    printf("Apres insert(30): ");
    list_print(liste, print_int);
    printf("\n\n");
    
    // Test 3: Lecture et modification de données
    printf("Test 3: Lecture et modification de donnees\n");
    void *data = list_get_data(liste);
    printf("Tete de liste: %d\n", *(int*)data);
    
    list_set_data(liste, &e);
    printf("Apres modification tete: ");
    list_print(liste, print_int);
    printf("\n");
    
    list_set_data(liste, &c);  // Remettre la valeur d'origine
    printf("Restauration: ");
    list_print(liste, print_int);
    printf("\n\n");
    
    // Test 4: Navigation avec list_next
    printf("Test 4: Parcours avec list_next\n");
    node_t *current = liste;
    int count = 0;
    printf("Elements: ");
    while (current != NULL) {
        printf("%d ", *(int*)list_get_data(current));
        current = list_next(current);
        count++;
    }
    printf("\nNombre d'elements: %d\n\n", count);
    
    // Test 5: Suppression d'un élément spécifique
    printf("Test 5: Suppressions d'elements\n");
    printf("Liste avant: ");
    list_print(liste, print_int);
    printf("\n");
    
    liste = list_remove(liste, &b);
    printf("Apres remove(20): ");
    list_print(liste, print_int);
    printf("\n");
    
    liste = list_remove(liste, &e);
    printf("Apres remove(45) [inexistant]: ");
    list_print(liste, print_int);
    printf("\n\n");
    
    // Test 6: Suppression de la tête
    printf("Test 6: Suppression de la tete\n");
    printf("Liste avant: ");
    list_print(liste, print_int);
    printf("\n");
    
    liste = list_headRemove(liste);
    printf("Apres headRemove: ");
    list_print(liste, print_int);
    printf("\n\n");
    
    // Test 7: Ajout dans une liste partiellement vidée
    printf("Test 7: Ajout apres suppressions\n");
    liste = list_append(liste, &e);
    printf("Apres append(45): ");
    list_print(liste, print_int);
    printf("\n\n");
    
    // Test 8: Vider complètement la liste
    printf("Test 8: Vider completement la liste\n");
    while (liste != NULL) {
        printf("Suppression tete: %d\n", *(int*)list_get_data(liste));
        liste = list_headRemove(liste);
    }
    printf("Liste finale: %s\n\n", liste == NULL ? "vide (NULL)" : "non vide");
    
    // Test 9: Réutilisation après vidage
    printf("Test 9: Reutilisation apres vidage\n");
    liste = list_insert(liste, &a);
    liste = list_append(liste, &b);
    liste = list_append(liste, &c);
    printf("Nouvelle liste: ");
    list_print(liste, print_int);
    printf("\n\n");
    
    // Destruction finale
    printf("Test 10: Destruction finale\n");
    list_destroy(liste);
    printf("Liste detruite\n\n");
    
    printf("=== Tous les tests termines ===\n");
    return 0;
}
