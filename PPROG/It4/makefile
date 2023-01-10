FILES=command.o game.o game_loop.o graphic_engine.o screen.o space.o object.o player.o GameManagement.o set.o  inventory.o link.o 
FLAGS= -g -Wall -ansi -pedantic

.PHONY: all clean dist tidy

all: clean ocabas dist tidy

test: clean  set_test inventory_test space_test link_test object_test player_test tidy
	  echo  set_test inventory_test space_test link_test object_test executables created

command.o: command.c command.h
	gcc -c $(FLAGS) $<

game.o:	game.c game.h
	gcc -c $(FLAGS) $<

game_loop.o: game_loop.c
	gcc -c $(FLAGS) $<

graphic_engine.o: graphic_engine.c graphic_engine.h
	gcc -c $(FLAGS) $<

screen.o: screen.c screen.h
	gcc -c $(FLAGS) $<

space.o: space.c space.h link.h 
	gcc -c $(FLAGS) $<

object.o: object.c object.h
	gcc -c $(FLAGS) $<

object_test.o: object_test.c object.h
	gcc -c $(FLAGS) $<

player.o: player.c player.h
	gcc -c $(FLAGS) $<
	
GameManagement.o: GameManagement.c GameManagement.h
	gcc -c $(FLAGS) $<

set.o: set.c set.h
	gcc -c $(FLAGS) $<

set_test.o: set_test.c set.h
	gcc -c $(FLAGS) $<

inventory_test.o: inventory_test.c inventory.h
	gcc -c $(FLAGS) $<
	
link_test.o: link_test.c link.h
	gcc -c $(FLAGS) $<

player_test.o: player_test.c player.h
	gcc -c $(FLAGS) $<

space_test.o: space_test.c space.h set.h
	gcc -c $(FLAGS) $<

inventory.o: inventory.c inventory.h
	gcc -c $(FLAGS) $<

link.o: link.c link.h 
	gcc -c $(FLAGS) $<

ocabas: $(FILES)
	gcc $^ $(FLAGS) -o  $@

set_test: set_test.o set.o 
	gcc $^ $(FLAGS) -o $@

inventory_test: inventory_test.o inventory.o set.o
	gcc $^ $(FLAGS) -o $@

link_test: link_test.o link.o
	gcc $^ $(FLAGS) -o $@

object_test: object_test.o object.o
	gcc $^ $(FLAGS) -o $@

player_test: player_test.o player.o inventory.o set.o
	gcc $^ $(FLAGS) -o $@

space_test: space_test.o space.o set.o
	gcc $^ $(FLAGS) -o $@
clean:	
	rm -rf *.o ocabas
	rm -rf set_test inventory_test link_test

dist:
	tar -cvzf s1-cod_OcaBasicaIni-1.tgz *.c *.h

tidy: 
	rm -rf *.o 
runv:
	valgrind --leak-check=full -v ./ocabas data1.dat
doc:
	doxygen