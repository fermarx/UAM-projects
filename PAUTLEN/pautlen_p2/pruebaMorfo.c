#include <stdio.h>
#include "tokens.h"
#define TOK_NEW_LINE 999

int yylex();

int main(int argc, char* argv[]) {
    extern FILE *yyin;
    extern FILE *yyout;
    extern char *yytext;
    extern int yyleng;
    int token, rows=1, columns=0, error=0;

    if(argc != 3) {
        printf("Error: ./prueba <infile> <outfile>");
        return -1;
    }
    
    yyin = fopen(argv[1], "r");
    if(!yyin) return -1;
    
    yyout = fopen(argv[2], "w");
    if(!yyout){
        fclose(yyin);
        return -1;
    }
    
    token = yylex();
    while (token != 0){
        if (error != 1){
            columns += yyleng;
        
            switch (token){
                case TOK_MAIN:
                    fprintf(yyout, "TOK_MAIN\t%d %s\n", TOK_MAIN, yytext);
                    break;
                case TOK_INT:
                    fprintf(yyout, "TOK_INT\t%d %s\n", TOK_INT, yytext);
                    break;
                case TOK_BOOLEAN:
                    fprintf(yyout, "TOK_BOOLEAN\t%d %s\n", TOK_BOOLEAN, yytext);
                    break;
                case TOK_ARRAY:
                    fprintf(yyout, "TOK_ARRAY\t%d %s\n", TOK_ARRAY, yytext);
                    break;
                case TOK_FUNCTION:
                    fprintf(yyout, "TOK_FUNCTION\t%d %s\n", TOK_FUNCTION, yytext);
                    break;
                case TOK_IF:
                    fprintf(yyout, "TOK_IF\t%d %s\n", TOK_IF, yytext);
                    break;
                case TOK_ELSE:
                    fprintf(yyout, "TOK_ELSE\t%d %s\n", TOK_ELSE, yytext);
                    break;
                case TOK_WHILE:
                    fprintf(yyout, "TOK_WHILE\t%d %s\n", TOK_WHILE, yytext);
                    break;
                case TOK_SCANF:
                    fprintf(yyout, "TOK_SCANF\t%d %s\n", TOK_SCANF, yytext);
                    break;
                case TOK_PRINTF:
                    fprintf(yyout, "TOK_PRINTF\t%d %s\n", TOK_PRINTF, yytext);
                    break;
                case TOK_RETURN:
                    fprintf(yyout, "TOK_RETURN\t%d %s\n", TOK_RETURN, yytext);
                    break;

                case TOK_PUNTOYCOMA:
                    fprintf(yyout, "TOK_PUNTOYCOMA\t%d %s\n", TOK_PUNTOYCOMA, yytext);
                    break;
                case TOK_COMA:
                    fprintf(yyout, "TOK_COMA\t%d %s\n", TOK_COMA, yytext);
                    break;
                case TOK_PARENTESISIZQUIERDO:
                    fprintf(yyout, "TOK_PARENTESISIZQUIERDO\t%d %s\n", TOK_PARENTESISIZQUIERDO, yytext);
                    break;
                case TOK_PARENTESISDERECHO:
                    fprintf(yyout, "TOK_PARENTESISDERECHO\t%d %s\n", TOK_PARENTESISDERECHO, yytext);
                    break;
                case TOK_CORCHETEIZQUIERDO:
                    fprintf(yyout, "TOK_CORCHETEIZQUIERDO\t%d %s\n", TOK_CORCHETEIZQUIERDO, yytext);
                    break;
                case TOK_CORCHETEDERECHO:
                    fprintf(yyout, "TOK_CORCHETEDERECHO\t%d %s\n", TOK_CORCHETEDERECHO, yytext);
                    break;
                case TOK_LLAVEIZQUIERDA:
                    fprintf(yyout, "TOK_LLAVEIZQUIERDA\t%d %s\n", TOK_LLAVEIZQUIERDA, yytext);
                    break;
                case TOK_LLAVEDERECHA:
                    fprintf(yyout, "TOK_LLAVEDERECHA\t%d %s\n", TOK_LLAVEDERECHA, yytext);
                    break;
                case TOK_ASIGNACION:
                    fprintf(yyout, "TOK_ASIGNACION\t%d %s\n", TOK_ASIGNACION, yytext);
                    break;
                case TOK_MAS:
                    fprintf(yyout, "TOK_MAS\t%d %s\n", TOK_MAS, yytext);
                    break;
                case TOK_MENOS:
                    fprintf(yyout, "TOK_MENOS\t%d %s\n", TOK_MENOS, yytext);
                    break;
                case TOK_DIVISION:
                    fprintf(yyout, "TOK_DIVISION\t%d %s\n", TOK_DIVISION, yytext);
                    break;
                case TOK_ASTERISCO:
                    fprintf(yyout, "TOK_ASTERISCO\t%d %s\n", TOK_ASTERISCO, yytext);
                    break;
                case TOK_AND:
                    fprintf(yyout, "TOK_AND\t%d %s\n", TOK_AND, yytext);
                    break;
                case TOK_OR:
                    fprintf(yyout, "TOK_OR\t%d %s\n", TOK_OR, yytext);
                    break;
                case TOK_NOT:
                    fprintf(yyout, "TOK_NOT\t%d %s\n", TOK_NOT, yytext);
                    break;
                case TOK_IGUAL:
                    fprintf(yyout, "TOK_IGUAL\t%d %s\n", TOK_IGUAL, yytext);
                    break;
                case TOK_DISTINTO:
                    fprintf(yyout, "TOK_DISTINTO\t%d %s\n", TOK_DISTINTO, yytext);
                    break;
                case TOK_MENORIGUAL:
                    fprintf(yyout, "TOK_MENORIGUAL\t%d %s\n", TOK_MENORIGUAL, yytext);
                    break;
                case TOK_MAYORIGUAL:
                    fprintf(yyout, "TOK_MAYORIGUAL\t%d %s\n", TOK_MAYORIGUAL, yytext);
                    break;
                case TOK_MENOR:
                    fprintf(yyout, "TOK_MENOR\t%d %s\n", TOK_MENOR, yytext);
                    break;
                case TOK_MAYOR:
                    fprintf(yyout, "TOK_MAYOR\t%d %s\n", TOK_MAYOR, yytext);
                    break;
                case TOK_IDENTIFICADOR:
                    fprintf(yyout, "TOK_IDENTIFICADOR\t%d %s\n", TOK_IDENTIFICADOR, yytext);
                    break;
                case TOK_CONSTANTE_ENTERA:
                    fprintf(yyout, "TOK_CONSTANTE_ENTERA\t%d %s\n", TOK_CONSTANTE_ENTERA, yytext);
                    break;
                case TOK_TRUE:
                    fprintf(yyout, "TOK_TRUE\t%d %s\n", TOK_TRUE, yytext);
                    break;
                case TOK_FALSE:
                    fprintf(yyout, "TOK_FALSE\t%d %s\n", TOK_FALSE, yytext);
                    break;
                /*Nuse que tiene que hacer el error*/
                case TOK_NEW_LINE:
                    rows++;
                    columns = 1;
                    break;
                default:
                    error = 1;
                    columns -= (yyleng - 1);

                    if (yyleng > 100){
                        fprintf(stderr, "****Error in [lin %d, col %d]: indentifier too long (%s)\n", rows, columns, yytext);
                    } else {
                        fprintf(stderr, "****Error in [lin %d, col %d]: symbol not allowed (%s)\n", rows, columns, yytext);
                    }
                    break;
            }
        }
        else{break;}
        token = yylex();
    }
    
    fclose(yyin);
    fclose(yyout);

    return 0;
}