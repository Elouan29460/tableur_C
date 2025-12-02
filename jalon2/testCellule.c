#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cellule.h"

// Fonction utilitaire pour afficher un token
void print_token(s_token *token) {
    if (token == NULL) {
        printf("NULL");
        return;
    }
    
    switch (token->type) {
        case VALUE:
            printf("VALUE(%.2f)", token->value.cst);
            break;
        case REF:
            printf("REF(%p)", (void*)token->value.ref);
            break;
        case OPERATOR:
            printf("OPERATOR(%p)", (void*)token->value.operator);
            break;
    }
}

// Fonction utilitaire pour afficher une liste de tokens
void print_token_list(node_t *tokens) {
    printf("Tokens: [");
    node_t *current = tokens;
    int first = 1;
    while (current != NULL) {
        if (!first) printf(", ");
        print_token((s_token *)list_get_data(current));
        first = 0;
        current = list_next(current);
    }
    printf("]\n");
}

// Test 1: Parsing de nombres
void test_parse_numbers() {
    printf("\n=== Test 1: Parsing de nombres ===\n");
    
    const char *formulas[] = {
        "42",
        "3.14",
        "-5.5",
        "10 20 30"
    };
    
    for (int i = 0; i < 4; i++) {
        printf("Formule: \"%s\"\n", formulas[i]);
        s_cell *temp = cell_create();
        temp->chaine = strdup(formulas[i]);
        analyse_chaine_cellule(temp);
        print_token_list(temp->tokens);
        cell_destroy(temp);
    }
}

// Test 2: Parsing d'opérateurs
void test_parse_operators() {
    printf("\n=== Test 2: Parsing d'opérateurs ===\n");
    
    const char *formulas[] = {
        "=5 3 +",
        "=10 2 -",
        "=4 5 *",
        "=20 4 /"
    };
    
    for (int i = 0; i < 4; i++) {
        printf("Formule: \"%s\"\n", formulas[i]);
        s_cell *temp = cell_create();
        temp->chaine = strdup(formulas[i]);
        analyse_chaine_cellule(temp);
        print_token_list(temp->tokens);
        cell_destroy(temp);
    }
}

// Test 3: Parsing de formules complexes
void test_parse_complex() {
    printf("\n=== Test 3: Parsing de formules complexes ===\n");
    
    const char *formulas[] = {
        "=5 3 + 2 *",
        "=10 2 / 3 -",
        "=1.5 2.5 + 3 *"
    };
    
    for (int i = 0; i < 3; i++) {
        printf("Formule: \"%s\"\n", formulas[i]);
        s_cell *temp = cell_create();
        temp->chaine = strdup(formulas[i]);
        analyse_chaine_cellule(temp);
        print_token_list(temp->tokens);
        cell_destroy(temp);
    }
}

// Test 4: Évaluation de cellules simples
void test_evaluate_simple() {
    printf("\n=== Test 4: Évaluation de cellules simples ===\n");
    
    // Test: 5 3 + (doit donner 8)
    s_cell *cell1 = cell_create();
    cell1->chaine = strdup("=5 3 +");
    analyse_chaine_cellule(cell1);
    double result1 = evaluate_cell(cell1);
    printf("Formule: \"%s\" => Résultat: %.2f (attendu: 8.00)\n", cell1->chaine, result1);
    cell_destroy(cell1);
    
    // Test: 10 2 - (doit donner 8)
    s_cell *cell2 = cell_create();
    cell2->chaine = strdup("=10 2 -");
    analyse_chaine_cellule(cell2);
    double result2 = evaluate_cell(cell2);
    printf("Formule: \"%s\" => Résultat: %.2f (attendu: 8.00)\n", cell2->chaine, result2);
    cell_destroy(cell2);
    
    // Test: 4 5 * (doit donner 20)
    s_cell *cell3 = cell_create();
    cell3->chaine = strdup("=4 5 *");
    analyse_chaine_cellule(cell3);
    double result3 = evaluate_cell(cell3);
    printf("Formule: \"%s\" => Résultat: %.2f (attendu: 20.00)\n", cell3->chaine, result3);
    cell_destroy(cell3);
    
    // Test: 20 4 / (doit donner 5)
    s_cell *cell4 = cell_create();
    cell4->chaine = strdup("=20 4 /");
    analyse_chaine_cellule(cell4);
    double result4 = evaluate_cell(cell4);
    printf("Formule: \"%s\" => Résultat: %.2f (attendu: 5.00)\n", cell4->chaine, result4);
    cell_destroy(cell4);
}

