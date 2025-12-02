#ifndef CELLULE_H
#define CELLULE_H

#include "../jalon1/liste.h"
#include "stack.h"

// Structure de base pour une cellule
typedef struct s_cell {
    char *chaine;
    double value;
    node_t *tokens;
    node_t *dependencies;
} s_cell;

// Types de jetons
typedef enum {
    VALUE,
    REF,
    OPERATOR
} token_type;

// Structure pour un jeton
typedef struct token {
    token_type type;
    union {
        double cst;
        s_cell *ref;
        void (*operator)(my_stack_t *eval);
    } value;
} s_token;




// Structure pour définir une opération
typedef struct {
    char *name;
    void (*function)(my_stack_t *eval);
} s_operation;

// Structure pour la feuille de calcul
typedef struct {
    char *filename;
    int nb_rows;
    int nb_cols;
    node_t *cells;
} s_sheet;

// Variable globale pour la feuille de calcul
extern s_sheet *sheet;

// Tableau des opérations disponibles
extern s_operation operations[];

// ===== Déclarations des fonctions =====

// Création et destruction
s_cell *cell_create(void);
void cell_destroy(s_cell *cell);
s_token *token_create(token_type type);
void token_destroy(s_token *token);




// Analyse de formule
void analyse_chaine_cellule(s_cell *cellule);
// Évaluation
double evaluate_cell(s_cell *cell);



// Opérateurs de base
void addition(my_stack_t *eval);
void soustraction(my_stack_t *eval);
void multiplication(my_stack_t *eval);
void division(my_stack_t *eval);
void modulo(my_stack_t *eval);
void cosinus(my_stack_t *eval);

// Utilitaires
s_cell *get_cell_by_reference(const char *ref);
int is_operator(const char *str);
int is_number(const char *str);
int is_cell_reference(const char *str);

#endif