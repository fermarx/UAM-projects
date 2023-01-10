CC = gcc
CFLAGS = -g -ansi -Wall -pedantic -ggdb -std=c99
IFLAGS=-I./
LDFLAGS=-L./
LDLIBS=-lib_afnd_64

all : clean lib_afnd_64.a main_de_ejemplo

transforma.o: transformav4.c transforma.h afnd.h
	$(CC) $(CFLAGS) -c $<


main_de_ejemplo.o: main_de_ejemplo.c
	$(CC) $(CFLAGS) -c $< $(IFLAGS)

main_de_ejemplo: main_de_ejemplo.o lib_afnd_64.a transforma.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS) 

.PHONY : clean
clean :
	rm -f *.o
