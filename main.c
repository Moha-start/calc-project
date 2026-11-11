#include <stdio.h>
#include <stdlib.h>
#include "calc.h"

int main(int argc, char **argv) {
    Calc *input = NULL;
    char **words;
    if (argc == 1) {
        input = getCalc();
        words = input->word;
    } else {
        words = argv + 1;
    }

    char *result = solve(words);
    if (input != NULL) {
        FreeCalc(input);
    }
    if (result == NULL) {
        return EXIT_FAILURE;
    }
    trim_trailing_zeros(result);
    
    for (uint i=0; words[i]!=NULL;i++){
        printf("%s", words[i]);
    }
    printf(" = %s\n", result);
    free(result);
    return EXIT_SUCCESS;
}