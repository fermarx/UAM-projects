/*dvdreq_customer -n <first_name> -a <last_name> */

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
    char customer_id[512];
    char first_name[512];
    char last_name[512];
    char regisdate[512];
    char address[512];
    char city[512];
    char country[512];
    char query[512];
    SQLUSMALLINT i;
    SQLCHAR name[512];
    SQLCHAR lastname[512];

    /*IN CASE OF ERROR */
    if (argc != 5) {
        fprintf(stderr, "Error in input parameters:\n\n");
        fprintf(stderr, "%s -n <First Name> -a <Last Name>\n", argv[0]);
        fprintf(stderr, "Where:\n");
        fprintf(stderr, "-First Name: first name of the costumer\n");
        fprintf(stderr, "-Last Name: last name of the costumer\n");
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
    if (strcmp(argv[1], "-n") == 0) {
        strcpy((char*)name, argv[2]);
    }if (strcmp(argv[3], "-a") == 0) {
        strcpy((char*)lastname, argv[4]);
    } 

    SQLExecDirect(stmt, (SQLCHAR*) "SELECT customer_id, first_name, last_name, create_date, address, city, country\
                                    FROM customer, address, city,  country\
                                    WHERE city.country_id = country.country_id\
                                    AND city.city_id = address.city_id\
                                    AND address.address_id = customer.address_id", SQL_NTS);

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
    
    sprintf(query, "SELECT customer_id, first_name, last_name, create_date, address, city, country\
                    FROM customer, address, city,  country\
                    WHERE city.country_id = country.country_id\
                    AND city.city_id = address.city_id\
                    AND address.address_id = customer.address_id\
                    AND (first_name = '%s' OR last_name = '%s');", name, lastname);
    SQLExecDirect(stmt, (SQLCHAR*)query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_CHAR, &customer_id, sizeof(customer_id), NULL);
    SQLBindCol(stmt, 2, SQL_C_CHAR, &first_name, sizeof(first_name), NULL);
    SQLBindCol(stmt, 3, SQL_C_CHAR, &last_name, sizeof(last_name), NULL);
    SQLBindCol(stmt, 4, SQL_C_CHAR, &regisdate, sizeof(regisdate), NULL);
    SQLBindCol(stmt, 5, SQL_C_CHAR, &address, sizeof(address), NULL);
    SQLBindCol(stmt, 6, SQL_C_CHAR, &city, sizeof(city), NULL);
    SQLBindCol(stmt, 7, SQL_C_CHAR, &country, sizeof(country), NULL);
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%s\t\t%s\t\t%s\t%s\t%s\t%s\t%s\n", customer_id, first_name, last_name, regisdate, address, city, country);
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