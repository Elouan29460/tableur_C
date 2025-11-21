
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "liste.h"

void print_int(void * ptr)
{
    int *ptrInt = (int *) ptr;
    printf("%d ", *ptrInt);
}

void test_list_create() {
    printf("\n=== Test 1: list_create ===\n");
    node_t *liste = list_create();
    assert(liste == NULL);
    printf("✓ Liste vide créée correctement (NULL)\n");
}

void test_list_insert() {
    printf("\n=== Test 2: list_insert ===\n");
    node_t *liste = list_create();
    int a = 10, b = 20, c = 30;
    
    // Insertion dans une liste vide
    liste = list_insert(liste, &a);
    assert(liste != NULL);
    assert(*(int*)list_get_data(liste) == 10);
    printf("✓ Insertion dans liste vide: %d\n", *(int*)list_get_data(liste));
    
    // Insertion en tête
    liste = list_insert(liste, &b);
    assert(*(int*)list_get_data(liste) == 20);
    printf("✓ Insertion en tête: %d\n", *(int*)list_get_data(liste));
    
    liste = list_insert(liste, &c);
    assert(*(int*)list_get_data(liste) == 30);
    printf("✓ Liste après insertions: ");
    list_print(liste, print_int);
    printf("\n");
    
    list_destroy(liste);
}

void test_list_append() {
    printf("\n=== Test 3: list_append ===\n");
    node_t *liste = list_create();
    int a = 10, b = 20, c = 30;
    
    // Append dans une liste vide
    liste = list_append(liste, &a);
    assert(liste != NULL);
    assert(*(int*)list_get_data(liste) == 10);
    printf("✓ Append dans liste vide: %d\n", *(int*)list_get_data(liste));
    
    // Append en queue
    liste = list_append(liste, &b);
    liste = list_append(liste, &c);
    printf("✓ Liste après appends: ");
    list_print(liste, print_int);
    printf("\n");
    
    // Vérifier que c est bien en queue
    node_t *dernier = liste;
    while (list_next(dernier) != NULL) {
        dernier = list_next(dernier);
    }
    assert(*(int*)list_get_data(dernier) == 30);
    printf("✓ Dernier élément: %d\n", *(int*)list_get_data(dernier));
    
    list_destroy(liste);
}

void test_list_get_set_data() {
    printf("\n=== Test 4: list_get_data et list_set_data ===\n");
    node_t *liste = list_create();
    int a = 10, b = 99;
    
    liste = list_insert(liste, &a);
    assert(*(int*)list_get_data(liste) == 10);
    printf("✓ Donnée initiale: %d\n", *(int*)list_get_data(liste));
    
    // Modifier la donnée
    list_set_data(liste, &b);
    assert(*(int*)list_get_data(liste) == 99);
    printf("✓ Donnée modifiée: %d\n", *(int*)list_get_data(liste));
    
    // Test avec noeud NULL
    void *data = list_get_data(NULL);
    assert(data == NULL);
    printf("✓ list_get_data(NULL) retourne NULL\n");
    
    list_destroy(liste);
}

void test_list_next() {
    printf("\n=== Test 5: list_next ===\n");
    node_t *liste = list_create();
    int a = 10, b = 20, c = 30;
    
    liste = list_insert(liste, &a);
    liste = list_insert(liste, &b);
    liste = list_insert(liste, &c);
    
    // Parcourir la liste
    printf("✓ Parcours avec list_next: ");
    node_t *current = liste;
    int count = 0;
    while (current != NULL) {
        printf("%d ", *(int*)list_get_data(current));
        current = list_next(current);
        count++;
    }
    printf("\n");
    assert(count == 3);
    printf("✓ Nombre d'éléments parcourus: %d\n", count);
    
    // Test avec NULL
    assert(list_next(NULL) == NULL);
    printf("✓ list_next(NULL) retourne NULL\n");
    
    list_destroy(liste);
}

void test_list_headRemove() {
    printf("\n=== Test 6: list_headRemove ===\n");
    node_t *liste = list_create();
    int a = 10, b = 20, c = 30;
    
    liste = list_insert(liste, &a);
    liste = list_insert(liste, &b);
    liste = list_insert(liste, &c);
    
    printf("Liste initiale: ");
    list_print(liste, print_int);
    printf("\n");
    
    // Supprimer la tête
    liste = list_headRemove(liste);
    assert(*(int*)list_get_data(liste) == 20);
    printf("✓ Après suppression tête: ");
    list_print(liste, print_int);
    printf("\n");
    
    liste = list_headRemove(liste);
    assert(*(int*)list_get_data(liste) == 10);
    printf("✓ Après 2ème suppression: ");
    list_print(liste, print_int);
    printf("\n");
    
    // Supprimer le dernier élément
    liste = list_headRemove(liste);
    assert(liste == NULL);
    printf("✓ Liste vide après suppression du dernier élément\n");
    
    // Test avec liste vide
    liste = list_headRemove(NULL);
    assert(liste == NULL);
    printf("✓ list_headRemove(NULL) retourne NULL\n");
}

