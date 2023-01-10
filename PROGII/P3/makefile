CC = gcc
CFLAGS = -Wall -ansi -pedantic -ggdb
IFLAGS=-I./
LDFLAGS=-L./
LDLIBS=-lstack_fp
OBJECTS1 = p3_e1.o queue_fp.o node.o graph.o
OBJECTS2 = p3_e2.o queue_fp.o node.o graph.o
OBJECTS3 = p3_e3.o queue_fp.o node.o graph.o
OBJECTS4 = p3_e3_c.o libstack_fp.a node.o graph.o

.PHONY: all clean

all: clean libstack_fp.a p3_e1 p3_e2 p3_e3 p3_e3_c clear 

node.o: node.c node.h types.h
	$(CC) $(CFLAGS) -c $<
graph.o: graph.c types.h graph.h node.h
	$(CC) $(CFLAGS) -c $<
stack.o: stack.c stack.h element.h types.h 
	$(CC) $(CFLAGS) -c $<
element.o: element.c element.h types.h
	$(CC) $(CFLAGS) -c $<
queue.o: queue_fp.c queue_fp.h types.h
	$(CC) $(CFLAGS) -c $<


p3_e1.o: p3_e1.c
	$(CC) $(CFLAGS) -c $< 
p3_e1: $(OBJECTS1)
	$(CC) $(CFLAGS) -o $@ $^

p3_e2.o: p3_e2.c
	$(CC) $(CFLAGS) -c $< 
p3_e2: $(OBJECTS2)
	$(CC) $(CFLAGS) -o $@ $^

p3_e3.o: p3_e3.c
	$(CC) $(CFLAGS) -c $< 
p3_e3: $(OBJECTS3)
	$(CC) $(CFLAGS) -o $@ $^

p3_e3_c.o: p3_e3_c.c
	$(CC) $(CFLAGS) -c $< $(IFLAGS)
p3_e3_c: $(OBJECTS4)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS) $(LIBS)

clean:
	rm -rf *.o p3_e1 p3_e2 p3_e3 p3_e3_c

clear:
	rm -rf *.o