# Directories
SRC := ./src
OBJ := ./obj
BIN := ./bin

# Files
SRC_FILES := $(wildcard $(SRC)/*.c)
OBJ_FILES := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRC_FILES))
BIN_FILES := $(BIN)/shell

# Making Executable
all: $(OBJ_FILES)
	mkdir -p $(BIN)
	gcc -o $(BIN_FILES) $^

# Making Object Files
$(OBJ)/%.o: $(SRC)/%.c
	mkdir -p $(OBJ)
	gcc -o $@ -c $<

# Running Program
run: $(BIN_FILES)
	$(BIN_FILES)

# Cleaning Files
clean:
	rm -rf $(OBJ);
	rm -rf $(BIN);
