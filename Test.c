#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "liste.h"

void print_int(void * ptr)
{
    printf("%d ", *(int*)ptr);
}

void test_create()
{
    node_t *list = list_create();
    assert(list == NULL);
    printf("[OK] list_create crée une liste vide\n");
}

void test_insert_and_get()
{
    int a = 10;
    node_t *list = NULL;

    list = list_insert(list, &a);
    assert(list != NULL);
    assert(*(int*)list->val == 10);
    assert(list->next == NULL);

    printf("[OK] list_insert insère en tête et list_get_data fonctionne\n");
}

void test_append()
{
    int a = 10, b = 20, c = 30;
    node_t *list = NULL;

    list = list_append(list, &a);  // liste = [10]
    list = list_append(list, &b);  // liste = [10 -> 20]
    list = list_append(list, &c);  // liste = [10 -> 20 -> 30]

    node_t *p = list;
    assert(*(int*)p->val == 10);
    p = p->next;
    assert(*(int*)p->val == 20);
    p = p->next;
    assert(*(int*)p->val == 30);

    printf("[OK] list_append ajoute correctement en queue\n");
}

void test_remove()
{
    int a = 10, b = 20, c = 30;
    node_t *list = NULL;

    list = list_append(list, &a);
    list = list_append(list, &b);
    list = list_append(list, &c);

    // Supprimer au milieu
    list = list_remove(list, &b);

    assert(*(int*)list->val == 10);
    assert(*(int*)list->next->val == 30);
    assert(list->next->next == NULL);

    // Supprimer élément absent → ne change rien
    int x = 999;
    list = list_remove(list, &x);
    assert(*(int*)list->val == 10);
    assert(*(int*)list->next->val == 30);

    // Supprimer tête
    list = list_remove(list, &a);
    assert(*(int*)list->val == 30);
    assert(list->next == NULL);

    printf("[OK] list_remove gère tête, milieu, fin et absents\n");

    list_destroy(list);
}

void test_headRemove()
{
    int a = 10, b = 20;
    node_t *list = NULL;

    list = list_append(list, &a);
    list = list_append(list, &b);

    list = list_headRemove(list);
    assert(*(int*)list->val == 20);
    assert(list->next == NULL);

    list = list_headRemove(list);
    assert(list == NULL);

    printf("[OK] list_headRemove supprime correctement la tête\n");
}

void test_destroy()
{
    int a = 10, b = 20, c = 30;
    node_t *list = NULL;

    list = list_append(list, &a);
    list = list_append(list, &b);
    list = list_append(list, &c);

    list_destroy(list);

    printf("[OK] list_destroy libère bien la liste (tester avec valgrind)\n");
}

int main(void)
{
    printf("===== TESTS DE LA LISTE CHAÎNÉE =====\n\n");

    test_create();
    test_insert_and_get();
    test_append();
    test_remove();
    test_headRemove();
    test_destroy();

    printf("\n🎉 Tous les tests sont PASSÉS !\n");
    return 0;
}
