#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.h"

/* Helper to replace POSIX strdup for strict C99 compliance */
char *test_strdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *dup = malloc(len);
    if (dup) {
        memcpy(dup, s, len);
    }
    return dup;
}

/* Helper function to create a mock Calc struct from an array of strings */
Calc* create_test_calc(int count, const char **strings) {
    Calc *c = malloc(sizeof(Calc));
    c->word = malloc((count + 1) * sizeof(char*));
    for (int i = 0; i < count; i++) {
        c->word[i] = test_strdup(strings[i]);
    }
    c->word[count] = NULL;
    return c;
}

bool test_dummy(char *c){
    if(strcmp("dummy",c)==0){
        return true;
    }
    return false;
}

bool test_getCalc(void) {
    FILE *tmp = tmpfile();
    if (!tmp) return false;
    fputs("3 + 4\n", tmp);
    rewind(tmp);
    FILE *old_stdin = stdin;
    stdin = tmp;
    
    Calc *c = getCalc();
    stdin = old_stdin;
    fclose(tmp);
    
    if (c == NULL) return false;
    if (c->word == NULL) {
        FreeCalc(c);
        return false;
    }
    FreeCalc(c);
    return true;
}

bool test_IsSameOpen(void) {
    if (!IsSameOpen('(', ')')) return false;
    if (!IsSameOpen('[', ']')) return false;
    if (!IsSameOpen('{', '}')) return false;
    if (IsSameOpen('(', '}')) return false;
    return true;
}

bool test_FreeCalc(void) {
    const char *w[] = {"1", "+", "1"};
    Calc *c = create_test_calc(3, w);
    FreeCalc(c); 
    /* If memory is not freed, valgrind memcheck will fail this test */
    return true;
}

bool test_AreDelimitersValid(void) {
    const char *w1[] = {"(", "2", "+", "2", ")"};
    Calc *c1 = create_test_calc(5, w1);
    if (!AreDelimitersValid(c1)) {
        FreeCalc(c1);
        return false;
    }
    FreeCalc(c1);

    const char *w2[] = {"(", "2", "+", "2"};
    Calc *c2 = create_test_calc(4, w2);
    if (AreDelimitersValid(c2)) {
        FreeCalc(c2);
        return false;
    }
    FreeCalc(c2);
    return true;
}

bool test_countOpeningDelimiters(void) {
    const char *w[] = {"(", "[", "1", "+", "2", "]", ")"};
    Calc *c = create_test_calc(7, w);
    if (countOpeningDelimiters(c) != 2) {
        FreeCalc(c);
        return false;
    }
    FreeCalc(c);
    return true;
}

