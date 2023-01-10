/*dvdfilm_remove <film_id>*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

int main (int argc, char **argv) {

    SQLHENV env; /*ENVIRONMENT */
    SQLHDBC dbc; /*DATABASE*/
    SQLHSTMT stmt; /*STATEMENT HANDLE*/
    SQLRETURN ret; /* ODBC API return status */
    char film_id[30];
    char query[512];
    SQLINTEGER filmid;

    /*IN CASE OF ERROR */
    if (argc != 2) {
        fprintf(stderr, "Error in input parameters:\n\n");
        fprintf(stderr, "%s  <film Id>\n", argv[0]);
        exit(-1);
    }

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    /*COPY THE ARGUMENTS ENTERED BY THE USER*/
    filmid = atoi(argv[1]);

    sprintf (query, "SELECT film_id\
                    FROM film\
                    WHERE film_id = %i", filmid);
    SQLExecDirect(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_CHAR, &film_id, sizeof(film_id), NULL);
    if (!SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("Film with id %i does not exists.\n", filmid);
        return 0;}
    SQLCloseCursor(stmt);
    
    sprintf(query, "DELETE FROM payment\
                    WHERE payment_id IN (SELECT payment_id\
                                        FROM payment, rental, inventory\
                                        WHERE inventory.inventory_id = rental.inventory_id\
                                        AND payment.rental_id = rental.rental_id\
                                        AND film_id = %i);", filmid);
    SQLPrepare(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLExecute(stmt);

    sprintf(query, "DELETE FROM rental\
                    WHERE rental_id IN (SELECT rental_id\
                                        FROM rental, inventory\
                                        WHERE inventory.inventory_id = rental.inventory_id\
                                        AND film_id = %i);", filmid);
    SQLPrepare(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLExecute(stmt);
    
    sprintf(query, "DELETE FROM inventory\
                    WHERE film_id = %i", filmid);
    SQLPrepare(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLExecute(stmt);

    sprintf(query, "DELETE FROM film_category\
                    WHERE film_id = %i", filmid);
    SQLPrepare(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLExecute(stmt);

    sprintf(query, "DELETE FROM film_actor\
                    WHERE film_id = %i", filmid);
    SQLPrepare(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLExecute(stmt);

    sprintf(query, "DELETE FROM film\
                    WHERE film_id = %i", filmid);
    SQLPrepare(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLExecute(stmt);

    printf ("Film with rental_id %s was deleted.\n", film_id);

    /* free up statement handle */
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
} 