/*dvdreq_rent <customer_id> <init_date> <end_date>*/

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
    SQLSMALLINT columns = 0; /* number of columns in result-set */
    SQLUSMALLINT i;
    SQLINTEGER customerid;
    SQLCHAR initdate[32];
    SQLCHAR enddate[32];
    char customer_id[512];
    char rental_id[512];
    char rental_date[512];
    char film_id[512];
    char title[512];
    char staff_id[512];
    char first_name[512];
    char store_id[512];
    char amount[32];
    char query[32768];

    /*IN CASE OF ERROR */
    if (argc != 4) {
        fprintf(stderr, "Error in input parameters:\n\n");
        fprintf(stderr, "%s <customer_id> <init_date> <end_date>\n", argv[0]);
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
    strcpy((char*)initdate, argv[2]);
    strcpy((char*)enddate,argv[3]);

    SQLExecDirect(stmt, (SQLCHAR*) "SELECT rental.rental_id, rental_date, film.film_id, title, staff.staff_id, first_name, staff.store_id, amount\
                                    FROM payment, rental, inventory, film, staff\
                                    WHERE payment.staff_id = staff.staff_id\
                                    AND payment.rental_id = rental.rental_id\
                                    AND rental.inventory_id = inventory.inventory_id\
                                    AND inventory.film_id = film.film_id;", SQL_NTS);

    /* How many columns are there */
    SQLNumResultCols(stmt, &columns);

    /*PRINTS THE NAME OF ALL THE COLUMNS OF THE QUERY*/
    SQLNumResultCols(stmt, &columns);
    for (i = 1; i <= columns; i++) {
        SQLCHAR buf[512];
        SQLDescribeCol(stmt, i, buf, sizeof(buf), NULL, NULL, NULL, NULL, NULL);
        printf("%s\t", buf);
    }
    printf("\n"); 
    SQLCloseCursor(stmt);

    sprintf(query, "SELECT customer_id\
                    FROM rental\
                    WHERE customer_id = %d;", customerid);
    SQLExecDirect(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_CHAR, &customer_id, sizeof(customer_id), NULL);
    if (!SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf ("The customer %i does not exists\n", customerid);
        return 0;}
    SQLCloseCursor(stmt);

    sprintf(query, "SELECT rental.rental_id, rental_date, film.film_id, title, staff.staff_id, first_name, staff.store_id, amount\
                    FROM rental, inventory, film, staff, payment\
                    WHERE rental.inventory_id = inventory.inventory_id\
                    AND payment.staff_id = staff.staff_id\
                    AND payment.rental_id = rental.rental_id\
                    AND inventory.film_id = film.film_id\
                    AND rental.customer_id = %i\
                    AND rental_date < '%s' AND rental_date > '%s';", customerid, initdate, enddate);
    SQLExecDirect(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_CHAR, &rental_id, sizeof(rental_id), NULL);
    SQLBindCol(stmt, 2, SQL_C_CHAR, &rental_date, sizeof(rental_date), NULL);
    SQLBindCol(stmt, 3, SQL_C_CHAR, &film_id, sizeof(film_id), NULL);
    SQLBindCol(stmt, 4, SQL_C_CHAR, &title, sizeof(title), NULL);
    SQLBindCol(stmt, 5, SQL_C_CHAR, &staff_id, sizeof(staff_id), NULL);
    SQLBindCol(stmt, 6, SQL_C_CHAR, &first_name, sizeof(first_name), NULL);
    SQLBindCol(stmt, 7, SQL_C_CHAR, &store_id, sizeof(store_id), NULL);
    SQLBindCol(stmt, 8, SQL_C_CHAR, &amount, sizeof(amount), NULL);
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%s\t   %s\t%s\t%s\t%s\t%s\t\t%s\t\t%s\n", rental_id, rental_date, film_id, title, staff_id, first_name, store_id, amount);
    }
    SQLCloseCursor(stmt);

    /* free up statement handle */
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
} 