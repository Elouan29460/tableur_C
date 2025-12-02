#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cellule.h"

// Test 1: Test des fonctions cell_setStr et feuille_setCell
void test_set_functions() {
    printf("\n=== Test 1: Fonctions cell_setStr et feuille_setCell ===\n");
    
    // Créer une feuille 10x10
    sheet = sheet_create(10, 10);
    if (sheet == NULL) {
        printf("ERREUR: Impossible de créer la feuille\n");
        return;
    }
    
    // Test cell_setStr
    s_cell *cell1 = cell_create();
    cell_setStr(cell1, "42");
    printf("Cellule après cell_setStr(\"42\"): chaine=\"%s\"\n", cell1->chaine);
    
    // Changer la valeur
    cell_setStr(cell1, "100");
    printf("Cellule après cell_setStr(\"100\"): chaine=\"%s\"\n", cell1->chaine);
    
    // Test feuille_setCell
    feuille_setCell(cell1, "A1");
    printf("Cellule placée en A1\n");
    
    s_cell *retrieved = sheet->cells_2d[0][0];
    if (retrieved == cell1) {
        printf("✓ Cellule bien récupérée depuis A1: chaine=\"%s\"\n", retrieved->chaine);
    }
    
    sheet_destroy(sheet);
    sheet = NULL;
}

// Test 2: Évaluation simple
void test_evaluation() {
    printf("\n=== Test 2: Évaluation de formules ===\n");
    
    sheet = sheet_create(10, 10);
    
    // Créer A1 = 10
    s_cell *cellA1 = cell_create();
    cell_setStr(cellA1, "10");
    analyse_chaine_cellule(cellA1);
    cellA1->value = evaluate_cell(cellA1);
    feuille_setCell(cellA1, "A1");
    printf("A1 = %.2f\n", cellA1->value);
    
    // Créer B1 = 5
    s_cell *cellB1 = cell_create();
    cell_setStr(cellB1, "5");
    analyse_chaine_cellule(cellB1);
    cellB1->value = evaluate_cell(cellB1);
    feuille_setCell(cellB1, "B1");
    printf("B1 = %.2f\n", cellB1->value);
    
    // Créer C1 = A1 + B1
    s_cell *cellC1 = cell_create();
    cell_setStr(cellC1, "=A1 B1 +");
    analyse_chaine_cellule(cellC1);
    feuille_setCell(cellC1, "C1");
    cellC1->value = evaluate_cell(cellC1);
    printf("C1 = A1 + B1 = %.2f (attendu: 15.00)\n", cellC1->value);
    
    sheet_destroy(sheet);
    sheet = NULL;
}

// Test 3: Opérations diverses
void test_operators() {
    printf("\n=== Test 3: Test des opérateurs ===\n");
    
    // Créer une feuille pour les tests avec références
    sheet = sheet_create(10, 10);

    s_cell *cellB1 = cell_create();
    cell_setStr(cellB1, "17.88");
    analyse_chaine_cellule(cellB1);
    cellB1->value = evaluate_cell(cellB1);
    feuille_setCell(cellB1, "B1");

    s_cell *cellZ10 = cell_create();
    cell_setStr(cellZ10, "6.66666");
    analyse_chaine_cellule(cellZ10);
    cellZ10->value = evaluate_cell(cellZ10);
    feuille_setCell(cellZ10, "Z10");

    struct {
        const char *formula;
        double expected;
    } tests[] = {
        {"=5 3 +", 8.0},
        {"=10 2 -", 8.0},
        {"=4 5 *", 20.0},
        {"=20 4 /", 5.0},
        {"=10 3 %", 1.0},
        {"=10 10 + 10 *", 200.0},
        {"=1 cos", 0.5403},
        {"=10.5 10.5 * 10.5 * 10.5 * 10.5 *", 127628.15},
        {"=2 cos 4 cos +", -1.07},
        {"=B1 B1 +",35.76},
        {"=Z10 cos", 0.99},
        {"=D2 10 +", 10.0},
        {"poulêt roti", 0.0},
        {"", 0.0}
    };
    
    for (int i = 0; i < 14; i++) {
        s_cell *cell = cell_create();
        cell_setStr(cell, tests[i].formula);
        analyse_chaine_cellule(cell);
        double result = evaluate_cell(cell);
        printf("%-15s => %.2f (attendu: %.2f)\n", 
               tests[i].formula, result, tests[i].expected);
        cell_destroy(cell);
    }
    
    sheet_destroy(sheet);
    sheet = NULL;
}

int main(void) {
    printf("=== Tests du module cellule ===\n");
    
    test_set_functions();
    test_evaluation();
    test_operators();
    
    printf("\n=== Fin des tests ===\n");
    return 0;
}