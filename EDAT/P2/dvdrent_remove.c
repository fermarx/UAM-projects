/*dvdrent_remove <rent_id>*/

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
    char rent_id[30];
    char prent_id[30];
    char query[512];
    SQLINTEGER rentid;

    /*IN CASE OF ERROR */
    if (argc != 2) {
        fprintf(stderr, "Error in input parameters:\n\n");
        fprintf(stderr, "%s  <rent Id>\n", argv[0]);
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
    rentid = atoi(argv[1]);

    sprintf (query, "SELECT payment.rental_id\
                    FROM rental, payment\
                    WHERE rental.rental_id = payment.rental_id\
                    AND rental.rental_id = %i", rentid);
    SQLExecDirect(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_CHAR, &rent_id, sizeof(rent_id), NULL);
    SQLBindCol(stmt, 2, SQL_C_CHAR, &prent_id, sizeof(prent_id), NULL);
    if (!SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("Rent or payment with id %i does not exists.\n", rentid);
        return 0;}
    SQLCloseCursor(stmt);
    
    sprintf(query, "DELETE FROM payment\
                    WHERE payment.rental_id = %i", rentid);
    SQLPrepare(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLExecute(stmt);

    sprintf(query, "DELETE FROM rental\
                    WHERE rental_id =%i", rentid);
    SQLPrepare(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLExecute(stmt);
    
    printf ("Rental with rental_id %s was deleted, as well as the associated payment.\n", rent_id);

    /* free up statement handle */
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
} 