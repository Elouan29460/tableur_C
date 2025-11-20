#include <stdio.h>
#include <stdlib.h>
#include "liste.h"

void print_int(void * ptr)
{
    if (!ptr) printf("NULL ");
    else printf("%d ", *(int*)ptr);
}

const char* val_or_null(node_t *n)
{
    static char buf[50];
    if (!n) return "NULL";
    if (!n->val) return "NULL(DATA)";
    sprintf(buf, "%d", *(int*)n->val);
    return buf;
}

/* ============================================================
   TEST MAXI COMPLET DE LISTE CHAINEE
   ============================================================ */
int main(void)
{
    printf("===== TEST VERSION MAXI COMPLETE =====\n\n");

    int a=10, b=20, c=30, d=40, x=999;

    /* --------------------------------------------------
       1. Création
       -------------------------------------------------- */
    node_t *list = list_create();
    printf("Test list_create: %s (attendu NULL)\n", val_or_null(list));


    /* --------------------------------------------------
       2. list_get_data et next avec NULL
       -------------------------------------------------- */
    printf("list_get_data(NULL) -> %p (attendu NULL)\n", list_get_data(NULL));
    printf("list_next(NULL) -> %p (attendu NULL)\n", (void*)list_next(NULL));


    /* --------------------------------------------------
       3. insert sur liste vide
       -------------------------------------------------- */
    list = list_insert(list, &a);
    printf("Après insert(a), tête = %s (attendu 10)\n", val_or_null(list));


    /* --------------------------------------------------
       4. insert sur liste non vide
       -------------------------------------------------- */
    list = list_insert(list, &b);
    printf("Après insert(b), tête = %s (attendu 20)\n", val_or_null(list));


    /* --------------------------------------------------
       5. append sur liste non vide
       -------------------------------------------------- */
    list = list_append(list, &c);
    list = list_append(list, &d);

    printf("Après append(c,d), liste (attendu 20 10 30 40) : ");
    list_print(list, print_int);
    printf("\n");


    /* --------------------------------------------------
       6. append sur liste NULL
       -------------------------------------------------- */
    node_t *list2 = NULL;
    list2 = list_append(list2, &a);
    printf("append sur NULL -> %s (attendu 10)\n", val_or_null(list2));
    list_destroy(list2);


    /* --------------------------------------------------
       7. remove au milieu
       -------------------------------------------------- */
    list = list_remove(list, &c);
    printf("remove(c), liste (attendu 20 10 40) : ");
    list_print(list, print_int);
    printf("\n");


    /* --------------------------------------------------
       8. remove tête
       -------------------------------------------------- */
    list = list_remove(list, &b);
    printf("remove(b), liste (attendu 10 40) : ");
    list_print(list, print_int);
    printf("\n");


    /* --------------------------------------------------
       9. remove dernier
       -------------------------------------------------- */
    list = list_remove(list, &d);
    printf("remove(d), liste (attendu 10) : ");
    list_print(list, print_int);
    printf("\n");


    /* --------------------------------------------------
       10. remove élément absent
       -------------------------------------------------- */
    node_t *before = list;
    list = list_remove(list, &x);
    printf("remove(x absent), liste inchangée (attendu 10) : ");
    list_print(list, print_int);
    printf("\n");


    /* --------------------------------------------------
       11. remove sur liste de 1 élément
       -------------------------------------------------- */
    list = list_remove(list, &a);
    printf("remove(a), liste devenue NULL (attendu NULL): %s\n", val_or_null(list));


    /* --------------------------------------------------
       12. remove sur liste vide
       -------------------------------------------------- */
    list = list_remove(list, &a);
    printf("remove(a) sur liste vide -> %s (attendu NULL)\n", val_or_null(list));


    /* --------------------------------------------------
       13. headRemove sur liste NULL
       -------------------------------------------------- */
    printf("headRemove(NULL) -> %s (attendu NULL)\n", val_or_null(list_headRemove(NULL)));


    /* --------------------------------------------------
       14. headRemove sur liste d’un élément
       -------------------------------------------------- */
    list = list_insert(list, &a);
    list = list_headRemove(list);
    printf("headRemove sur liste [10] -> %s (attendu NULL)\n", val_or_null(list));


    /* --------------------------------------------------
       15. insertion d’un élément dont la DATA = NULL
       -------------------------------------------------- */
    list = list_insert(list, NULL);
    printf("insert(NULL) -> tête = %s (attendu NULL(DATA))\n", val_or_null(list));

    list_destroy(list);
    list = NULL;


    /* --------------------------------------------------
       16. Stress test : 10 000 insertions
       -------------------------------------------------- */
    printf("\n--- STRESS TEST (10 000 insertions + destruction) ---\n");

    for (int i = 0; i < 10000; i++) {
        list = list_insert(list, &a);
    }
    printf("Insertion OK (10 000 éléments)\n");

    list_destroy(list);
    printf("Destruction OK (tester valgrind : aucune fuite prévue)\n");


    /* --------------------------------------------------
       17. Stress test : 10 000 append
       -------------------------------------------------- */
    printf("\n--- STRESS APPEND TEST (10 000 append) ---\n");

    for (int i = 0; i < 10000; i++) {
        list = list_append(list, &b);
    }
    printf("Append OK (10 000 éléments)\n");

    list_destroy(list);


    /* --------------------------------------------------
       FIN
       -------------------------------------------------- */
    printf("\n===== FIN DU TEST MAXI COMPLET =====\n");
    return 0;
}
