CC = gcc
.PHONY: all clean

all: clean ejercicio_kill ejercicio_lect_escr ejercicio_prottemp ejercicio_prottemp_mejorado clear 



ejercicio_kill.o: ejercicio_kill.c
	$(CC) -c $<

ejercicio_lect_escr.o: ejercicio_lect_escr.c
	$(CC) -c $<

ejercicio_prottemp.o: ejercicio_prottemp.c
	$(CC) -c $<

ejercicio_prottemp_mejorado.o: ejercicio_prottemp_mejorado.c
	$(CC) -c $<


ejercicio_kill: ejercicio_kill.c
	$(CC) -o $@ $^

ejercicio_lect_escr: ejercicio_lect_escr.c
	$(CC) -pthread -o $@ $^

ejercicio_prottemp: ejercicio_prottemp.c
	$(CC) -pthread -o $@ $^

ejercicio_prottemp_mejorado: ejercicio_prottemp_mejorado.c
	$(CC) -pthread -o $@ $^


clean:
	rm -rf *.o ejercicio_kill ejercicio_lect_escr ejercicio_prottemp ejercicio_prottemp_mejorado

clear:
	rm -rf *.o
