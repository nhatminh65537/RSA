all: int256.c genkey.c int256.h
	gcc -c int256.c genkey.c -Ofast
	gcc -lpthread int256.o genkey.o
