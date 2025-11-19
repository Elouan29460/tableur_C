#ifndef LISTE_H
#define LISTE_H

typedef struct node{
    void* val;
    struct node* next;
} node_t;

node_t * list_create(void); //creation d'une nouvelle liste

void * list_get_data(const node_t * node);

void list_set_data(node_t * node , void * data); //Lire ou écrire la donnée d'un noeud

node_t * list_next(node_t * node); //Obtenir le noeud suivant

node_t * list_insert(node_t * head, void * data); // Creation et insertion d'un noeud en tête de liste // Retourne la tête de la liste

node_t * list_append(node_t * head, void * data); //Creation et ajout d'un noeud en queue de liste // Retourne la tête de la liste

node_t * list_remove(node_t * head, void * data); // Suppresion de la première instance d'une donnée dans la liste, retourne la tete de liste

node_t * list_headRemove(node_t * head); // Suppression de la tête de la liste // Retourne la nouvelle tête de liste

void list_destroy(node_t * head); // destruction d'une liste //(La libération des données n'est pas prise en charge)

void list_print(node_t * head, void (*fct)(void));

#endif
