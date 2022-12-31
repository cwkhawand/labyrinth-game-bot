# options de compilation
CC=gcc
CCFLAGS=-Wall
LIBS=-lm
LIBSDIR=
BUILD_DIR=bin
SRC_DIR=src
LIB_DIR=lib

# fichiers du projet
SRC=main.c $(SRC_DIR)/Labyrinth.c $(LIB_DIR)/Labyrinth/clientAPI.c $(LIB_DIR)/Labyrinth/labyrinthAPI.c
OBJ=$(SRC:.c=.o)
EXEC=Labyrinth

# règle initiale
all: $(EXEC)
	make run

run:
	$(BUILD_DIR)/$(EXEC)

clientAPI.o: $(LIB_DIR)/Labyrinth/clientAPI.h $(LIB_DIR)/Labyrinth/clientAPI.c
labyrinthAPI.o: $(LIB_DIR)/Labyrinth/labyrinthAPI.h $(LIB_DIR)/Labyrinth/labyrinthAPI.c $(LIB_DIR)/Labyrinth/clientAPI.h
Labyrinth.o: $(SRC_DIR)/Labyrinth.h $(SRC_DIR)/Labyrinth.c $(LIB_DIR)/Labyrinth/labyrinthAPI.h
main.o: main.c $(LIB_DIR)/Labyrinth/labyrinthAPI.h $(SRC_DIR)/Labyrinth.h

# règles de compilation
%.o: %.c
	$(CC) $(CCFLAGS) -o $(patsubst %,$(BUILD_DIR)/%,$(notdir $@)) -c $<

# règles d'édition de liens
$(EXEC): $(OBJ)
	$(CC) $(CCFLAGS) -o $(BUILD_DIR)/$@ $(patsubst %,$(BUILD_DIR)/%,$(notdir $^)) $(LIBS) $(LIBSDIR)

# règles supplémentaires
clean:
	rm -f $(BUILD_DIR)/$(EXEC) $(BUILD_DIR)/*.o