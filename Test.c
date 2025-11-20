#include <stdio.h>
#include <stdlib.h>
#include "liste.h"

/* Fonction d'affichage d'une valeur int */
void print_int(void *ptr)
{
    if (!ptr) printf("NULL ");
    else printf("%d ", *(int*)ptr);
}

/* Fonction d'affichage d'un nœud */
void print_node(node_t *n)
{
    if (!n) printf("NULL\n");
    else if (!n->val) printf("NULL(DATA)\n");
    else printf("%d\n", *(int*)n->val);
}

int main(void)
{
    printf("Début test : \n");
    int a=10, b=20, c=30, d=40, x=999;

    /* Création */
    node_t *list = list_create();
    printf("Test list_create: ");
    print_node(list); // attendu NULL

    /* list_get_data et next avec NULL */
    printf("list_get_data(NULL) -> %p (attendu NULL)\n", list_get_data(NULL));
    printf("list_next(NULL) -> %p (attendu NULL)\n", (void*)list_next(NULL));

    /* insert sur liste vide */
    list = list_insert(list, &a);
    printf("Après insert(a), tête = ");
    print_node(list); // attendu 10

    /* insert sur liste non vide */
    list = list_insert(list, &b);
    printf("Après insert(b), tête = ");
    print_node(list); // attendu 20

    /* append sur liste non vide */
    list = list_append(list, &c);
    list = list_append(list, &d);
    printf("Après append(c,d), liste (attendu 20 10 30 40) : ");
    list_print(list, print_int);
    printf("\n");

    /* append sur liste NULL */
    node_t *list2 = NULL;
    list2 = list_append(list2, &a);
    printf("append sur NULL -> ");
    print_node(list2); // attendu 10
    list_destroy(list2);

    /* remove au milieu */
    list = list_remove(list, &c);
    printf("remove(c), liste (attendu 20 10 40) : ");
    list_print(list, print_int);
    printf("\n");

    /* remove tête */
    list = list_remove(list, &b);
    printf("remove(b), liste (attendu 10 40) : ");
    list_print(list, print_int);
    printf("\n");

    /* remove dernier */
    list = list_remove(list, &d);
    printf("remove(d), liste (attendu 10) : ");
    list_print(list, print_int);
    printf("\n");

    /* remove élément absent */
    node_t *before = list;
    list = list_remove(list, &x);
    printf("remove(x absent), liste inchangée (attendu 10) : ");
    list_print(list, print_int);
    printf("\n");

    /* remove sur liste de 1 élément */
    list = list_remove(list, &a);
    printf("remove(a), liste devenue NULL (attendu NULL) : ");
    print_node(list);

    /* remove sur liste vide */
    list = list_remove(list, &a);
    printf("remove(a) sur liste vide -> ");
    print_node(list); // attendu NULL

    /* headRemove sur liste NULL */
    printf("headRemove(NULL) -> ");
    print_node(list_headRemove(NULL)); // attendu NULL

    /* headRemove sur liste d’un élément */
    list = list_insert(list, &a);
    list = list_headRemove(list);
    printf("headRemove sur liste [10] -> ");
    print_node(list); // attendu NULL

    /* insertion d’un élément dont la DATA = NULL */
    list = list_insert(list, NULL);
    printf("insert(NULL) -> tête = ");
    print_node(list); // attendu NULL(DATA)

    /* Destruction finale */
    list_destroy(list);

    printf("Fin test \n");

    return 0;
}
