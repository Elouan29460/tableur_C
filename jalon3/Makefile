# Makefile pour le jalon3

CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm

# Répertoires
JALON1_DIR = ../jalon1

# Fichiers sources
SOURCES = cellule.c graphe.c stack.c $(JALON1_DIR)/liste.c
OBJECTS = cellule.o graphe.o stack.o liste.o

# Fichiers de test
TEST_STACK = testStack
TEST_GRAPHE = testGraphe

all: $(TEST_STACK) $(TEST_GRAPHE)

# Compilation du test de pile
$(TEST_STACK): testStack.o stack.o
	$(CC) -o $@ $^ $(LDFLAGS)

testStack.o: testStack.c stack.h
	$(CC) $(CFLAGS) -c testStack.c

# Compilation du test de graphe
$(TEST_GRAPHE): testGraphe.o cellule.o graphe.o stack.o liste.o
	$(CC) -o $@ $^ $(LDFLAGS)

testGraphe.o: testGraphe.c cellule.h stack.h $(JALON1_DIR)/liste.h
	$(CC) $(CFLAGS) -c testGraphe.c

cellule.o: cellule.c cellule.h stack.h $(JALON1_DIR)/liste.h
	$(CC) $(CFLAGS) -c cellule.c

graphe.o: graphe.c cellule.h $(JALON1_DIR)/liste.h
	$(CC) $(CFLAGS) -c graphe.c

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c

liste.o: $(JALON1_DIR)/liste.c $(JALON1_DIR)/liste.h
	$(CC) $(CFLAGS) -c $(JALON1_DIR)/liste.c

clean:
	rm -f *.o $(TEST_STACK) $(TEST_GRAPHE)

.PHONY: all clean
