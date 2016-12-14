all:
	gcc -fopenmp -o multLow MultithreadedPokemonLowCores.c -lm
	gcc -fopenmp -o mult MultithreadedPokemon.c -lm