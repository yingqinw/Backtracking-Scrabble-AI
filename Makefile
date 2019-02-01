
all: scrabble

scrabble: scrabble.cpp Player.o Bag.o Dictionary.o Move.o Board.o ConsolePrinter.o AI.o Trie.o
	g++ -g -Wall -std=c++11 scrabble.cpp Player.o Bag.o Dictionary.o Move.o Board.o ConsolePrinter.o AI.o Trie.o -o scrabble

Player.o: Player.cpp
	g++ -g -Wall -std=c++11 -c Player.cpp -o Player.o

Bag.o: Bag.cpp
	g++ -g -Wall -std=c++11 -c Bag.cpp -o Bag.o

Dictionary.o: Dictionary.cpp
	g++ -g -Wall -std=c++11 -c Dictionary.cpp -o Dictionary.o

Move.o: Move.cpp
	g++ -g -Wall -std=c++11 -c Move.cpp -o Move.o

Board.o: Board.cpp
	g++ -g -Wall -std=c++11 -c Board.cpp -o Board.o

ConsolePrinter.o: ConsolePrinter.cpp
	g++ -g -Wall -std=c++11 -c ConsolePrinter.cpp -o ConsolePrinter.o

AI.o: AI.cpp
	g++ -g -Wall -std=c++11 -c AI.cpp -o AI.o

Trie.o: Trie.cpp
	g++ -g -Wall -std=c++11 -c Trie.cpp -o Trie.o




