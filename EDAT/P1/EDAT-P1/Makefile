# Version 1.2
# Modificacion
# 1) Las consultas 1 y 2 se han intercambiado para que sigan el mismo orden 
#    que en el enunciado de la práctica
# 2) se ha añadido la opción allqueries la cual ejecuta todas las consultas
# Por lo general no se requiere modificar el fichero
# http://www.dpriver.com/pp/sqlformat.htm

export PGDATABASE:=dvdrental_ext
export PGNEWDATABASE:=newdatabase
export PGUSER :=alumnodb
export PGPASSWORD :=alumnodb
export PGCLIENTENCODING:=LATIN9
export PGHOST:=localhost


DBNAME =$(PGDATABASE)
EX4DBNAME =$(PGNEWDATABASE)
PSQL = psql
CREATEDB = createdb
NEWCREATEDB = newcreatedb
DROPDB = dropdb
PG_DUMP = pg_dump
NEWPG_DUMP = newpg_dump
PG_RESTORE = pg_restore


all: dropdb createdb restore

newdatabase: dropnewdb exercise_4

exercise_4:
	@echo Deleting: $(DBNAME)
	@$(DROPDB)$(DBNAME)
	rm -f *.log
	@echo Creating new data base $(EX4DBNAME)
	@$(CREATEDB)$(EX4DBNAME)
	@echo restoring db
	psql -U $(PGUSER) -d $(EX4DBNAME) -1 -f $(PGNEWDATABASE).sql

dropnewdb:
	@echo Deleting BBDD
	@$(DROPDB) $(EX4DBNAME)
	rm -f *.log

createdb:
	@echo Creando BBDD
	@$(CREATEDB)
dropdb:
	@echo Eliminando BBDD
	@$(DROPDB) $(DBNAME)
	rm -f *.log
dump:
	@echo creando dumpfile
	@$(PG_DUMP) > $(DBNAME).sql
restore:
	@echo restore data base
	@cat $(DBNAME).sql | $(PSQL)  
shell:
	@echo create psql shell
	@$(PSQL)  

allqueries: query1 query2 query3 query4 query5 query6
	@cat query?.log > all_queries.log

query1:
	@echo query-1: "Alquireles por año" | tee query1.log
	@cat query1.sql | $(PSQL) | tee -a query1.log
query2:
	@echo query-2: "clienet que ha alquilado más peliculas" | tee query2.log
	@cat query2.sql | $(PSQL) | tee -a query2.log
query3:
	@echo query3: "Ciudades alquiler Bob Fawcett" | tee query3.log
	@cat query3.sql | $(PSQL) | tee -a query3.log
query4:
	@echo query4: "lenguaje en el que se producen más películas" | tee query4.log
	@cat query4.sql | $(PSQL) | tee -a query4.log
query5:
	@echo query5: "lenguaje en el que se alquilan más películas" | tee query5.log
	@cat query5.sql | $(PSQL) | tee -a query5.log
query6:
	@echo query6: "categoría favorita del cliente que alquila más peliculas" | tee query6.log
	@cat query6.sql | $(PSQL) | tee -a query6.log

