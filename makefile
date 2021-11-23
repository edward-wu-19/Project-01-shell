SRC := src
BIN := bin
SRC_FILES := $(wildcard $(SRC)/*.c)
BIN_FILES := $(patsubst $(SRC)/%.c, $(BIN)/%.o, $(SRC_FILES))

all: $(BIN_FILES)
	gcc -o shell $^

$(BIN)/%.o: $(SRC)/%.c
	mkdir -p bin
	gcc -o $@ -c $<

run:
	./shell

clean:
	rm -rf bin
	rm shell
