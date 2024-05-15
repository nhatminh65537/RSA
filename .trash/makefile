entest: encrypt.c 
	gcc -c encrypt.c -g
	gcc -o entest encrypt.o ../build/int256.o

detest: decrypt.c
	gcc -c decrypt.c -g
	gcc -o detest decrypt.o ../build/int256.o

gktest: genkey.c
	gcc -c genkey.c -Ofast
	gcc -lpthread -o gktest genkey.o ../build/int256.o 