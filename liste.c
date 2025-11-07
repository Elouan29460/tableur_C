#include <stdlib.h>
#include "liste.h"


node_t * liste_create(void) {
    return NULL;
}

void * list_get_data(const node_t * node) {
    if (!node) return NULL;
    return node->val;
}

void list_set_data(node_t * node , void * data) {
    if (node) {
        node->val = data;
    }
}

node_t * list_next(node_t * node) {
    if (!node) return NULL;
    return node->next;
}

node_t * list_insert(node_t * head, void * data) {
    node_t* node = malloc(sizeof(node_t));
    if (!node) return head;
    node->val = data;
    node->next = head;
    return node;
}

node_t * list_append(node_t * head, void * data) {
    node_t * node = malloc(sizeof(node_t));
    if (!node || !data) return NULL;

    node->next = NULL;
    node->val = data;

    node_t * node_tmp = head;
    while (node_tmp->next != NULL) {
        node_tmp = node_tmp->next;
    }

    node_tmp->next = node;
    return head;
}

node_t * list_remove(node_t * head, void * data) {
    if (!head) return NULL;

    if (head->val == data) {
        node_t * next = head->next;
        free(head);
        return next;
    }

    node_t* prev = head;
    node_t* curr = head->next;

    while (curr) {
        if (curr->val == data) {
            prev->next = curr->next;
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    return head;
}

node_t * list_headRemove(node_t * head) {
    if (!head) return NULL; 

    node_t* next = head->next;
    free(head);
    return next;
}

void list_destroy(node_t * head) {
    node_t * curr = head;
    while (curr) {
        node_t* next = curr->next;
        free(curr);
        curr = next;
    }
}

void list_print(node_t * head, void (*fct)(void*)) {
    node_t * ptr = head;
    while (ptr) {
        (*fct)(list_get_data(ptr));
        ptr = list_next(ptr);
    }
}

