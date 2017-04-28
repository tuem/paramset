CC = g++
CFLAGS = -std=c++11 -Wall
CINCLUDES = -I .. -I ../external/cmdline -I ../external/json/src
PARAMSET = ../paramset.hpp
SRC = paramset.cpp
BIN = $(subst .cpp,,$(SRC))
all: $(BIN)

example: $(BIN)
	./$(BIN) foo bar
	./$(BIN) -c config.json foo bar
	./$(BIN) -c config.json -s 'command line argument' -i 123 --doublearg 456.7 foo bar baz

$(BIN): $(SRC) $(PARAMSET)
	$(CC) $(CFLAGS) $(CINCLUDES) $(SRC) -o $(BIN)

clean:
	$(RM) $(BIN)

.PHONY: clean
