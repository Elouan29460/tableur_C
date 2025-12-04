#include <stdio.h>
#include <stdlib.h>
#include "cellule.h"

/**
 * Ajoute une relation de succession entre deux cellules
 * predecessor -> successor (successor dépend de predecessor)
 */
void add_successor(s_cell *predecessor, s_cell *successor) {
    if (predecessor == NULL || successor == NULL) return;
    
    // Vérifier si le successeur n'est pas déjà dans la liste
    node_t *current = predecessor->successors;
    while (current != NULL) {
        if (list_get_data(current) == successor) {
            return; // Déjà présent
        }
        current = list_next(current);
    }
    
    // Ajouter le successeur
    predecessor->successors = list_append(predecessor->successors, successor);
}

/**
 * Retire une relation de succession
 */
void remove_successor(s_cell *predecessor, s_cell *successor) {
    if (predecessor == NULL || successor == NULL) return;
    
    // Reconstruire la liste sans le successeur à retirer
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

/**
 * Met à jour les dépendances et les successeurs d'une cellule
 * après modification de sa formule
 */
void update_dependencies(s_cell *cell) {
    if (cell == NULL) return;
    
    node_t *old_dep = cell->dependencies;
    while (old_dep != NULL) {
        s_cell *pred = (s_cell *)list_get_data(old_dep);
        remove_successor(pred, cell);
        old_dep = list_next(old_dep);
    }
    
    list_destroy(cell->dependencies);
    cell->dependencies = NULL;
    
    // 3. Construire la nouvelle liste de dépendances à partir des tokens
    node_t *current = cell->tokens;
    while (current != NULL) {
        s_token *token = (s_token *)list_get_data(current);
        
        if (token->type == REF && token->value.ref != NULL) {
            s_cell *ref_cell = token->value.ref;
            
            // Ajouter à la liste des dépendances si pas déjà présent
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
                // Ajouter cette cellule aux successeurs de la cellule référencée
                add_successor(ref_cell, cell);
            }
        }
        
        current = list_next(current);
    }
}

/**
 * Calcule le degré négatif d'une cellule dans un sous-graphe
 * (nombre de prédécesseurs présents dans le sous-graphe)
 */
int compute_negative_degree(s_cell *cell, node_t *subgraph) {
    if (cell == NULL) return 0;
    
    int degree = 0;
    node_t *dep = cell->dependencies;
    
    while (dep != NULL) {
        s_cell *pred = (s_cell *)list_get_data(dep);
        
        // Vérifier si ce prédécesseur est dans le sous-graphe
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

/**
 * Construit le sous-graphe des successeurs directs et indirects d'une cellule
 */
static node_t *build_subgraph(s_cell *cell) {
    if (cell == NULL) return NULL;
    
    node_t *subgraph = NULL;
    node_t *to_visit = NULL;
    
    // Ajouter tous les successeurs directs à visiter
    node_t *succ = cell->successors;
    while (succ != NULL) {
        s_cell *successor = (s_cell *)list_get_data(succ);
        to_visit = list_append(to_visit, successor);
        succ = list_next(succ);
    }
    
    // Parcours en largeur pour trouver tous les successeurs
    while (to_visit != NULL) {
        s_cell *current = (s_cell *)list_get_data(to_visit);
        to_visit = list_next(to_visit);
        
        // Vérifier si déjà dans le sous-graphe
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
            
            // Ajouter ses successeurs à visiter
            node_t *curr_succ = current->successors;
            while (curr_succ != NULL) {
                to_visit = list_append(to_visit, list_get_data(curr_succ));
                curr_succ = list_next(curr_succ);
            }
        }
    }
    
    return subgraph;
}

/**
 * Réévalue tous les successeurs d'une cellule en utilisant le tri topologique
 * Implémente l'algorithme de la figure 3
 * Utilise degre_negatif et estPassee dans la structure s_cell
 */
void reevaluate_successors(s_cell *cell) {
    if (cell == NULL) return;
    
    // 1. Construire le sous-graphe X des successeurs
    node_t *subgraph = build_subgraph(cell);
    if (subgraph == NULL) return; // Pas de successeurs
    
    // 2. Initialisation : calculer les degrés négatifs et marquer comme non passées
    node_t *current = subgraph;
    while (current != NULL) {
        s_cell *s = (s_cell *)list_get_data(current);
        s->degre_negatif = compute_negative_degree(s, subgraph);
        s->estPassee = 0;
        current = list_next(current);
    }
    
    // Créer la liste des cellules de degré 0
    node_t *liste = NULL;
    current = subgraph;
    while (current != NULL) {
        s_cell *s = (s_cell *)list_get_data(current);
        if (s->degre_negatif == 0) {
            liste = list_append(liste, s);
        }
        current = list_next(current);
    }
    
    // 3. Itération : évaluation dans l'ordre topologique
    while (liste != NULL) {
        // Retirer un sommet de la liste
        s_cell *s = (s_cell *)list_get_data(liste);
        liste = list_next(liste);
        
        // Vérifier si déjà passée (sécurité)
        if (s->estPassee) continue;
        
        // Marquer comme passée
        s->estPassee = 1;
        
        // Évaluer la cellule
        evaluate_cell(s);
        
        // Pour chaque successeur de s
        node_t *succ = s->successors;
        while (succ != NULL) {
            s_cell *sj = (s_cell *)list_get_data(succ);
            
            // Vérifier si ce successeur est dans le sous-graphe
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
                // Décrémenter le degré négatif
                sj->degre_negatif--;
                
                // Si degré devient 0, ajouter à la liste
                if (sj->degre_negatif == 0) {
                    liste = list_append(liste, sj);
                }
            }
            
            succ = list_next(succ);
        }
    }
    
    // Nettoyage
    list_destroy(subgraph);
}