bool test_printCalc(void) {
    const char *w[] = {"5", "*", "5"};
    Calc *c = create_test_calc(3, w);
    
    const char *tmp_filename = "test_print_output.txt";
    if (freopen(tmp_filename, "w", stdout) == NULL) {
        FreeCalc(c);
        return false;
    }

    printCalc(c);
    fflush(stdout);

    FILE *f = fopen(tmp_filename, "r");
    if (f == NULL) {
        FreeCalc(c);
        return false;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fclose(f);

    remove(tmp_filename); 
    FreeCalc(c);
    return (size > 0);
}

bool test_calctype(void) {
    if (calctype('7') != NUMBER) return false;
    if (calctype('+') != OPERATION) return false;
    if (calctype('x') != VARIABLE) return false;
    if (calctype('(') != DELIMITER) return false;
    return true;
}

bool test_getCalctype(void) {
    if (getCalctype("42") != NUMBER) return false;
    if (getCalctype("*") != OPERATION) return false;
    if (getCalctype("100") != NUMBER) return false;
    if (getCalctype("}") != DELIMITER) return false;
    return true;
}

bool test_sizeCalc(void) {
    const char *w[] = {"10", "/", "2"};
    Calc *c = create_test_calc(3, w);
    if (sizeCalc(c) != 4) {
        FreeCalc(c);
        return false;
    }
    FreeCalc(c);
    return true;
}

bool test_moveResult(void) {
    const char *w[] = {"1", "+", "2", "*", "3"};
    Calc *c = create_test_calc(5, w);
    char *res = test_strdup("6");
    moveResult(c, 2, 4, res);
    
    if (strcmp(c->word[2], "6") != 0) {
        FreeCalc(c);
        return false;
    }
    FreeCalc(c);
    return true;
}

bool test_fromStringToDouble(void) {
    if (fromStringToDouble("42.5") != 42.5) return false;
    if (fromStringToDouble("-3.14") != -3.14) return false;
    return true;
}

bool test_fromDoobleToString(void) {
    char *s = fromDoobleToString(9.99);
    if (s == NULL) return false;
    double val = fromStringToDouble(s);
    if (val < 9.98 || val > 10.0) {
        free(s);
        return false;
    }
    free(s);
    return true;
}

bool test_isMixOperation(void) {
    const char *w1[] = {"1", "+", "2", "*", "3"};
    Calc *c1 = create_test_calc(5, w1);
    if (!isMixOperation(c1)) {
        FreeCalc(c1);
        return false;
    }
    FreeCalc(c1);

    const char *w2[] = {"1", "+", "2", "+", "3"};
    Calc *c2 = create_test_calc(5, w2);
    if (isMixOperation(c2)) {
        FreeCalc(c2);
        return false;
    }
    FreeCalc(c2);
    return true;
}

bool test_calculateSimple(void) {
    if (calculateSimple(4.0, '+', 2.0) != 6.0) return false;
    if (calculateSimple(4.0, '-', 2.0) != 2.0) return false;
    if (calculateSimple(4.0, '*', 2.0) != 8.0) return false;
    if (calculateSimple(4.0, '/', 2.0) != 2.0) return false;
    return true;
}

bool test_isPrio(void) {
    const char *w[] = {"1", "+", "2", "*", "3"};
    Calc *c = create_test_calc(5, w);
    uint idx = 0;
    if (!isPrio(c, &idx) || idx != 3) {
        FreeCalc(c);
        return false;
    }
    FreeCalc(c);
    return true;
}

bool test_solveSimple(void) {
    const char *w[] = {"2", "+", "3", "*", "4"};
    Calc *c = create_test_calc(5, w);
    char *res = solveSimple(c);
    if (res == NULL) {
        FreeCalc(c);
        return false;
    }
    free(res);
    FreeCalc(c);
    return true;
}

bool test_evaluateParenthesizedExpression(void) {
    const char *w[] = {"(", "2", "+", "3", ")", "*", "4"};
    Calc *c = create_test_calc(7, w);
    evaluateParenthesizedExpression(c);
    if (c->word[0] == NULL) {
        FreeCalc(c);
        return false;
    }
    FreeCalc(c);
    return true;
}

bool test_solve(void) {
    const char *w[] = {"(", "2", "+", "3", ")", "*", "4", NULL};
    char **words_copy = malloc(8 * sizeof(char*));
    for(int i = 0; i < 7; i++) words_copy[i] = (char*)w[i];
    words_copy[7] = NULL;
    
    char *res = solve(words_copy);
    free(words_copy);

    if (res == NULL) return false;
    if (fromStringToDouble(res) != 20.0) {
        free(res);
        return false;
    }
    free(res);
    return true;
}

int main(int argc, char * argv[]){
    if(argc==1){
        fprintf(stderr, "Error: no argument given!\n");
        exit(EXIT_FAILURE);
    }
    
    bool ok = false;
    
    if(strcmp("dummy", argv[1])==0){
        ok = test_dummy(argv[1]);
    } else if(strcmp("getCalc", argv[1])==0){
        ok = test_getCalc();
    } else if(strcmp("IsSameOpen", argv[1])==0){
        ok = test_IsSameOpen();
    } else if(strcmp("FreeCalc", argv[1])==0){
        ok = test_FreeCalc();
    } else if(strcmp("AreDelimitersValid", argv[1])==0){
        ok = test_AreDelimitersValid(); 
    } else if(strcmp("countOpeningDelimiters", argv[1])==0){
        ok = test_countOpeningDelimiters(); 
    } else if(strcmp("printCalc", argv[1])==0){
        ok = test_printCalc(); 
    } else if(strcmp("calctype", argv[1])==0){
        ok = test_calctype();
    } else if(strcmp("getCalctype", argv[1])==0){
        ok = test_getCalctype();
    } else if(strcmp("sizeCalc", argv[1])==0){
        ok = test_sizeCalc();
    } else if(strcmp("moveResult", argv[1])==0){
        ok = test_moveResult();
    } else if(strcmp("fromStringToDouble", argv[1])==0){
        ok = test_fromStringToDouble();
    } else if(strcmp("fromDoobleToString", argv[1])==0){
        ok = test_fromDoobleToString();
    } else if(strcmp("isMixOperation", argv[1])==0){
        ok = test_isMixOperation();
    } else if(strcmp("calculateSimple", argv[1])==0){
        ok = test_calculateSimple();
    } else if(strcmp("isPrio", argv[1])==0){
        ok = test_isPrio();
    } else if(strcmp("solveSimple", argv[1])==0){
        ok = test_solveSimple();
    } else if(strcmp("evaluateParenthesizedExpression", argv[1])==0){
        ok = test_evaluateParenthesizedExpression();
    } else if(strcmp("solve", argv[1])==0){
        ok = test_solve();
    } else {
        fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (ok)
    {
        fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }
}