CC = gcc
CFLAGS = -Wall -ansi -pedantic -ggdb
IFLAGS=-I./
LDFLAGS=-L./
LDLIBS=-lstack_fp
OBJECTS1 = p2_e0_a.o libstack_fp.a
OBJECTS2 = node.o p2_e0_b.o libstack_fp.a
OBJECTS3 = node.o p2_e1.o libstack_fp.a
OBJECTS4 = node.o p2_e2.o libstack_fp.a
OBJECTS5 = node.o p2_e3_a.o stack.o element.o
OBJECTS6 = node.o p2_e3_b.o stack.o element.o

.PHONY: all clean

all: clean libstack_fp.a p2_e0_a p2_e0_b p2_e1 p2_e2 p2_e3_a p2_e3_b clear 

node.o: node.c node.h types.h
	$(CC) $(CFLAGS) -c $<
graph.o: graph.c types.h graph.h node.h
	$(CC) $(CFLAGS) -c $<
stack.o: stack.c stack.h types.h element.h types.h
	$(CC) $(CFLAGS) -c $<
element.o: element.c element.h types.h
	$(CC) $(CFLAGS) -c $<



p2_e0_a.o: p2_e0_a.c
	$(CC) $(CFLAGS) -c $< $(IFLAGS)
p2_e0_a: $(OBJECTS1)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS) $(LIBS)

p2_e0_b.o: p2_e0_b.c
	$(CC) $(CFLAGS) -c $< $(IFLAGS)
p2_e0_b: $(OBJECTS2)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS) $(LIBS)



p2_e1.o: p2_e1.c
	$(CC) $(CFLAGS) -c $< $(IFLAGS)
p2_e1: $(OBJECTS3)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS) $(LIBS)



p2_e2.o: p2_e2.c
	$(CC) $(CFLAGS) -c $< $(IFLAGS)
p2_e2: $(OBJECTS4)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS) $(LIBS)


p2_e3_a.o: p2_e3_a.c
	$(CC) $(CFLAGS) -c $<
p2_e3_a: $(OBJECTS5)
	$(CC) $(CFLAGS) -o $@ $^

p2_e3_b.o: p2_e3_b.c
	$(CC) $(CFLAGS) -c $< 
p2_e3_b: $(OBJECTS6)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o node p2_e0_a p2_e0_b p2_e1 p2_e2 p2_e3_a p2_e3_b

clear:
	rm -rf *.o