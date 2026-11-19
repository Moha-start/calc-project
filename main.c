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
            freeCalc(input);
        }
        return EXIT_FAILURE;
    }

    trimTrailingZeros(result);
    
    for (unsigned int j = 0; words[j] != NULL; j++) {
        printf("%s", words[j]);
    }
    printf(" = %s\n", result);
    
    free(result);

    if (input != NULL) {
        freeCalc(input);
    }
    
    return EXIT_SUCCESS;
}