########################################################
CC=gcc
CFLAGS= -g -Wall -pedantic -pthread
FILES= servidor
OBJECTS= methods.o picohttpparser.o http.o
LIB= mylib
########################################################

all: clean mkdirs $(OBJECTS) $(LIB) $(FILES)

mkdirs:
	mkdir ./obj/ ./lib/

methods.o: ./srclib/methods.c
	$(CC) $(CFLAGS) -c ./srclib/methods.c -o ./obj/methods.o

picohttpparser.o: ./srclib/picohttpparser.c
	$(CC) $(CFLAGS) -c ./srclib/picohttpparser.c -o ./obj/picohttpparser.o

http.o: ./srclib/http.c
	$(CC) $(CFLAGS) -c ./srclib/http.c -o ./obj/http.o

mylib:
	ar rcs ./lib/lib$(LIB).a ./obj/methods.o ./obj/picohttpparser.o ./obj/http.o
	ranlib ./lib/lib$(LIB).a

servidor.o: ./src/servidor.c
	$(CC) $(CFLAGS) -c ./src/servidor.c -o ./obj/servidor.o

servidor: servidor.o
	$(CC) $(CFLAGS) ./obj/servidor.o -L./lib/ -l$(LIB) -o servidor

run:
	./servidor

clean:
	rm -rf ./lib/
	rm -rf ./obj/
	rm -rf $(FILES)
