#include <stdio.h>
#include "parser.tab.h"
#include "utilities.h"

int main(int argc, char const *argv[])
{
    init();
    printf("Starting parser...\n");
    if (yyparse() != 0) {
        fprintf(stderr, "Parsing failed.\n");
        return 1;
    }
    return 0;
}
