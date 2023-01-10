/*dvdrent_new  <customer Id> <film id> <staff id> <store id> <amount>*/

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
    char film_id[15];
    char customer_id[15];
    char staff_id[15];
    char store_id[15];
    char inventory_id[15];
    char rental_id[15];
    char query[32768];
    SQLINTEGER customerid;
    SQLINTEGER filmid;
    SQLINTEGER staffid;
    SQLINTEGER storeid;
    SQLINTEGER amount;

    /*IN CASE OF ERROR */
    if (argc != 6) {
        fprintf(stderr, "Error in input parameters:\n\n");
        fprintf(stderr, "%s  <customer Id> <film id> <staff id> <store id> <amount>\n", argv[0]);
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
    customerid = atoi(argv[1]);
    filmid = atoi(argv[2]);
    staffid = atoi(argv[3]);
    storeid = atoi(argv[4]);
    amount = atoi(argv[5]);

    sprintf (query,"SELECT film.film_id, inventory_id\
                    FROM film, inventory\
                    WHERE film.film_id = %i OR inventory.film_id = %i", filmid, filmid);
    SQLExecDirect(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_CHAR, &film_id, sizeof(film_id), NULL);
    SQLBindCol(stmt, 2, SQL_C_CHAR, &inventory_id, sizeof(inventory_id), NULL);
    if (!SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("Film with id %s does not exists or it is not avialbe.\n", film_id);
        return 0;}
    SQLCloseCursor(stmt);

    sprintf (query, "SELECT customer_id \
                            FROM customer\
                            WHERE customer_id = %i", customerid);
    SQLExecDirect(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_CHAR, &customer_id, sizeof(customer_id), NULL);
    if (!SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("Customer with id %s does not exists.\n", customer_id);
        return 0;}
    SQLCloseCursor(stmt);

    sprintf (query, "SELECT staff_id\
                    FROM staff\
                    WHERE staff_id =  %i", staffid);
    SQLExecDirect(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_CHAR, &staff_id, sizeof(staff_id), NULL);
    if (!SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("Employee with id %s does not exists.\n", staff_id);
        return 0;}
    SQLCloseCursor(stmt);

    sprintf (query, "SELECT store_id\
                    FROM store\
                    WHERE store_id = %i", storeid);
    SQLExecDirect(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_CHAR, &store_id, sizeof(store_id), NULL);
    if (!SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("Store with id %s does not exists.\n", store_id);
        return 0;}
    SQLCloseCursor(stmt);    

    sprintf (query, "INSERT INTO rental VALUES\
                    (default, now(), %s, %i, NULL, %i, now())", inventory_id, customerid, staffid);
    SQLPrepare(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLExecute(stmt);

    sprintf (query, "SELECT rental_id\
                    FROM rental\
                    WHERE inventory_id = %s", inventory_id);
    SQLExecDirect(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_CHAR, &rental_id, sizeof(rental_id), NULL);
    if (!SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("Rental with id %s does not exists.\n", rental_id);
        return 0;}
    SQLCloseCursor(stmt);

    sprintf(query,"INSERT INTO payment VALUES\
                    (default, %i, %i, %s, %i, now())",customerid, staffid, rental_id, amount);
    SQLPrepare(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLExecute(stmt);

    printf("Correct, you have rented the film with id %s\n", film_id);

    /* free up statement handle */
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}   