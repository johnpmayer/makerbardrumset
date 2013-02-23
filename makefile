
all: keyPlayer.c
	gcc -Wall -lncurses keyPlayer.c -o rock

pi: keyPlayer.c
	gcc -Wall -DPI -lncurses keyPlayer.c -o rock
