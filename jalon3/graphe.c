#include <stdio.h>
#include <stdlib.h>
#include "cellule.h"


void ajouter_successeur(s_cell *predecessor, s_cell *successor) {
    if (predecessor == NULL || successor == NULL) return;
    
    // check si pas deja present
    node_t *current = predecessor->successors;
    while (current != NULL) {
        if (list_get_data(current) == successor) {
            return;
        }
        current = list_next(current);
    }
    
    predecessor->successors = list_append(predecessor->successors, successor);
}


void retirer_successeur(s_cell *predecessor, s_cell *successor) {
    if (predecessor == NULL || successor == NULL) return;
    
    node_t *new_list = NULL;
    node_t *current = predecessor->successors;
    
    while (current != NULL) {
        s_cell *data = (s_cell *)list_get_data(current);
        if (data != successor) {
            new_list = list_append(new_list, data);
        }
        current = list_next(current);
    }
    
    list_destroy(predecessor->successors);
    predecessor->successors = new_list;
}

//ON met à jour les successeurs et predecesseurs aprys modif
void maj_dependances(s_cell *cell) {
    if (cell == NULL) return;
    
    node_t *old_dep = cell->dependencies;
    while (old_dep != NULL) {
        s_cell *pred = (s_cell *)list_get_data(old_dep);
        retirer_successeur(pred, cell);
        old_dep = list_next(old_dep);
    }
    
    list_destroy(cell->dependencies);
    cell->dependencies = NULL;
    
    //nouvelle liste
    node_t *current = cell->tokens;
    while (current != NULL) {
        s_token *token = (s_token *)list_get_data(current);
        
        if (token->type == REF && token->value.ref != NULL) {
            s_cell *ref_cell = token->value.ref;
            int already_present = 0;
            node_t *check = cell->dependencies;
            while (check != NULL) {
                if (list_get_data(check) == ref_cell) {
                    already_present = 1;
                    break;
                }
                check = list_next(check);
            }
            if (!already_present) {
                cell->dependencies = list_append(cell->dependencies, ref_cell);
                ajouter_successeur(ref_cell, cell);
            }
        }
        current = list_next(current);
    }
}


int calculer_degre_negatif(s_cell *cell, node_t *subgraph) {
    if (cell == NULL) return 0;
    int degree = 0;
    node_t *dep = cell->dependencies;
    
    while (dep != NULL) {
        s_cell *pred = (s_cell *)list_get_data(dep);
        node_t *check = subgraph;
        while (check != NULL) {
            if (list_get_data(check) == pred) {
                degree++;
                break;
            }
            check = list_next(check);
        }
        dep = list_next(dep);
    }
    return degree;
}


static node_t *sous_graphe(s_cell *cell) {
    if (cell == NULL) return NULL;
    
    node_t *subgraph = NULL;
    node_t *to_visit = NULL;
    
    node_t *succ = cell->successors;
    while (succ != NULL) {
        s_cell *successor = (s_cell *)list_get_data(succ);
        to_visit = list_append(to_visit, successor);
        succ = list_next(succ);
    }
    
    while (to_visit != NULL) {
        s_cell *current = (s_cell *)list_get_data(to_visit);
        to_visit = list_next(to_visit);
        
        int already_in = 0;
        node_t *check = subgraph;
        while (check != NULL) {
            if (list_get_data(check) == current) {
                already_in = 1;
                break;
            }
            check = list_next(check);
        }
        
        if (!already_in) {
            subgraph = list_append(subgraph, current);
            
            node_t *curr_succ = current->successors;
            while (curr_succ != NULL) {
                to_visit = list_append(to_visit, list_get_data(curr_succ));
                curr_succ = list_next(curr_succ);
            }
        }
    }
    
    return subgraph;
}

void evaluation_successeurs(s_cell *cell) {
    if (cell == NULL) return;
    
    //On construit le sous graphe
    node_t *subgraph = sous_graphe(cell);
    if (subgraph == NULL) return;
    
    //On calcule les degres négatifs pour tous
    node_t *current = subgraph;
    while (current != NULL) {
        s_cell *s = (s_cell *)list_get_data(current);
        s->degre_negatif = calculer_degre_negatif(s, subgraph);
        s->estPassee = 0;
        current = list_next(current);
    }
    
    //Liste des celluels à calculer maintenant
    node_t *liste = NULL;
    current = subgraph;
    while (current != NULL) {
        s_cell *s = (s_cell *)list_get_data(current);
        if (s->degre_negatif == 0) {
            liste = list_append(liste, s);
        }
        current = list_next(current);
    }
    
    //Evaluer dans l'ordre topologique
    while (liste != NULL) {
        s_cell *s = (s_cell *)list_get_data(liste);
        liste = list_next(liste);
        if (s->estPassee) continue;
        s->estPassee = 1;
        evaluate_cell(s);
        node_t *succ = s->successors;
        while (succ != NULL) {
            s_cell *sj = (s_cell *)list_get_data(succ);
            
            node_t *check = subgraph;
            int in_subgraph = 0;
            while (check != NULL) {
                if (list_get_data(check) == sj) {
                    in_subgraph = 1;
                    break;
                }
                check = list_next(check);
            }
            if (in_subgraph) {
                sj->degre_negatif--;
                
                //degre 0 = ajout dans la liste à calculée
                if (sj->degre_negatif == 0) {
                    liste = list_append(liste, sj);
                }
            }
            
            succ = list_next(succ);
        }
    }
    
    list_destroy(subgraph);
}
