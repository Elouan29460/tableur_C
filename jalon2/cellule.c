#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "cellule.h"

// La sainte feuille de calcul
s_sheet *sheet = NULL;




/**
 * Les opérateurs
 */
void op_add(my_stack_t *eval) {
    double b = STACK_POP(eval, double);
    double a = STACK_POP(eval, double);
    STACK_PUSH(eval, a + b, double);
}

void op_sub(my_stack_t *eval) {
    double b = STACK_POP(eval, double);
    double a = STACK_POP(eval, double);
    STACK_PUSH(eval, a - b, double);
}

void op_mul(my_stack_t *eval) {
    double b = STACK_POP(eval, double);
    double a = STACK_POP(eval, double);
    STACK_PUSH(eval, a * b, double);
}

void op_div(my_stack_t *eval) {
    double b = STACK_POP(eval, double);
    double a = STACK_POP(eval, double);
    if (b != 0.0) {
        STACK_PUSH(eval, a / b, double);
    } else {
        STACK_PUSH(eval, 0.0, double);
    }
}

void op_mod(my_stack_t *eval) {
    double b = STACK_POP(eval, double);
    double a = STACK_POP(eval, double);
    if (b != 0.0) {
        STACK_PUSH(eval, fmod(a, b), double);
    } else {
        STACK_PUSH(eval, 0.0, double);
    }
}

void op_cos(my_stack_t *eval) {
    double a = STACK_POP(eval, double);
    STACK_PUSH(eval, cos(a), double);
}

// Tableau des opérations disponibles
s_operation operations[] = {
    {"+", op_add},
    {"-", op_sub},
    {"*", op_mul},
    {"/", op_div},
    {"%", op_mod},
    {"cos", op_cos},
    {NULL, NULL}
};

/**
 * Création et suppression des cellules et des tkens
 */

s_cell *cell_create(void) {
    s_cell *cell = (s_cell *)malloc(sizeof(s_cell));
    if (cell == NULL) {
        return NULL;
    }
    cell->chaine = NULL;
    cell->value = 0.0;
    cell->tokens = NULL;
    cell->dependencies = NULL;
    return cell;
}

void cell_destroy(s_cell *cell) {
    if (cell == NULL) return;
    if (cell->chaine != NULL) {
        free(cell->chaine);
    }
    
    node_t *current = cell->tokens;
    while (current != NULL) {
        s_token *token = (s_token *)list_get_data(current);
        token_destroy(token);
        current = list_next(current);
    }

    list_destroy(cell->tokens);
    list_destroy(cell->dependencies);
    free(cell);
}

s_token *token_create(token_type type) {
    s_token *token = (s_token *)malloc(sizeof(s_token));
    if (token == NULL) {
        return NULL;
    }
    token->type = type;
    return token;
}

void token_destroy(s_token *token) {
    if (token != NULL) {
        free(token);
    }
}

/**
 * Fonctions utilitaires
 */

int is_number(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    
    double votreReel;
    return sscanf(str,"%lf",&votreReel);
}

int is_operator(const char *str) {
    if (str == NULL) return 0;
    int i = 0;
    
    while (operations[i].function != NULL && operations[i].name != NULL) {
        if (strcmp(str, operations[i].name) == 0) {
            return 1;
        }
        i++;
    }
    return 0;
}

int is_cell_reference(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    
    // Format: Lettre(s) + Nombre(s), ex: A1, B10, AA23
    int i = 0;
    
    // Au moins une lettre
    if (!isalpha(str[i])) return 0;
    while (isalpha(str[i])) {
        i++;
    }
    
    // Au moins un chiffre
    if (!isdigit(str[i])) return 0;
    while (isdigit(str[i])) {
        i++;
    }
    
    // Doit être la fin de la chaîne
    return str[i] == '\0';
}

s_cell *get_cell_by_reference(const char *ref, s_sheet *sheet) {
    if (sheet == NULL || ref == NULL) return NULL;
    
    //A finir :'(
    return NULL;
}

/**
 * Analyse d'une chaine de caractères contenue dans une cellule
 * Modifie directement la liste de tokens de la cellule
 */

void analyse_chaine_cellule(s_cell *cellule, s_sheet *sheet) {
    if (cellule == NULL || cellule->chaine == NULL) {
        return;
    }
    
    //On dégage l'ancienne liste de token si il en a une
    if (cellule->tokens != NULL) {
        node_t *current = cellule->tokens;
        while (current != NULL) {
            s_token *token = (s_token *)list_get_data(current);
            token_destroy(token);
            current = list_next(current);
        }
        list_destroy(cellule->tokens);
    }
    
    cellule->tokens = list_create();
    
    //Faire une copie de la chaîne car strtok la modifie
    char *formula_copy = strdup(cellule->chaine);
    if (formula_copy == NULL) {
        return;
    }
    
    //On utilise strtok pour séparer notre chaine de caarctère en plusieurs éléments pas un séparateur
    char *token_str = strtok(formula_copy, " \t\n\r");
    
    while (token_str != NULL) {
        s_token *token = NULL;
        
        if (is_number(token_str)) {
            // C'est un nombre
            token = token_create(VALUE);
            token->value.cst = atof(token_str);
            cellule->tokens = list_append(cellule->tokens, token);
            
        } else if (is_cell_reference(token_str)) {
            // C'est une référence de cellule
            token = token_create(REF);
            token->value.ref = get_cell_by_reference(token_str, sheet);
            cellule->tokens = list_append(cellule->tokens, token);
            
        } else if (is_operator(token_str)) {
            // C'est un opérateur
            token = token_create(OPERATOR);
            // Trouver l'opérateur correspondant
            int j = 0;
            while (operations[j].function != NULL && operations[j].name != NULL) {
                if (strcmp(token_str, operations[j].name) == 0) {
                    token->value.operator = operations[j].function;
                    break;
                }
                j++;
            }
            cellule->tokens = list_append(cellule->tokens, token);
        }
        
        // Passer au token suivant
        token_str = strtok(NULL, " \t\n\r");
    }
    
    // Libérer la copie de la formule
    free(formula_copy);
}

/**
 * Fonction d'évaluation
 */

double evaluate_cell(s_cell *cell) {
    if (cell == NULL || cell->tokens == NULL) {
        return 0.0;
    }
    
    // Créer une pile pour l'évaluation (notation polonaise inverse)
    my_stack_t *eval_stack = STACK_CREATE(100, double);
    if (eval_stack == NULL) {
        return 0.0;
    }
    
    // Parcourir tous les tokens
    node_t *current = cell->tokens;
    while (current != NULL) {
        s_token *token = (s_token *)list_get_data(current);
        
        if (token->type == VALUE) {
            // Empiler la valeur
            STACK_PUSH(eval_stack, token->value.cst, double);
        } else if (token->type == REF) {
            // Évaluer la cellule référencée et empiler sa valeur
            if (token->value.ref != NULL) {
                double ref_value = evaluate_cell(token->value.ref);
                STACK_PUSH(eval_stack, ref_value, double);
            } else {
                // Référence non résolue -> 0.0
                STACK_PUSH(eval_stack, 0.0, double);
            }
        } else if (token->type == OPERATOR) {
            // Appliquer l'opérateur
            if (token->value.operator != NULL) {
                token->value.operator(eval_stack);
            }
        }
        
        current = list_next(current);
    }
    
    // Le résultat final est au sommet de la pile
    double result = STACK_POP(eval_stack, double);
    
    STACK_REMOVE(eval_stack);
    
    cell->value = result;
    return result;
}