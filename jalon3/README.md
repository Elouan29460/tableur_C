# Jalon 3 : Graphe des cellules

## Description

Ce jalon implémente la gestion du graphe de dépendances entre cellules et l'algorithme de tri topologique pour réévaluer efficacement les cellules lorsqu'une cellule est modifiée.

## Structure du graphe

- **Sommets** : Les cellules
- **Arcs** : Les dépendances entre cellules
- **Prédécesseurs** : Cellules référencées dans une formule (liste `dependencies`)
- **Successeurs** : Cellules qui référencent cette cellule (liste `successors`)

## Fonctionnalités

### Gestion du graphe

- `add_successor(predecessor, successor)` : Ajoute une relation de succession
- `remove_successor(predecessor, successor)` : Retire une relation de succession
- `update_dependencies(cell)` : Met à jour les dépendances après modification d'une formule

### Tri topologique

- `reevaluate_successors(cell)` : Réévalue tous les successeurs d'une cellule en utilisant le tri topologique
- `compute_negative_degree(cell, subgraph)` : Calcule le degré négatif (nombre de prédécesseurs dans le sous-graphe)

## Algorithme de réévaluation

Lorsqu'une cellule est modifiée :

1. **Construction du sous-graphe** : On identifie tous les successeurs directs et indirects
2. **Calcul des degrés** : Pour chaque cellule du sous-graphe, on calcule son degré négatif
3. **Tri topologique** : 
   - On commence par les cellules de degré 0 (pas de prédécesseurs dans le sous-graphe)
   - On évalue la cellule
   - On décrémente le degré de ses successeurs
   - Quand un degré atteint 0, on ajoute la cellule à la liste d'évaluation

Cet algorithme garantit qu'une cellule n'est évaluée qu'une seule fois et seulement après que tous ses prédécesseurs ont été évalués.

## Compilation et tests

```bash
make
./testGraphe
```

## Tests

Le fichier `testGraphe.c` contient 3 tests :

1. **Graphe simple** : D3 → A4 → A5 → C5
2. **Graphe en diamant** : A1 → B1,C1 → D1
3. **Chaîne de dépendances** : A1 → B1 → C1 → D1 → E1

## Exemple

```
D3 = 10
A4 = D3 * 2      (= 20)
A5 = A4 + D3     (= 30)
C5 = A5 * 2      (= 60)

Si on modifie D3 = 5, alors :
- A4 = 10
- A5 = 15  
- C5 = 30
```

Le tri topologique s'assure que A4 est évalué avant A5, et A5 avant C5.