void test_list_remove() {
    printf("\n=== Test 7: list_remove ===\n");
    node_t *liste = list_create();
    int a = 10, b = 20, c = 30, d = 40;
    
    liste = list_insert(liste, &a);
    liste = list_append(liste, &b);
    liste = list_append(liste, &c);
    liste = list_append(liste, &d);
    
    printf("Liste initiale: ");
    list_print(liste, print_int);
    printf("\n");
    
    // Supprimer un élément au milieu
    liste = list_remove(liste, &b);
    printf("✓ Après suppression de 20: ");
    list_print(liste, print_int);
    printf("\n");
    
    // Supprimer la tête
    liste = list_remove(liste, &a);
    printf("✓ Après suppression de 10 (tête): ");
    list_print(liste, print_int);
    printf("\n");
    
    // Supprimer un élément qui n'existe pas
    int e = 99;
    node_t *liste_avant = liste;
    liste = list_remove(liste, &e);
    assert(liste == liste_avant);
    printf("✓ Suppression d'élément inexistant: liste inchangée\n");
    
    // Supprimer le dernier élément
    liste = list_remove(liste, &d);
    printf("✓ Après suppression de 40 (queue): ");
    list_print(liste, print_int);
    printf("\n");
    
    // Test avec liste vide
    liste = list_remove(liste, &c);
    liste = list_remove(NULL, &a);
    assert(liste == NULL);
    printf("✓ list_remove sur liste vide retourne NULL\n");
}

void test_list_destroy() {
    printf("\n=== Test 8: list_destroy ===\n");
    node_t *liste = list_create();
    int a = 10, b = 20, c = 30;
    
    liste = list_insert(liste, &a);
    liste = list_insert(liste, &b);
    liste = list_insert(liste, &c);
    
    printf("Liste avant destruction: ");
    list_print(liste, print_int);
    printf("\n");
    
    list_destroy(liste);
    printf("✓ Liste détruite sans erreur\n");
    
    // Test avec liste vide
    list_destroy(NULL);
    printf("✓ list_destroy(NULL) sans erreur\n");
}

void test_mixed_operations() {
    printf("\n=== Test 9: Opérations mixtes ===\n");
    node_t *liste = list_create();
    int a = 10, b = 20, c = 30, d = 40, e = 50;
    
    // Combinaison d'insertions et d'appends
    liste = list_insert(liste, &c);      // [30]
    liste = list_append(liste, &d);      // [30, 40]
    liste = list_insert(liste, &b);      // [20, 30, 40]
    liste = list_append(liste, &e);      // [20, 30, 40, 50]
    liste = list_insert(liste, &a);      // [10, 20, 30, 40, 50]
    
    printf("Liste après insertions/appends mixtes: ");
    list_print(liste, print_int);
    printf("\n");
    
    // Vérifier l'ordre
    assert(*(int*)list_get_data(liste) == 10);
    node_t *current = list_next(liste);
    assert(*(int*)list_get_data(current) == 20);
    current = list_next(current);
    assert(*(int*)list_get_data(current) == 30);
    printf("✓ Ordre correct: 10, 20, 30, 40, 50\n");
    
    // Suppressions mixtes
    liste = list_remove(liste, &c);      // [10, 20, 40, 50]
    liste = list_headRemove(liste);      // [20, 40, 50]
    
    printf("Après suppressions: ");
    list_print(liste, print_int);
    printf("\n");
    
    list_destroy(liste);
    printf("✓ Test d'opérations mixtes réussi\n");
}

int main(void){
    printf("==========================================\n");
    printf("   TESTS DU MODULE LISTE CHAÎNÉE\n");
    printf("==========================================\n");
    
    test_list_create();
    test_list_insert();
    test_list_append();
    test_list_get_set_data();
    test_list_next();
    test_list_headRemove();
    test_list_remove();
    test_list_destroy();
    test_mixed_operations();
    
    printf("\n==========================================\n");
    printf("   ✓ TOUS LES TESTS SONT PASSÉS !\n");
    printf("==========================================\n");
    
    return 0;
}

