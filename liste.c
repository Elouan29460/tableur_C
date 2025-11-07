#include <stdlib.h>
#include "liste.h"


node_t * liste_create(void) {
    return NULL;
}

void * list_get_data(const node_t * node) {
    if (node == NULL) {
        perror("erreur");
    }
    return node->val;
}

void list_set_data(node_t * node , void * data) {
    node->val = data;
}

node_t * list_next(node_t * node) {
    return node->next;
}

node_t * list_insert(node_t * head, void * data) {
    node_t * node = liste_create();
    head = malloc(sizeof(node_t*));
    node->val = data;
    node->next = head;
    return node;
}


