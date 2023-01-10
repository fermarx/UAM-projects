#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main(int argc, char** argv) {

    FILE *f = NULL;
    Graph *g = NULL; 

    if (argc < 2){
        printf("You should enter a namefile.\n");
        return 1;
    }

    f = fopen(argv[1], "r");
    if(!f) return 1;

    g = graph_init();
    if (!g){
        fclose(f);
        return 1;
    }

    if(graph_readFromFile(f, g) == ERROR){
        fclose(f);
        graph_free(g);
        return 1;
    }

    if (graph_print(stdout, g) == -1){
        fclose(f);
        graph_free(g);
        return 1;
    }

    fclose(f);
    graph_free(g);

    return 0;

}