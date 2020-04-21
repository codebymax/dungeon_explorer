dungeonGenerator: dungeonGenerator.cpp
		   g++ dungeonGenerator.cpp heap.c -o dungeonGenerator -g -lm -l ncurses
clean:
	rm dungeonGenerator
#-Wall -Werror
