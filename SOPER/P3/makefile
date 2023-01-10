.PHONY: all clean

all: clean shm_producer clear

shm_producer: shm_producer.o shm_consumer.o
	gcc  -pthread -g -o shm_producer shm_producer.o shm_consumer.o -lrt

shm_producer.o:
	gcc -pthread -lrt -g -c shm_producer.c

shm_consumer.o:
	gcc -lrt -pthread -g -c shm_consumer.c

clean: 
	rm -f *.o shm_producer

clear:
	rm -rf *.o