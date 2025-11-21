#include <stdlib.h>
#include "liste.h"

/**
 * Création d'une nouvelle liste
 * Liste chaînée vide représentée par pointeur NULL
 */
node_t * list_create(void) {
    return NULL;
}

/**
 * Renvoie la donnée contenue dans un nœud
 * Si noeud est NULL, renvoie NULL pour éviter un crash
 */
void * list_get_data(const node_t * node) {
    if (!node) return NULL;
    return node->val;
}

/**
 * Modifie la donnée contenue dans un noeud
 * Vérifie que le noeud est valide avant d'écrire dedans
 */
void list_set_data(node_t * node , void * data) {
    if (node) {
        node->val = data;
    }
}

/**
 * Obtenir le noeud suivant
 * Si le noeud est NULL, retourne NULL
 */
node_t * list_next(node_t * node) {
    if (!node) return NULL;
    return node->next;
}

/**
 * Creation et insertion d'un noeud en tête de liste
 * Alloue un nouveau noeud
 * Stocke la donnée
 * Retourne la tête de la liste
 */
node_t * list_insert(node_t * head, void * data) {
    node_t* node = malloc(sizeof(node_t));
    if (!node) return head; //Si echec alloc, on garde l'ancienne
    node->val = data;
    node->next = head; //Le nouveau noeud point sur l'ancienne tête
    return node;
}

/**
 * Creation et ajout d'un noeud en queue de liste
 * Alloue un nouveau noeud
 * Si liste vide, nouveau noeud devient tête
 * Accroche le nouveau noeud à la fin
 * Retourne la tête de la liste
 */
node_t * list_append(node_t * head, void * data) {
    node_t * node = malloc(sizeof(node_t));
    if (!node) return head; //Si echec alloc

    node->next = NULL; //Dernier élément
    node->val = data;

    if (!head) {
        //Si liste vide, nouveau noeud devient tête
        return node;
    }
    
    //Parcours de la liste jusqu'à la fin
    node_t * node_tmp = head;
    while (node_tmp->next != NULL) {
        node_tmp = node_tmp->next;
    }

    node_tmp->next = node;
    return head;
}

/**
 * Suppresion de la première instance d'une donnée dans la liste
 * Si tête contient donnée, supprimé
 * Sinon on parcourt avec prev et curr
 * Comparaion via adresse
 * Retourne la tete de liste
 */
node_t * list_remove(node_t * head, void * data) {
    if (!head) return NULL;

    //Cas particulier, donnée dans la tête
    if (head->val == data) {
        node_t * next = head->next;
        free(head);
        return next;
    }

    node_t* prev = head;
    node_t* curr = head->next;

    while (curr) {
        if (curr->val == data) {
            //On enlève le noeud courrant
            prev->next = curr->next;
            free(curr);
            break; //Stop dès la première occurrence
        }
        prev = curr;
        curr = curr->next;
    }

    return head;
}

/**
 * Suppression de la tête de la liste
 * Retourne la nouvelle tête de liste
 */
node_t * list_headRemove(node_t * head) {
    if (!head) return NULL; 

    node_t* next = head->next;
    free(head);
    return next;
}

/**
 * destruction d'une liste
 * (La libération des données n'est pas prise en charge)
 * Libère uniquement les noeuds
 */
void list_destroy(node_t * head) {
    node_t * curr = head;
    while (curr) {
        node_t* next = curr->next;
        free(curr); //Libère
        curr = next;
    }
}

/**
 * Affichage des données dans une liste
 * fct(void*) : permet d'imprimer n'importe quel type de données
 */
void list_print(node_t * head, void (*fct)(void*)) {
    node_t * ptr = head;
    while (ptr) {
        (*fct)(list_get_data(ptr)); //Appel de la fonction d'affichage
        ptr = list_next(ptr);
    }
}
