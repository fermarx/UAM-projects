CC = gcc
CFLAGS = -Wall -Wextra -pedantic
LDLIBS = -lodbc

EXE = dvdreq_customer dvdreq_film dvdreq_rent dvdreq_recommended dvdrent_new dvdrent_remove dvdfilm_remove

PG_RESTORE = pg_restore
CREATEDB = createdb

all : $(EXE)

clean :
	rm -f *.o core $(EXE)

$(EXE) : % : %.o odbc.o

restore:
	@echo restore data base
	@cat dvdrental.sql | psql  

createdb:
	@echo Creando BBDD
	@$(CREATEDB)
