CC=gcc
CFLAGS=-Wall -std=c99
SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c,out/%.o,$(SRC))
FOLDER=out
EXE=compiler

all: $(EXE)

$(OBJ): $(FOLDER)/%.o : src/%.c
	$(CC) $(CFLAGS) -o $@ -c $^

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(EXE) $(FOLDER)/*.o  *~

cleanbin:
	rm -f generated/*.bin

cleanall:
	rm -f $(EXE) $(FOLDER)/*.o  *~
	rm -f generated/*.bin
