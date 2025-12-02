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
        STACK_PUSH(eval, 0.0, double);
    }
}

void cosinus(my_stack_t *eval) {
    double a = STACK_POP(eval, double);
    STACK_PUSH(eval, cos(a), double);
}

//Tableau des opérations
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

void cell_setStr(s_cell *cell, const char *formule) {
    if (cell == NULL || formule == NULL) {
        return;
    }
    if (cell->chaine != NULL) {
        free(cell->chaine);
    }
    cell->chaine = strdup(formule);
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
 * Gestion de la feuille de calcul
 */

s_sheet *sheet_create(int nb_rows, int nb_cols) {
    s_sheet *s = (s_sheet *)malloc(sizeof(s_sheet));
    if (s == NULL) return NULL;
    
    s->filename = NULL;
    s->nb_rows = nb_rows;
    s->nb_cols = nb_cols;
    s->cells = NULL;
    
    //Malloc le tableau 2D
    s->cells_2d = (s_cell ***)malloc(nb_rows * sizeof(s_cell **));
    if (s->cells_2d == NULL) {
        free(s);
        return NULL;
    }
    
    for (int i = 0; i < nb_rows; i++) {
        s->cells_2d[i] = (s_cell **)calloc(nb_cols, sizeof(s_cell *));
        if (s->cells_2d[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(s->cells_2d[j]);
            }
            free(s->cells_2d);
            free(s);
            return NULL;
        }
    }
    
    return s;
}

void sheet_destroy(s_sheet *s) {
    if (s == NULL) return;
    
    if (s->filename != NULL) {
        free(s->filename);
    }
    
    //Détruire le tableau 2D
    if (s->cells_2d != NULL) {
        for (int i = 0; i < s->nb_rows; i++) {
            if (s->cells_2d[i] != NULL) {
                for (int j = 0; j < s->nb_cols; j++) {
                    if (s->cells_2d[i][j] != NULL) {
                        cell_destroy(s->cells_2d[i][j]);
                    }
                }
                free(s->cells_2d[i]);
            }
        }
        free(s->cells_2d);
    }
    
    //Détruire la liste
    list_destroy(s->cells);
    
    free(s);
}

void feuille_setCell(s_cell *cell, const char* nom) {
    if (sheet == NULL || nom == NULL || !is_cell_reference(nom)) {
        return;
    }
    
    int col = nom[0] - 'A';
    int row = atoi(&nom[1]) - 1;
    
    if (row < 0 || row >= sheet->nb_rows || col < 0 || col >= sheet->nb_cols) {
        return;
    }

    sheet->cells_2d[row][col] = cell;
}

/**
 * Fonctions utilitaires
 */

int is_number(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    double votreReel;
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
    
    //Est une majuscule
    if (!isupper(str[0])) return 0;
    
    //premier est un chiffre
    if (!isdigit(str[1])) return 0;
    
    //Le reste aussi des chiffres
    for (int i = 2; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

s_cell *get_cell_by_reference(const char *ref) {
    if (sheet == NULL || ref == NULL || !is_cell_reference(ref)) {
        return NULL;
    }
    
    int col = ref[0] - 'A';
    int row = atoi(&ref[1]) - 1;
    
    //pas de dépassement
    if (row < 0 || row >= sheet->nb_rows || col < 0 || col >= sheet->nb_cols) {
        return NULL;
    }
    
    //On crée la cellule si elle existe pas
    if (sheet->cells_2d[row][col] == NULL) {
        sheet->cells_2d[row][col] = cell_create();
        if (sheet->cells_2d[row][col] != NULL) {
            sheet->cells_2d[row][col]->chaine = strdup("");
            sheet->cells_2d[row][col]->value = 0.0;
        }
        

    }
    
    return sheet->cells_2d[row][col];
}

//Analyse la chaine d'une cellule
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
    
    //Vérifier que la chaine commence par un = pour savoir si c'est une formule
    if (cellule->chaine[0] == '=') {
        //On analyse la formule mais sans le =
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
                //VALUE
                token = token_create(VALUE);
                sscanf(token_str, "%lf", &token->value.cst);
                cellule->tokens = list_append(cellule->tokens, token);
                
            } else if (is_cell_reference(token_str)) {
                //REF
                token = token_create(REF);
                token->value.ref = get_cell_by_reference(token_str);
                cellule->tokens = list_append(cellule->tokens, token);
                
            } else if (is_operator(token_str)) {
                //OPERATEUR
                token = token_create(OPERATOR);
                //Trouver l'opérateur correspondant
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
            
            //On passe au token suivant
            token_str = strtok(NULL, " \t\n\r");
        }
        free(formula_copy);
        
    //Pas une formule donc vérifier si c'est un nombre
    } else if (is_number(cellule->chaine)) {
        
        s_token *token = token_create(VALUE);
        sscanf(cellule->chaine, "%lf", &token->value.cst);
        cellule->tokens = list_append(cellule->tokens, token);
        cellule->value = token->value.cst;
        
    } else {
        
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
    
    //Création de la pile
    my_stack_t *eval_stack = STACK_CREATE(100, double);
    if (eval_stack == NULL) {
        return 0.0;
    }
    
    //On parcours tous les tokens
    node_t *current = cell->tokens;
    while (current != NULL) {
        s_token *token = (s_token *)list_get_data(current);
        
        if (token->type == VALUE) {
            STACK_PUSH(eval_stack, token->value.cst, double);
        } else if (token->type == REF) {
            if (token->value.ref != NULL) {
                double ref_value = evaluate_cell(token->value.ref);
                STACK_PUSH(eval_stack, ref_value, double);
            } else {
                STACK_PUSH(eval_stack, 0.0, double);
            }
        } else if (token->type == OPERATOR) {
            if (token->value.operator != NULL) {
                token->value.operator(eval_stack);
            }
        }
        
        current = list_next(current);
    }
    
    double result = STACK_POP(eval_stack, double);
    
    STACK_REMOVE(eval_stack);
    
    cell->value = result;
    return result;
}