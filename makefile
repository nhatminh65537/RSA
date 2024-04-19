CC=gcc

main: 	
	$(CC) -c main.c uilib.c ui.c prgvar.c cmd.c -g
	$(CC) -o main.exe ui.o main.o uilib.o prgvar.o cmd.o

launch:
	$(CC) -o launch launch.c