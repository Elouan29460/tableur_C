# Makefile pour le jalon2

CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm

# Répertoires
JALON1_DIR = ../jalon1

# Fichiers sources
SOURCES = cellule.c stack.c $(JALON1_DIR)/liste.c
OBJECTS = cellule.o stack.o liste.o

# Fichiers de test
TEST_STACK = testStack
TEST_CELLULE = testCellule

all: $(TEST_STACK) $(TEST_CELLULE)

# Compilation du test de pile
$(TEST_STACK): testStack.o stack.o
	$(CC) -o $@ $^ $(LDFLAGS)

testStack.o: testStack.c stack.h
	$(CC) $(CFLAGS) -c testStack.c

# Compilation du test de cellule
$(TEST_CELLULE): testCellule.o cellule.o stack.o liste.o
	$(CC) -o $@ $^ $(LDFLAGS)

testCellule.o: testCellule.c cellule.h stack.h $(JALON1_DIR)/liste.h
	$(CC) $(CFLAGS) -c testCellule.c

cellule.o: cellule.c cellule.h stack.h $(JALON1_DIR)/liste.h
	$(CC) $(CFLAGS) -c cellule.c

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c

liste.o: $(JALON1_DIR)/liste.c $(JALON1_DIR)/liste.h
	$(CC) $(CFLAGS) -c $(JALON1_DIR)/liste.c

clean:
	rm -f *.o $(TEST_STACK) $(TEST_CELLULE)

.PHONY: all clean
