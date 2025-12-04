#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cellule.h"

void test_graphe_simple() {
    printf("\nTEST 1\n");
    printf("D3=10, A4=D3*2, A5=A4+D3, C5=A5*2\n\n");
    
    sheet = sheet_create(10, 10);
    
    s_cell *D3 = cell_create();
    cell_setStr(D3, "10");
    analyse_chaine_cellule(D3);
    feuille_setCell(D3, "D3");
    D3->value = evaluate_cell(D3);
    maj_dependances(D3);
    printf("D3 = %.2f\n", D3->value);
    
    s_cell *A4 = cell_create();
    cell_setStr(A4, "=D3 2 *");
    analyse_chaine_cellule(A4);
    feuille_setCell(A4, "A4");
    maj_dependances(A4);
    A4->value = evaluate_cell(A4);
    printf("A4 = D3*2 = %.2f\n", A4->value);
    
    s_cell *A5 = cell_create();
    cell_setStr(A5, "=A4 D3 +");
    analyse_chaine_cellule(A5);
    feuille_setCell(A5, "A5");
    maj_dependances(A5);
    A5->value = evaluate_cell(A5);
    printf("A5 = A4+D3 = %.2f\n", A5->value);
    
    s_cell *C5 = cell_create();
    cell_setStr(C5, "=A5 2 *");
    analyse_chaine_cellule(C5);
    feuille_setCell(C5, "C5");
    maj_dependances(C5);
    C5->value = evaluate_cell(C5);
    printf("C5 = A5*2 = %.2f\n", C5->value);
    
    printf("\n--- Modification de D3 à 5 ---\n");
    cell_setStr(D3, "5");
    analyse_chaine_cellule(D3);
    maj_dependances(D3);
    D3->value = evaluate_cell(D3);
    printf("D3 = %.2f\n", D3->value);
    
    evaluation_successeurs(D3);
    
    printf("A4 = %.2f (attendu: 10.00)\n", A4->value);
    printf("A5 = %.2f (attendu: 15.00)\n", A5->value);
    printf("C5 = %.2f (attendu: 30.00)\n", C5->value);
    
    sheet_destroy(sheet);
    sheet = NULL;
}

void test_graphe_diamant() {
    printf("\nTEST 2\n");
    printf("A1=10, B1=A1*2, C1=A1*3, D1=B1+C1\n\n");
    
    sheet = sheet_create(10, 10);
    
    s_cell *A1 = cell_create();
    cell_setStr(A1, "10");
    analyse_chaine_cellule(A1);
    feuille_setCell(A1, "A1");
    maj_dependances(A1);
    A1->value = evaluate_cell(A1);
    printf("A1 = %.2f\n", A1->value);
    
    s_cell *B1 = cell_create();
    cell_setStr(B1, "=A1 2 *");
    analyse_chaine_cellule(B1);
    feuille_setCell(B1, "B1");
    maj_dependances(B1);
    B1->value = evaluate_cell(B1);
    printf("B1 = A1*2 = %.2f\n", B1->value);
    
    s_cell *C1 = cell_create();
    cell_setStr(C1, "=A1 3 *");
    analyse_chaine_cellule(C1);
    feuille_setCell(C1, "C1");
    maj_dependances(C1);
    C1->value = evaluate_cell(C1);
    printf("C1 = A1*3 = %.2f\n", C1->value);
    
    s_cell *D1 = cell_create();
    cell_setStr(D1, "=B1 C1 +");
    analyse_chaine_cellule(D1);
    feuille_setCell(D1, "D1");
    maj_dependances(D1);
    D1->value = evaluate_cell(D1);
    printf("D1 = B1+C1 = %.2f\n", D1->value);
    
    printf("\n--- Modification de A1 à 20 ---\n");
    cell_setStr(A1, "20");
    analyse_chaine_cellule(A1);
    maj_dependances(A1);
    A1->value = evaluate_cell(A1);
    printf("A1 = %.2f\n", A1->value);
    
    evaluation_successeurs(A1);
    
    printf("B1 = %.2f (attendu: 40.00)\n", B1->value);
    printf("C1 = %.2f (attendu: 60.00)\n", C1->value);
    printf("D1 = %.2f (attendu: 100.00)\n", D1->value);
    
    sheet_destroy(sheet);
    sheet = NULL;
}

void test_graphe_chaine() {
    printf("\nTEST 3\n");
    printf("A1=1, B1=A1+1, C1=B1+1, D1=C1+1, E1=D1+1\n\n");
    
    sheet = sheet_create(10, 10);
    
    s_cell *A1 = cell_create();
    cell_setStr(A1, "1");
    analyse_chaine_cellule(A1);
    feuille_setCell(A1, "A1");
    maj_dependances(A1);
    A1->value = evaluate_cell(A1);
    
    s_cell *B1 = cell_create();
    cell_setStr(B1, "=A1 1 +");
    analyse_chaine_cellule(B1);
    feuille_setCell(B1, "B1");
    maj_dependances(B1);
    B1->value = evaluate_cell(B1);
    
    s_cell *C1 = cell_create();
    cell_setStr(C1, "=B1 1 +");
    analyse_chaine_cellule(C1);
    feuille_setCell(C1, "C1");
    maj_dependances(C1);
    C1->value = evaluate_cell(C1);
    
    s_cell *D1 = cell_create();
    cell_setStr(D1, "=C1 1 +");
    analyse_chaine_cellule(D1);
    feuille_setCell(D1, "D1");
    maj_dependances(D1);
    D1->value = evaluate_cell(D1);
    
    s_cell *E1 = cell_create();
    cell_setStr(E1, "=D1 1 +");
    analyse_chaine_cellule(E1);
    feuille_setCell(E1, "E1");
    maj_dependances(E1);
    E1->value = evaluate_cell(E1);
    
    printf("A1=%0.f, B1=%.0f, C1=%.0f, D1=%.0f, E1=%.0f\n", 
           A1->value, B1->value, C1->value, D1->value, E1->value);
    
    printf("\n--- Modification de A1 à 10 ---\n");
    cell_setStr(A1, "10");
    analyse_chaine_cellule(A1);
    maj_dependances(A1);
    A1->value = evaluate_cell(A1);
    
    evaluation_successeurs(A1);
    
    printf("A1=%0.f, B1=%.0f, C1=%.0f, D1=%.0f, E1=%.0f\n", 
           A1->value, B1->value, C1->value, D1->value, E1->value);
    printf("(attendu: 10, 11, 12, 13, 14)\n");
    
    sheet_destroy(sheet);
    sheet = NULL;
}

int main(void) {    
    test_graphe_simple();
    test_graphe_diamant();
    test_graphe_chaine();
    return 0;
}
