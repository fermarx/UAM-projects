/*dvdreq_rent <customer_id>*/

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
    char customer_id[10];
    char film_id[512];
    char title[512];
    char category_name[512];
    char query[32768];
    char list_rented[600];
    char most_rented_films[1224];

    /*IN CASE OF ERROR */
    if (argc != 2) {
        fprintf(stderr, "Error in input parameters:\n\n");
        fprintf(stderr, "%s <customer_id>\n", argv[0]);
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

    SQLExecDirect(stmt, (SQLCHAR*) "SELECT film.film_id, title, name\
                                    FROM film, category, film_category\
                                    WHERE film.film_id = film_category.film_id\
                                    AND film_category.category_id = category.category_id", SQL_NTS);

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

    sprintf (list_rented, "CREATE OR REPLACE VIEW list_rented AS\
                        SELECT category_id, count(category_id)\
                        FROM film_category, (\
                            SELECT film_id\
                            FROM rental, inventory\
                            WHERE rental.customer_id = %i\
                            AND rental.inventory_id = inventory.inventory_id) AS customer_rented\
                            WHERE customer_rented.film_id=film_category.film_id\
                        GROUP BY category_id\
                        ORDER BY count desc;", customerid);
    SQLExecDirect(stmt, (SQLCHAR*)list_rented, SQL_NTS);
    while (!SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf ("Error on view list_rented.\n"); 
        return 0;
    }
    SQLCloseCursor(stmt);

    sprintf (most_rented_films, "CREATE OR REPLACE VIEW most_rented_films AS\
                        SELECT film_id\
                        FROM (\
                            SELECT film_category.film_id, count(rental.inventory_id)\
                            FROM film_category, inventory, rental, (\
                                SELECT list_rented.category_id\
                                FROM list_rented, (\
                                    SELECT count \
                                    FROM list_rented\
                                    LIMIT 1) AS max\
                                WHERE list_rented.count = max.count) AS most_rented_category\
                            WHERE film_category.category_id = most_rented_category.category_id AND film_category.film_id = inventory.film_id\
                            AND inventory.inventory_id = rental.rental_id\
                            GROUP BY film_category.film_id\
                            ORDER BY count desc ) AS X\
                        EXCEPT (SELECT film_id\
                            FROM rental, inventory\
                            WHERE rental.customer_id = %i\
                            AND rental.inventory_id = inventory.inventory_id) LIMIT 3;", customerid);
    SQLExecDirect(stmt, (SQLCHAR*)most_rented_films, SQL_NTS);
    while (!SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf ("Error on view most_rented_films.\n"); 
        return 0;
    }
    SQLCloseCursor(stmt);

    sprintf (query, "SELECT most_rented_films.film_id, film.title, category.name\
                    FROM most_rented_films, film, film_category, category\
                    WHERE most_rented_films.film_id = film.film_id\
                    AND film_category.film_id = most_rented_films.film_id\
                    AND film_category.category_id = category.category_id");
    SQLExecDirect(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_CHAR, &film_id, sizeof(film_id), NULL);
    SQLBindCol(stmt, 2, SQL_C_CHAR, &title, sizeof(title), NULL);
    SQLBindCol(stmt, 3, SQL_C_CHAR, &category_name, sizeof(category_name), NULL);
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%s\t\t%s\t%s\n", film_id, title, category_name);
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