# Compilateur et flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g
LDFLAGS = -lm

# Nom de l'exécutable
TARGET = prog

# Fichiers sources et objets
SRCS = testListe.c liste.c
OBJS = $(SRCS:.c=.o)
HEADERS = liste.h

# Règle par défaut
all: $(TARGET)

# Création de l'exécutable
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compilation des fichiers objets
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	del /Q $(OBJS) $(TARGET).exe 2>nul || echo Nettoyage termine

# Nettoyage complet
distclean: clean
	del /Q *~ *.bak 2>nul || echo Nettoyage complet termine

# Exécution du programme
run: $(TARGET)
	.\$(TARGET)

# Recompilation complète
rebuild: clean all

# Affichage des informations
info:
	@echo Sources: $(SRCS)
	@echo Objets: $(OBJS)
	@echo Executable: $(TARGET)

.PHONY: all clean distclean run rebuild info
