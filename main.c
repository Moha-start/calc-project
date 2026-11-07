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

    printf("%s\n", result);
    free(result);
    return EXIT_SUCCESS;
}