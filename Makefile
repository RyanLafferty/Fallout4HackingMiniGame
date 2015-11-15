all: falloutHackingGame

falloutHackingGame: falloutHackingGame.c
	gcc falloutHackingGame.c -o falloutHackingGame

.PHONY: clean
clean:
	rm falloutHackingGame
