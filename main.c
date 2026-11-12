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
    
    if (result == NULL) {
        if (input != NULL) {
            FreeCalc(input);
        }
        return EXIT_FAILURE;
    }

    trim_trailing_zeros(result);
    
    for (unsigned int j = 0; words[j] != NULL; j++) {
        printf("%s", words[j]);
    }
    printf(" = %s\n", result);
    
    free(result);

    if (input != NULL) {
        FreeCalc(input);
    }
    
    return EXIT_SUCCESS;
}