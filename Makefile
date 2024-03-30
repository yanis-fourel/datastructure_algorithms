
SRC := src/vec32i.c src/main.c

BIN := build/a.out

run: all
	@echo "--- Running binary ---"
	@$(BIN)


all:
	$(CC) $(SRC) -I include/ -o $(BIN)

bear:
	bear -- make all

.PHONY: all run
