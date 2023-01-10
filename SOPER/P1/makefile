CC = gcc
.PHONY: all clean

all: clean ejemplo_buffer ejemplo_descriptores ejemplo_fork ejemplo_hilos ejemplo_malloc ejemplo_pipe ejercicio_arbol ejercicio_hilos ejercicio_pipes ejercicio_shell clear 

ejemplo_buffer.o: ejemplo_buffer.c
	$(CC) -c $<

ejemplo_descriptores.o: ejemplo_descriptores.c
	$(CC) -c $<

ejemplo_fork.o: ejemplo_fork.c
	$(CC) -c $<

ejemplo_hilos.o: ejemplo_hilos.c
	$(CC) -pthread -c $<

ejemplo_malloc.o: ejemplo_malloc.c
	$(CC) -c $<

ejemplo_pipe.o: ejemplo_pipe.c
	$(CC) -c $<

ejercicio_arbol.o: ejercicio_arbol.c
	$(CC) -c $<

ejercicio_hilos.o: ejercicio_hilos.c
	$(CC) -pthread -c $<

ejercicio_pipes.o: ejercicio_pipes.c
	$(CC) -c $<

ejercicio_shell.o: ejercicio_shell.c
	$(CC) -c $<


ejemplo_buffer: ejemplo_buffer.c
	$(CC) -o $@ $^

ejemplo_descriptores: ejemplo_descriptores.c
	$(CC) -o $@ $^

ejemplo_fork: ejemplo_fork.c
	$(CC) -o $@ $^

ejemplo_hilos: ejemplo_hilos.c
	$(CC) -pthread -o $@ $^
	
ejemplo_malloc: ejemplo_malloc.c
	$(CC) -o $@ $^

ejemplo_pipe: ejemplo_pipe.c
	$(CC) -o $@ $^

ejercicio_arbol: ejercicio_arbol.c
	$(CC) -o $@ $^

ejercicio_hilos: ejercicio_hilos.c
	$(CC) -pthread -o $@ $^

ejercicio_pipes: ejercicio_pipes.c
	$(CC) -o $@ $^

ejercicio_shell: ejercicio_shell.c
	$(CC) -o $@ $^

clean:
	rm -rf *.o ejemplo_buffer ejemplo_descriptores ejemplo_fork ejemplo_hilos ejemplo_malloc ejemplo_pipe ejercicio_arbol ejercicio_hilos ejercicio_pipes ejercicio_shell

clear:
	rm -rf *.o