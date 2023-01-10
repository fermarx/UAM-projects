CC = gcc
CFLAGS = -Wall -ansi -pedantic
OBJECTS1 = node.o p1_e1.o
OBJECTS2 = node.o graph.o p1_e2.o
OBJECTS3 = node.o graph.o p1_e3.o

.PHONY: all clean

all: clean p1_e1 p1_e2 p1_e3 clear 

node.o: node.c node.h types.h
	$(CC) $(CFLAGS) -c $<

graph.o: graph.c graph.h types.h
	$(CC) $(CFLAGS) -c $<

p1_e1.o: p1_e1.c node.h types.h
	$(CC) $(CFLAGS) -c $<

p1_e2.o: p1_e2.c graph.h node.h types.h
	$(CC) $(CFLAGS) -c $<

p1_e3.o: p1_e3.c graph.h node.h types.h
	$(CC) $(CFLAGS) -c $<

p1_e1: $(OBJECTS1)
	$(CC) $(CFLAGS) -o $@ $^

p1_e2: $(OBJECTS2)
	$(CC) $(CFLAGS) -o $@ $^

p1_e3: $(OBJECTS3)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o node p1_e1 p1_e2 p1_e3

clear:
	rm -rf *.o
