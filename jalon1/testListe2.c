
#include <stdio.h>
#include <stdlib.h>
#include "liste.h"

void print_int(void * ptr)
{
    int *ptrInt = (int *) ptr;
    printf("%d ", *ptrInt);
}

int main(void){
    node_t *liste = list_create();
    int a = 10;
    int b = 20;
    int c = 30;
    int d = 60;

    int e = 45;

    liste = list_insert(liste, &a);
    liste = list_append(liste, &b);
    liste = list_append(liste, &d);
    liste = list_insert(liste, &c);

    printf("Liste complète : ");
    list_print(liste, print_int);
    printf("Complet\n");
    
    liste = list_remove(liste, &b);
    liste = list_remove(liste, &e);

    printf("Après suppression : ");
    list_print(liste, print_int);
    printf("Remove\n");

    liste = list_headRemove(liste);
    printf("Après suppression de la tête :");
    list_print(liste, print_int);
    printf("Head\n");

    list_destroy(liste);
    //list_print(liste, print_int);
    return 0;
}
