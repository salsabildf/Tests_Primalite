# Variables ...................................................................:

## Compilateur ................................................................:

CC = gcc
CFLAGS = -g3 -Wall
LDFLAGS = -lm -lgmp

## Structure du projet ........................................................:

EXEC = bin/prima
SRC_PATH = src/
OBJ_PATH = out/
INC_PATH = lib/
REPORT_PATH = doc/
SRC = $(shell find $(SRC_PATH)*.c)
OBJ = $(SRC:$(SRC_PATH)%.c=$(OBJ_PATH)%.o)

## Autres .....................................................................:

ARGS =

## Lancement ..................................................................:

run : compil
	@./$(EXEC) $(ARGS)

## Compilation ................................................................:

compil : $(EXEC)

$(EXEC) : $(OBJ)
	@$(CC) $^ -o $(EXEC) $(LDFLAGS)

$(OBJ_PATH)%.o : $(SRC_PATH)%.c $(INC_PATH)%.h
	@$(CC) -c $< -o $@ $(CFLAGS)

## Nettoyage ..................................................................:

clean :
	$(info Suppression de $(EXEC) et de $(OBJ))
	@rm -f $(EXEC)
	@rm -f $(OBJ)

## Debugger ...................................................................:

gdb : compil
	$(info Debbugage avec $@)
	@gdb --args ./$(EXEC) $(ARGS)
