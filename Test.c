#include <stdio.h>
#include <stdlib.h>
#include "liste.h"

void print_int(void * ptr)
{
    printf("%d ", *(int*)ptr);
}

// Fonction utilitaire pour lire la valeur d'un noeud ou indiquer NULL
const char* val_or_null(node_t *n)
{
    static char buf[50];
    if (!n) return "NULL";
    sprintf(buf, "%d", *(int*)n->val);
    return buf;
}

int main(void)
{
    printf("===== TEST DE LA LISTE CHAINEE =====\n\n");

    int a=10, b=20, c=30, d=40, x=999;

    /* ----------------------------
       TEST 1 : création liste
    ----------------------------- */
    node_t *list = list_create();
    if (list != NULL)
        printf("ERREUR: Liste après list_create: %s (attendu NULL)\n", val_or_null(list));
    else
        printf("OK: list_create -> NULL\n");


    /* ----------------------------
       TEST 2 : insertion en tête
    ----------------------------- */
    list = list_insert(list, &a); // [10]

    if (list == NULL || *(int*)list->val != 10)
        printf("ERREUR: Valeur en tête: %s (attendu 10)\n", val_or_null(list));
    else
        printf("OK: list_insert tête -> %s (attendu 10)\n", val_or_null(list));


    /* ----------------------------
       TEST 3 : append
    ----------------------------- */
    list = list_append(list, &b); // [10 -> 20]
    list = list_append(list, &c); // [10 -> 20 -> 30]

    printf("Liste après append (attendu 10 20 30) : ");
    list_print(list, print_int);
    printf("\n");


    /* ----------------------------
       TEST 4 : insertion tête
    ----------------------------- */
    list = list_insert(list, &d); // [40 -> 10 -> 20 -> 30]

    if (*(int*)list->val != 40)
        printf("ERREUR: Tête après insertion: %s (attendu 40)\n", val_or_null(list));
    else
        printf("OK: list_insert nouvelle tête -> %s\n", val_or_null(list));


    printf("Liste actuelle (attendu 40 10 20 30) : ");
    list_print(list, print_int);
    printf("\n");


    /* ----------------------------
       TEST 5 : remove milieu
    ----------------------------- */
    list = list_remove(list, &b); // retire 20

    printf("Après remove(20) (attendu 40 10 30) : ");
    list_print(list, print_int);
    printf("\n");


    /* ----------------------------
       TEST 6 : remove élément absent
    ----------------------------- */
    node_t *before = list;
    list = list_remove(list, &x); // ne change rien

    if (list != before)
        printf("ERREUR: remove(element absent) modifie la liste !\n");
    else
        printf("OK: remove(x) où x absent ne change pas la liste\n");


    /* ----------------------------
       TEST 7 : remove tête
    ----------------------------- */
    list = list_remove(list, &d); // retire 40

    printf("Après remove(40) (attendu 10 30) : ");
    list_print(list, print_int);
    printf("\n");


    /* ----------------------------
       TEST 8 : headRemove
    ----------------------------- */
    list = list_headRemove(list); // retire 10

    printf("Après headRemove (attendu 30) : ");
    list_print(list, print_int);
    printf("\n");

    list = list_headRemove(list); // retire 30 → vide

    if (list != NULL)
        printf("ERREUR: Liste après headRemove final devrait être NULL\n");
    else
        printf("OK: headRemove sur dernier élément -> NULL\n");


    /* ----------------------------
       TEST 9 : append sur liste vide
    ----------------------------- */
    list = list_append(list, &a); // [10]

    if (*(int*)list->val != 10)
        printf("ERREUR: append sur liste vide -> %s (attendu 10)\n", val_or_null(list));
    else
        printf("OK: append sur liste vide -> 10\n");


    /* ----------------------------
       TEST 10 : destruction
    ----------------------------- */
    list_destroy(list);
    printf("OK: list_destroy exécuté (vérifier valgrind pour fuites mémoire)\n");


    printf("\n===== FIN DU TEST =====\n");
    return 0;
}
