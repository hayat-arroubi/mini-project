# Makefile pour le simulateur d'ordonnancement de processus

CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -ldl

# Répertoires
SRC_DIR = .
POLICIES_DIR = policies
OBJ_DIR = obj
BIN_DIR = bin

# Fichiers sources
MAIN_SRC = $(SRC_DIR)/main.c
CORE_SRC = $(SRC_DIR)/process.c $(SRC_DIR)/parser.c $(SRC_DIR)/scheduler.c
POLICIES_SRC = $(wildcard $(POLICIES_DIR)/*.c)

# Fichiers objets
MAIN_OBJ = $(MAIN_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CORE_OBJ = $(CORE_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
POLICIES_OBJ = $(POLICIES_SRC:$(POLICIES_DIR)/%.c=$(OBJ_DIR)/policies/%.o)

# Exécutable
EXEC = $(BIN_DIR)/scheduler

# Règle par défaut
all: $(EXEC)

# Création de l'exécutable
$(EXEC): $(MAIN_OBJ) $(CORE_OBJ) $(POLICIES_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compilation des fichiers sources principaux
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compilation des fichiers sources des politiques
$(OBJ_DIR)/policies/%.o: $(POLICIES_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/policies
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Reconstruction complète
rebuild: clean all

# Exécution avec un fichier de configuration par défaut
run: $(EXEC)
	./$(EXEC) config.txt

.PHONY: all clean rebuild run