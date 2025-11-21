#ifndef CELLULE_H
#define CELLULE_H

#include "../jalon1/liste.h"
#include "stack.h"

// Structure de base pour une cellule
typedef struct s_cell {
    char *chaine;              // Chaîne saisie par l'utilisateur
    double value;               // Valeur numérique calculée (0.0 si non calculable)
    node_t *tokens;            // Liste des jetons (token)
    node_t *dependencies;      // Liste des cellules qui dépendent de cette cellule
} s_cell;

// Types de jetons
typedef enum {
    VALUE,      // Constante numérique
    REF,        // Référence vers une autre cellule
    OPERATOR    // Opérateur
} token_type;

// Structure pour un jeton
typedef struct token {
    token_type type;
    union {
        double cst;                                    // Constante numérique
        s_cell *ref;                                   // Référence vers une cellule
        void (*operator)(my_stack_t *eval);           // Pointeur vers fonction opérateur
    } value;
} s_token;




// Structure pour définir une opération
typedef struct {
    char *name;                                       // Nom de l'opérateur ("+", "-", etc.)
    void (*function)(my_stack_t *eval);              // Pointeur vers la fonction
} s_operation;

// Structure pour la feuille de calcul
typedef struct {
    char *filename;             // Nom du fichier pour l'enregistrement
    int nb_rows;                // Nombre de lignes
    int nb_cols;                // Nombre de colonnes
    node_t *cells;             // Liste des cellules existantes
} s_sheet;

// Variable globale pour la feuille de calcul
extern s_sheet *sheet;

// Tableau des opérations disponibles
extern s_operation operations[];
extern int nb_operations;

// ===== Déclarations des fonctions =====

// Création et destruction
s_cell *cell_create(void);
void cell_destroy(s_cell *cell);
s_token *token_create(token_type type);
void token_destroy(s_token *token);

// Analyse de formule
node_t *parse_formula(const char *formula, s_sheet *sheet);

// Évaluation
double evaluate_cell(s_cell *cell);

// Opérateurs de base
void op_add(my_stack_t *eval);
void op_sub(my_stack_t *eval);
void op_mul(my_stack_t *eval);
void op_div(my_stack_t *eval);

// Utilitaires
s_cell *get_cell_by_reference(const char *ref, s_sheet *sheet);
int is_operator(const char *str);
int is_number(const char *str);
int is_cell_reference(const char *str);

#endif