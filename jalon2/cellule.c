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
void addition(my_stack_t *eval) {
    double b = STACK_POP(eval, double);
    double a = STACK_POP(eval, double);
    STACK_PUSH(eval, a + b, double);
}

void soustraction(my_stack_t *eval) {
    double b = STACK_POP(eval, double);
    double a = STACK_POP(eval, double);
    STACK_PUSH(eval, a - b, double);
}

void multiplication(my_stack_t *eval) {
    double b = STACK_POP(eval, double);
    double a = STACK_POP(eval, double);
    STACK_PUSH(eval, a * b, double);
}

void division(my_stack_t *eval) {
    double b = STACK_POP(eval, double);
    double a = STACK_POP(eval, double);
    if (b != 0.0) {
        STACK_PUSH(eval, a / b, double);
    } else {
        STACK_PUSH(eval, 0.0, double);
    }
}

void modulo(my_stack_t *eval) {
    double b = STACK_POP(eval, double);
    double a = STACK_POP(eval, double);
    if (b != 0.0) {
        STACK_PUSH(eval, fmod(a, b), double);
    } else {
        STACK_PUSH(eval, 0.0, double); // Modulo par zéro -> 0.0
    }
}

void cosinus(my_stack_t *eval) {
    double a = STACK_POP(eval, double);
    STACK_PUSH(eval, cos(a), double);
}

// Tableau des opérations disponibles
s_operation operations[] = {
    {"+", addition},
    {"-", soustraction},
    {"*", multiplication},
    {"/", division},
    {"%", modulo},
    {"cos", cosinus},
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
    // sscanf retourne le nombre d'éléments lus
    return sscanf(str, "%lf", &votreReel) == 1;
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
    
    // Format: 1 Lettre (A-Z) + Nombre(s), ex: A1, B10, Z99
    // str[0] doit être une lettre majuscule
    if (!isupper(str[0])) return 0;
    
    // str[1] doit être un chiffre
    if (!isdigit(str[1])) return 0;
    
    // Tous les caractères suivants doivent être des chiffres
    for (int i = 2; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return 0;
    }
    
    return 1;
}

s_cell *get_cell_by_reference(const char *ref) {
    if (sheet == NULL || ref == NULL) return NULL;
    
    //A finir :'(
    return NULL;
}

/**
 * Analyse d'une chaine de caractères contenue dans une cellule
 * Modifie directement la liste de tokens de la cellule
 * 
 * 3 cas possibles :
 * 1. Commence par '=' : formule à analyser (ex: "=5 3 +")
 * 2. Nombre simple : convertir directement (ex: "3.2" -> 3.2)
 * 3. Autre (texte/vide) : valeur = 0.0 (ex: "" ou "bonjour" -> 0.0)
 */

void analyse_chaine_cellule(s_cell *cellule) {
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
    
    // Cas 1 : Formule (commence par '=')
    if (cellule->chaine[0] == '=') {
        // Analyser la formule (sauter le '=')
        const char *formula = cellule->chaine + 1;
        
        //Faire une copie de la chaîne car strtok la modifie
        char *formula_copy = strdup(formula);
        if (formula_copy == NULL) {
            return;
        }
        
        //On utilise strtok pour séparer notre chaine de caractère en plusieurs éléments par un séparateur
        char *token_str = strtok(formula_copy, " \t\n\r");
        
        while (token_str != NULL) {
            s_token *token = NULL;
            
            if (is_number(token_str)) {
                // C'est un nombre
                token = token_create(VALUE);
                sscanf(token_str, "%lf", &token->value.cst);
                cellule->tokens = list_append(cellule->tokens, token);
                
            } else if (is_cell_reference(token_str)) {
                // C'est une référence de cellule
                token = token_create(REF);
                token->value.ref = get_cell_by_reference(token_str);
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
        
    } else if (is_number(cellule->chaine)) {
        // Cas 2 : Nombre simple (ex: "3.2" ou "5")
        s_token *token = token_create(VALUE);
        sscanf(cellule->chaine, "%lf", &token->value.cst);
        cellule->tokens = list_append(cellule->tokens, token);
        cellule->value = token->value.cst;  // Mettre directement la valeur
        
    } else {
        // Cas 3 : Texte, vide ou invalide -> 0.0
        cellule->value = 0.0;
    }
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