// Test 5: Évaluation de formules complexes
void test_evaluate_complex() {
    printf("\n=== Test 5: Évaluation de formules complexes ===\n");
    
    // Test: 5 3 + 2 * (doit donner 16: (5+3)*2)
    s_cell *cell1 = cell_create();
    cell1->chaine = strdup("=5 3 + 2 *");
    analyse_chaine_cellule(cell1);
    double result1 = evaluate_cell(cell1);
    printf("Formule: \"%s\" => Résultat: %.2f (attendu: 16.00)\n", cell1->chaine, result1);
    cell_destroy(cell1);
    
    // Test: 10 2 / 3 - (doit donner 2: 10/2-3 = 5-3)
    s_cell *cell2 = cell_create();
    cell2->chaine = strdup("=10 2 / 3 -");
    analyse_chaine_cellule(cell2);
    double result2 = evaluate_cell(cell2);
    printf("Formule: \"%s\" => Résultat: %.2f (attendu: 2.00)\n", cell2->chaine, result2);
    cell_destroy(cell2);
    
    // Test: 1.5 2.5 + 3 * (doit donner 12: (1.5+2.5)*3 = 4*3)
    s_cell *cell3 = cell_create();
    cell3->chaine = strdup("=1.5 2.5 + 3 *");
    analyse_chaine_cellule(cell3);
    double result3 = evaluate_cell(cell3);
    printf("Formule: \"%s\" => Résultat: %.2f (attendu: 12.00)\n", cell3->chaine, result3);
    cell_destroy(cell3);
}

// Test 6: Tests des fonctions utilitaires
void test_utilities() {
    printf("\n=== Test 6: Tests des fonctions utilitaires ===\n");
    
    // Test is_number
    printf("is_number(\"42\"): %d (attendu: 1)\n", is_number("42"));
    printf("is_number(\"3.14\"): %d (attendu: 1)\n", is_number("3.14"));
    printf("is_number(\"-5.5\"): %d (attendu: 1)\n", is_number("-5.5"));
    printf("is_number(\"abc\"): %d (attendu: 0)\n", is_number("abc"));
    
    // Test is_operator
    printf("is_operator(\"+\"): %d (attendu: 1)\n", is_operator("+"));
    printf("is_operator(\"-\"): %d (attendu: 1)\n", is_operator("-"));
    printf("is_operator(\"*\"): %d (attendu: 1)\n", is_operator("*"));
    printf("is_operator(\"/\"): %d (attendu: 1)\n", is_operator("/"));
    printf("is_operator(\"x\"): %d (attendu: 0)\n", is_operator("x"));
    
    // Test is_cell_reference
    printf("is_cell_reference(\"A1\"): %d (attendu: 1)\n", is_cell_reference("A1"));
    printf("is_cell_reference(\"B10\"): %d (attendu: 1)\n", is_cell_reference("B10"));
    printf("is_cell_reference(\"Z99\"): %d (attendu: 1)\n", is_cell_reference("Z99"));
    printf("is_cell_reference(\"AA23\"): %d (attendu: 0)\n", is_cell_reference("AA23"));
    printf("is_cell_reference(\"1A\"): %d (attendu: 0)\n", is_cell_reference("1A"));
    printf("is_cell_reference(\"ABC\"): %d (attendu: 0)\n", is_cell_reference("ABC"));
}

// Test 7: Test avec références de cellules
void test_cell_references() {
    printf("\n=== Test 7: Test avec références de cellules ===\n");
    
    // Créer deux cellules
    s_cell *cellA1 = cell_create();
    cellA1->chaine = strdup("10");
    analyse_chaine_cellule(cellA1);
    cellA1->value = evaluate_cell(cellA1);
    printf("Cellule A1: \"%s\" = %.2f\n", cellA1->chaine, cellA1->value);
    
    s_cell *cellB1 = cell_create();
    cellB1->chaine = strdup("5");
    analyse_chaine_cellule(cellB1);
    cellB1->value = evaluate_cell(cellB1);
    printf("Cellule B1: \"%s\" = %.2f\n", cellB1->chaine, cellB1->value);
    
    // Créer une cellule qui référence A1 et B1
    // Note: Pour l'instant, get_cell_by_reference retourne NULL
    // Ce test sera complété quand la gestion de la feuille sera implémentée
    s_cell *cellC1 = cell_create();
    cellC1->chaine = strdup("=A1 B1 +");
    analyse_chaine_cellule(cellC1);
    
    // Manuellement injecter les références pour le test
    node_t *current = cellC1->tokens;
    int pos = 0;
    while (current != NULL) {
        s_token *token = (s_token *)list_get_data(current);
        if (token->type == REF) {
            if (pos == 0) token->value.ref = cellA1;
            else if (pos == 1) token->value.ref = cellB1;
            pos++;
        }
        current = list_next(current);
    }
    
    double result = evaluate_cell(cellC1);
    printf("Cellule C1: \"%s\" = %.2f (attendu: 15.00)\n", cellC1->chaine, result);
    
    cell_destroy(cellA1);
    cell_destroy(cellB1);
    cell_destroy(cellC1);
}

int main(void) {
    printf("=== Tests du module cellule ===\n");
    
    test_utilities();
    test_parse_numbers();
    test_parse_operators();
    test_parse_complex();
    test_evaluate_simple();
    test_evaluate_complex();
    test_cell_references();
    
    printf("\n=== Fin des tests ===\n");
    return 0;
}