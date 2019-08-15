
all: scrabble

scrabble: BIN/Move.o BIN/Board.o BIN/Dictionary.o BIN/Player.o BIN/Bag.o BIN/Game.o BIN/ConsolePrinter.o BIN/Trie.o BIN/AI.o src/main.cpp
	g++ -g -Wall -std=c++11 $^ -o $@


BIN/%.o: src/%.cpp BIN/.dirstamp
	g++ -g -Wall -c -std=c++11 $< -o $@

BIN/.dirstamp:
	mkdir -p BIN
	touch BIN/.dirstamp

.PHONY: clean
clean:
	rm -rf scrabble
	rm -rf BIN
