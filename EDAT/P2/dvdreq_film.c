/*dvdreq_film <title>*/

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
    SQLCHAR title[512];
    char film_title[512];
    char film_id[512];
    char release_year[512];
    char length[512];
    char language[512];
    char description[512];
    char first_name[512];
    char last_name[512];
    char query[512];


    /*IN CASE OF ERROR */
    if (argc != 2) {
        fprintf(stderr, "Error in input parameters:\n\n");
        fprintf(stderr, "%s '<Title>'\n", argv[0]);
        exit(-1);
    }

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    /*COPY THE ARGUMENTS ENTER BY THE USER*/
    strcpy((char*)title, argv[1]);

    SQLExecDirect(stmt, (SQLCHAR*) "SELECT film.film_id, title, release_year, length, name, description\
        FROM film, language , film_actor, actor\
        WHERE language.language_id = film.language_id\
        AND actor.actor_id = film_actor.actor_id\
        AND film.film_id = film_actor.film_id\
        AND film_actor.film_id = film.film_id", SQL_NTS);

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
    
    /*CHEKING IF THE MOVIE EXISTS*/
    sprintf(film_title, "SELECT title FROM film WHERE title = '%s';", title);
    SQLExecDirect(stmt, (SQLCHAR*)film_title, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_CHAR, &film_title, sizeof(film_title), NULL);
    if (!SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf ("The film %s does not exists\n", title);
        return 0;}
    SQLCloseCursor(stmt);


    sprintf(query, "SELECT film.film_id, title, release_year, length, name, description\
                    FROM film, language , film_actor, actor\
                    WHERE language.language_id = film.language_id\
                    AND actor.actor_id = film_actor.actor_id\
                    AND film.film_id = film_actor.film_id\
                    AND film_actor.film_id = film.film_id\
                    AND title = '%s';", title);
    SQLExecDirect(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_CHAR, &film_id, sizeof(film_id), NULL);
    SQLBindCol(stmt, 2, SQL_C_CHAR, &film_title, sizeof(film_title), NULL);
    SQLBindCol(stmt, 3, SQL_C_CHAR, &release_year, sizeof(release_year), NULL);
    SQLBindCol(stmt, 4, SQL_C_CHAR, &length, sizeof(length), NULL);
    SQLBindCol(stmt, 5, SQL_C_CHAR, &language, sizeof(language), NULL);
    SQLBindCol(stmt, 6, SQL_C_CHAR, &description, sizeof(description), NULL);
    while (!SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf ("Error.\n"); 
        return 0;
    }
    SQLCloseCursor(stmt);
    printf("%s\t%s\t%s\t%s\t%s\t%s\n\n", film_id, film_title, release_year, length, language, description);


    printf("first_name\t last_name\n");
    sprintf(query, "SELECT first_name, last_name\
                    FROM film, language , film_actor, actor\
                    WHERE language.language_id = film.language_id\
                    AND actor.actor_id = film_actor.actor_id\
                    AND film.film_id = film_actor.film_id\
                    AND film_actor.film_id = film.film_id\
                    AND title = '%s';", title);
    SQLExecDirect(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_CHAR, &first_name, sizeof(first_name), NULL);
    SQLBindCol(stmt, 2, SQL_C_CHAR, &last_name, sizeof(last_name), NULL);
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%s\t\t %s\n", first_name, last_name);
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