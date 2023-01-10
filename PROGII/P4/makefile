CC = gcc
CFLAGS = -Wall -ansi -pedantic -ggdb
IFLAGS=-I./
LDFLAGS=-L./
LDLIBS= -llist -lstack_fp
OBJECTS1 = p4_e1.o libstack_fp.a liblist.a
OBJECTS2 = p4_e2.o graph_list.o node.o libstack_fp.a liblist.a
OBJECTS3 = p4_e3_a.o graph_list.o node.o tree.o libstack_fp.a liblist.a
OBJECTS4 = p4_e3_b.o tree.o libstack_fp.a liblist.a

.PHONY: all clean

all: clean libstack_fp.a liblist.a p4_e1 p4_e2 p4_e3_a p4_e3_b clear 

node.o: node.c node.h types.h
	$(CC) $(CFLAGS) -c $<
graph_list.o: graph_list.c types.h graph.h node.h
	$(CC) $(CFLAGS) -c $<
tree.o: tree.c tree.h types.h
	$(CC) $(CFLAGS) -c $<

p4_e1.o: p4_e1.c
	$(CC) $(CFLAGS) -c $< $(IFLAGS)
p4_e1: $(OBJECTS1)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS) $(LIBS)

p4_e2.o: p4_e2.c
	$(CC) $(CFLAGS) -c $< $(IFLAGS)
p4_e2: $(OBJECTS2)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS) $(LIBS)

p4_e3_a.o: p4_e3_a.c
	$(CC) $(CFLAGS) -c $< $(IFLAGS)
p4_e3_a: $(OBJECTS3)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS) $(LIBS)

p4_e3_b.o: p4_e3_b.c
	$(CC) $(CFLAGS) -c $< $(IFLAGS)
p4_e3_b: $(OBJECTS4)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS) $(LIBS)

clean:
	rm -rf *.o p4_e1 p4_e2 p4_e3_a p4_e3_b

clear:
	rm -rf *.o