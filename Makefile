CFLAGS=-Wall -g

all: ex1 ex3 ex4 ex5 ex6 ex7 ex8 ex9 ex10 ex11 ex12 ex13 ex14 ex15 ex16 ex17 ex17_2 ex18 ex19 ex20 ex22_main ex23

test: ex19
	./ex19 < test_game.txt

ex19: object.o

ex22_main: ex22.o

minotaur: object.o game_engine.o 

clean:
	rm -rf ex1 ex3 ex4 ex5 ex6 ex7 ex8 ex9 ex10 ex11 ex12 ex13 ex14 ex15 ex16 ex17 ex17_2 ex18 ex19 ex20 ex22_main object.o ex22.o ex23
