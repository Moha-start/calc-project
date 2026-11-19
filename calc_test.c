#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.h"
#include "math.h"

/* ============================================================================
 * Test Helpers & Assertion Macros
 * ============================================================================ */

static double abs_d(double x) {
    return x < 0.0 ? -x : x;
}

static bool approx_eq(double a, double b, double eps) {
    return abs_d(a - b) <= eps;
}

/* Strict C99 replacement for POSIX strdup */
static char *test_strdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *dup = malloc(len);
    if (dup) {
        memcpy(dup, s, len);
    }
    return dup;
}

static Calc *create_test_calc(int count, const char **strings) {
    Calc *c = malloc(sizeof(Calc));
    if (!c) return NULL;
    c->word = malloc((size_t)(count + 1) * sizeof(char *));
    if (!c->word) {
        free(c);
        return NULL;
    }
    for (int i = 0; i < count; i++) {
        c->word[i] = test_strdup(strings[i]);
    }
    c->word[count] = NULL;
    return c;
}

/* Helper to call solve() cleanly from a static string array */
static char *run_solve(int count, const char **strings) {
    char **words = malloc((size_t)(count + 1) * sizeof(char *));
    if (!words) return NULL;
    for (int i = 0; i < count; i++) {
        words[i] = (char *)strings[i];
    }
    words[count] = NULL;
    char *res = solve(words);
    free(words);
    return res;
}

#define CHECK(cond) do { \
    if (!(cond)) { \
        fprintf(stderr, "  [FAIL] %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        return false; \
    } \
} while (0)

#define CHECK_CALC(cond, calc_ptr) do { \
    if (!(cond)) { \
        fprintf(stderr, "  [FAIL] %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        freeCalc(calc_ptr); \
        return false; \
    } \
} while (0)

/* ============================================================================
 * 1. Core Tests (calc_core.h)
 * ============================================================================ */

bool test_dummy(char *c) {
    CHECK(strcmp("dummy", c) == 0);
    return true;
}

bool test_getCalc(void) {
    /* Case 1: Expression with spaces (spaces should be stripped) */
    FILE *tmp = tmpfile();
    CHECK(tmp != NULL);
    fputs("  3 + 4 * 5 \n", tmp);
    rewind(tmp);

    FILE *old_stdin = stdin;
    stdin = tmp;
    Calc *c = getCalc();
    stdin = old_stdin;
    fclose(tmp);

    CHECK(c != NULL);
    CHECK_CALC(c->word != NULL, c);
    CHECK_CALC(c->word[0] != NULL, c);
    CHECK_CALC(strcmp(c->word[0], "3+4*5") == 0, c);
    CHECK_CALC(c->word[1] == NULL, c);
    freeCalc(c);

    /* Case 2: Expression with no spaces */
    tmp = tmpfile();
    CHECK(tmp != NULL);
    fputs("(10-2)/4\n", tmp);
    rewind(tmp);

    stdin = tmp;
    c = getCalc();
    stdin = old_stdin;
    fclose(tmp);

    CHECK(c != NULL);
    CHECK_CALC(c->word != NULL && c->word[0] != NULL, c);
    CHECK_CALC(strcmp(c->word[0], "(10-2)/4") == 0, c);
    freeCalc(c);

    return true;
}

bool test_freeCalc(void) {
    /* Standard multi-word Calc */
    const char *w1[] = {"1", "+", "2", "*", "3"};
    Calc *c1 = create_test_calc(5, w1);
    freeCalc(c1);

    /* Single-word Calc */
    const char *w2[] = {"42"};
    Calc *c2 = create_test_calc(1, w2);
    freeCalc(c2);

    /* Empty word array (only NULL terminator) */
    Calc *c3 = create_test_calc(0, NULL);
    freeCalc(c3);

    return true;
}

bool test_printCalc(void) {
    const char *w[] = {"5", "*", "(", "2", "+", "3", ")"};
    Calc *c = create_test_calc(7, w);

    const char *tmp_filename = "test_print_output.txt";
    FILE *f = freopen(tmp_filename, "w", stdout);
    if (f == NULL) {
        freeCalc(c);
        return false;
    }

    printCalc(c);
    fflush(stdout);

    /* Read back printed output */
    FILE *in = fopen(tmp_filename, "r");
    if (in == NULL) {
        freeCalc(c);
        return false;
    }

    fseek(in, 0, SEEK_END);
    long size = ftell(in);
    fclose(in);
    remove(tmp_filename);
    freeCalc(c);

    CHECK(size > 0);
    return true;
}

bool test_getCalcSize(void) {
    /* Empty array -> 1 (just NULL terminator) */
    Calc *c0 = create_test_calc(0, NULL);
    CHECK_CALC(getCalcSize(c0) == 1, c0);
    freeCalc(c0);

    /* Single element -> 2 */
    const char *w1[] = {"42"};
    Calc *c1 = create_test_calc(1, w1);
    CHECK_CALC(getCalcSize(c1) == 2, c1);
    freeCalc(c1);

    /* Three elements -> 4 */
    const char *w3[] = {"10", "/", "2"};
    Calc *c3 = create_test_calc(3, w3);
    CHECK_CALC(getCalcSize(c3) == 4, c3);
    freeCalc(c3);

    /* Seven elements -> 8 */
    const char *w7[] = {"(", "1", "+", "2", ")", "*", "3"};
    Calc *c7 = create_test_calc(7, w7);
    CHECK_CALC(getCalcSize(c7) == 8, c7);
    freeCalc(c7);

    return true;
}

bool test_replaceRangeWithResult(void) {
    /* Case 1: Replace tail range [2..4] */
    const char *w1[] = {"1", "+", "2", "*", "3"};
    Calc *c1 = create_test_calc(5, w1);
    replaceRangeWithResult(c1, 2, 4, test_strdup("6"));
    CHECK_CALC(getCalcSize(c1) == 4, c1);
    CHECK_CALC(strcmp(c1->word[0], "1") == 0, c1);
    CHECK_CALC(strcmp(c1->word[1], "+") == 0, c1);
    CHECK_CALC(strcmp(c1->word[2], "6") == 0, c1);
    CHECK_CALC(c1->word[3] == NULL, c1);
    freeCalc(c1);

    /* Case 2: Replace head range [0..2] */
    const char *w2[] = {"2", "*", "3", "+", "4"};
    Calc *c2 = create_test_calc(5, w2);
    replaceRangeWithResult(c2, 0, 2, test_strdup("6"));
    CHECK_CALC(getCalcSize(c2) == 4, c2);
    CHECK_CALC(strcmp(c2->word[0], "6") == 0, c2);
    CHECK_CALC(strcmp(c2->word[1], "+") == 0, c2);
    CHECK_CALC(strcmp(c2->word[2], "4") == 0, c2);
    CHECK_CALC(c2->word[3] == NULL, c2);
    freeCalc(c2);

    /* Case 3: Replace entire expression [0..2] */
    const char *w3[] = {"10", "-", "4"};
    Calc *c3 = create_test_calc(3, w3);
    replaceRangeWithResult(c3, 0, 2, test_strdup("6"));
    CHECK_CALC(getCalcSize(c3) == 2, c3);
    CHECK_CALC(strcmp(c3->word[0], "6") == 0, c3);
    CHECK_CALC(c3->word[1] == NULL, c3);
    freeCalc(c3);

    /* Case 4: Replace single element [1..1] */
    const char *w4[] = {"1", "+", "2"};
    Calc *c4 = create_test_calc(3, w4);
    replaceRangeWithResult(c4, 1, 1, test_strdup("-"));
    CHECK_CALC(getCalcSize(c4) == 4, c4);
    CHECK_CALC(strcmp(c4->word[1], "-") == 0, c4);
    freeCalc(c4);

    return true;
}

/* ============================================================================
 * 2. Parser Tests (calc_parser.h)
 * ============================================================================ */

bool test_isMatchingDelimiter(void) {
    /* Valid pairs */
    CHECK(isMatchingDelimiter('(', ')'));
    CHECK(isMatchingDelimiter('[', ']'));
    CHECK(isMatchingDelimiter('{', '}'));

    /* Mismatched pairs */
    CHECK(!isMatchingDelimiter('(', ']'));
    CHECK(!isMatchingDelimiter('(', '}'));
    CHECK(!isMatchingDelimiter('[', ')'));
    CHECK(!isMatchingDelimiter('[', '}'));
    CHECK(!isMatchingDelimiter('{', ')'));
    CHECK(!isMatchingDelimiter('{', ']'));

    /* Reversed & non-delimiters */
    CHECK(!isMatchingDelimiter(')', '('));
    CHECK(!isMatchingDelimiter('a', 'a'));
    CHECK(!isMatchingDelimiter('(', '('));

    return true;
}

bool test_areDelimitersValid(void) {
    /* Valid: simple */
    const char *v1[] = {"(", "2", "+", "2", ")"};
    Calc *c = create_test_calc(5, v1);
    CHECK_CALC(areDelimitersValid(c), c);
    freeCalc(c);

    /* Valid: nested mixed delimiters */
    const char *v2[] = {"{", "[", "(", "1", "+", "2", ")", "*", "3", "]", "}"};
    c = create_test_calc(11, v2);
    CHECK_CALC(areDelimitersValid(c), c);
    freeCalc(c);

    /* Valid: no delimiters */
    const char *v3[] = {"1", "+", "2", "*", "3"};
    c = create_test_calc(5, v3);
    CHECK_CALC(areDelimitersValid(c), c);
    freeCalc(c);

    /* Valid: multiple adjacent groups */
    const char *v4[] = {"(", "1", "+", "2", ")", "*", "[", "3", "+", "4", "]"};
    c = create_test_calc(11, v4);
    CHECK_CALC(areDelimitersValid(c), c);
    freeCalc(c);

    /* Invalid: unclosed opening */
    const char *i1[] = {"(", "2", "+", "2"};
    c = create_test_calc(4, i1);
    CHECK_CALC(!areDelimitersValid(c), c);
    freeCalc(c);

    /* Invalid: extra closing */
    const char *i2[] = {"2", "+", "2", ")"};
    c = create_test_calc(4, i2);
    CHECK_CALC(!areDelimitersValid(c), c);
    freeCalc(c);

    /* Invalid: mismatched types */
    const char *i3[] = {"(", "2", "+", "2", "]"};
    c = create_test_calc(5, i3);
    CHECK_CALC(!areDelimitersValid(c), c);
    freeCalc(c);

    /* Invalid: interleaved / crossed */
    const char *i4[] = {"(", "[", "1", ")", "]"};
    c = create_test_calc(5, i4);
    CHECK_CALC(!areDelimitersValid(c), c);
    freeCalc(c);

    return true;
}

bool test_trimTrailingZeros(void) {
    char s1[32] = "3.500000";
    trimTrailingZeros(s1);
    CHECK(strcmp(s1, "3.5") == 0);

    char s2[32] = "42.000000";
    trimTrailingZeros(s2);
    CHECK(strcmp(s2, "42") == 0);

    char s3[32] = "100";
    trimTrailingZeros(s3);
    CHECK(strcmp(s3, "100") == 0);

    char s4[32] = "0.0000";
    trimTrailingZeros(s4);
    CHECK(strcmp(s4, "0") == 0);

    char s5[32] = "-12.3400";
    trimTrailingZeros(s5);
    CHECK(strcmp(s5, "-12.34") == 0);

    char s6[32] = "1.0500";
    trimTrailingZeros(s6);
    CHECK(strcmp(s6, "1.05") == 0);

    return true;
}

bool test_countOpeningDelimiters(void) {
    /* None */
    const char *w0[] = {"1", "+", "2"};
    Calc *c = create_test_calc(3, w0);
    CHECK_CALC(countOpeningDelimiters(c) == 0, c);
    freeCalc(c);

    /* Nested ( and [ */
    const char *w2[] = {"(", "[", "1", "+", "2", "]", ")"};
    c = create_test_calc(7, w2);
    CHECK_CALC(countOpeningDelimiters(c) == 2, c);
    freeCalc(c);

    /* All three types */
    const char *w3[] = {"{", "(", "1", ")", "+", "[", "2", "]", "}"};
    c = create_test_calc(9, w3);
    CHECK_CALC(countOpeningDelimiters(c) == 3, c);
    freeCalc(c);

    /* Closing only -> 0 */
    const char *wc[] = {")", "]", "}"};
    c = create_test_calc(3, wc);
    CHECK_CALC(countOpeningDelimiters(c) == 0, c);
    freeCalc(c);

    return true;
}

bool test_getCharType(void) {
    /* Digits and decimal point */
    CHECK(getCharType('0') == NUMBER);
    CHECK(getCharType('7') == NUMBER);
    CHECK(getCharType('9') == NUMBER);
    CHECK(getCharType('.') == NUMBER);

    /* Operations */
    CHECK(getCharType('+') == OPERATION);
    CHECK(getCharType('-') == OPERATION);
    CHECK(getCharType('*') == OPERATION);
    CHECK(getCharType('/') == OPERATION);
    CHECK(getCharType('^') == OPERATION);
    CHECK(getCharType('%') == OPERATION);

    /* Delimiters */
    CHECK(getCharType('(') == DELIMITER);
    CHECK(getCharType(')') == DELIMITER);
    CHECK(getCharType('[') == DELIMITER);
    CHECK(getCharType(']') == DELIMITER);
    CHECK(getCharType('{') == DELIMITER);
    CHECK(getCharType('}') == DELIMITER);

    /* Variables */
    CHECK(getCharType('x') == VARIABLE);
    CHECK(getCharType('a') == VARIABLE);
    CHECK(getCharType('Z') == VARIABLE);

    return true;
}

bool test_isUnaryOperation(void) {
    CHECK(isUnaryOperation("sin"));
    CHECK(isUnaryOperation("cos"));
    CHECK(isUnaryOperation("tan"));

    CHECK(!isUnaryOperation("+"));
    CHECK(!isUnaryOperation("42"));
    CHECK(!isUnaryOperation("xyz"));
    CHECK(!isUnaryOperation("("));

    return true;
}

bool test_getTokenType(void) {
    CHECK(getTokenType("0") == NUMBER);
    CHECK(getTokenType("42") == NUMBER);
    CHECK(getTokenType("100.5") == NUMBER);
    CHECK(getTokenType("-3.14") == NUMBER);

    CHECK(getTokenType("+") == OPERATION);
    CHECK(getTokenType("-") == OPERATION);
    CHECK(getTokenType("*") == OPERATION);
    CHECK(getTokenType("/") == OPERATION);
    CHECK(getTokenType("^") == OPERATION);
    CHECK(getTokenType("%") == OPERATION);

    CHECK(getTokenType("(") == DELIMITER);
    CHECK(getTokenType("}") == DELIMITER);
    CHECK(getTokenType("[") == DELIMITER);

    CHECK(getTokenType("sin") == SINGLEOPERATION);
    CHECK(getTokenType("cos") == SINGLEOPERATION);
    CHECK(getTokenType("tan") == SINGLEOPERATION);

    CHECK(getTokenType("x") == VARIABLE);

    return true;
}

bool test_stringToDouble(void) {
    CHECK(approx_eq(stringToDouble("0"), 0.0, 1e-9));
    CHECK(approx_eq(stringToDouble("42"), 42.0, 1e-9));
    CHECK(approx_eq(stringToDouble("42.5"), 42.5, 1e-9));
    CHECK(approx_eq(stringToDouble("-3.14"), -3.14, 1e-9));
    CHECK(approx_eq(stringToDouble("0.001"), 0.001, 1e-9));
    CHECK(approx_eq(stringToDouble("123456.789"), 123456.789, 1e-6));
    return true;
}

bool test_doubleToString(void) {
    double vals[] = {0.0, 1.0, -5.5, 9.99, 12345.678, -0.0025};
    size_t n = sizeof(vals) / sizeof(vals[0]);

    for (size_t i = 0; i < n; i++) {
        char *s = doubleToString(vals[i]);
        CHECK(s != NULL);
        double back = stringToDouble(s);
        free(s);
        CHECK(approx_eq(back, vals[i], 1e-4));
    }
    return true;
}

bool test_combineDigits(void) {
    /* Multi-digit integer & operator tokenization */
    const char *w1[] = {"12+34"};
    Calc *c1 = create_test_calc(1, w1);
    combineDigits(c1);
    CHECK_CALC(getCalcSize(c1) == 4, c1);
    CHECK_CALC(strcmp(c1->word[0], "12") == 0, c1);
    CHECK_CALC(strcmp(c1->word[1], "+") == 0, c1);
    CHECK_CALC(strcmp(c1->word[2], "34") == 0, c1);
    freeCalc(c1);

    /* Decimals and delimiters */
    const char *w2[] = {"(3.5*2)"};
    Calc *c2 = create_test_calc(1, w2);
    combineDigits(c2);
    CHECK_CALC(getCalcSize(c2) == 6, c2);
    CHECK_CALC(strcmp(c2->word[0], "(") == 0, c2);
    CHECK_CALC(strcmp(c2->word[1], "3.5") == 0, c2);
    CHECK_CALC(strcmp(c2->word[2], "*") == 0, c2);
    CHECK_CALC(strcmp(c2->word[3], "2") == 0, c2);
    CHECK_CALC(strcmp(c2->word[4], ")") == 0, c2);
    freeCalc(c2);

    return true;
}

/* ============================================================================
 * 3. Evaluator Tests (calc_eval.h)
 * ============================================================================ */

bool test_hasMixedOperations(void) {
    /* Mixed: + and * */
    const char *w1[] = {"1", "+", "2", "*", "3"};
    Calc *c = create_test_calc(5, w1);
    CHECK_CALC(hasMixedOperations(c), c);
    freeCalc(c);

    /* Mixed: - and / */
    const char *w2[] = {"10", "-", "4", "/", "2"};
    c = create_test_calc(5, w2);
    CHECK_CALC(hasMixedOperations(c), c);
    freeCalc(c);

    /* Only additive */
    const char *w3[] = {"1", "+", "2", "-", "3"};
    c = create_test_calc(5, w3);
    CHECK_CALC(!hasMixedOperations(c), c);
    freeCalc(c);

    /* Only multiplicative */
    const char *w4[] = {"2", "*", "3", "/", "4"};
    c = create_test_calc(5, w4);
    CHECK_CALC(!hasMixedOperations(c), c);
    freeCalc(c);

    /* Single number, no operations */
    const char *w5[] = {"42"};
    c = create_test_calc(1, w5);
    CHECK_CALC(!hasMixedOperations(c), c);
    freeCalc(c);

    return true;
}

bool test_applyBinaryOperator(void) {
    CHECK(approx_eq(applyBinaryOperator(4.0, '+', 2.0, NULL), 6.0, 1e-9));
    CHECK(approx_eq(applyBinaryOperator(-4.0, '+', 2.0, NULL), -2.0, 1e-9));
    CHECK(approx_eq(applyBinaryOperator(4.0, '-', 2.0, NULL), 2.0, 1e-9));
    CHECK(approx_eq(applyBinaryOperator(2.0, '-', 5.0, NULL), -3.0, 1e-9));
    CHECK(approx_eq(applyBinaryOperator(4.0, '*', 2.5, NULL), 10.0, 1e-9));
    CHECK(approx_eq(applyBinaryOperator(-3.0, '*', -4.0, NULL), 12.0, 1e-9));
    CHECK(approx_eq(applyBinaryOperator(9.0, '/', 2.0, NULL), 4.5, 1e-9));
    CHECK(approx_eq(applyBinaryOperator(2.0, '^', 3.0, NULL), 8.0, 1e-9));
    CHECK(approx_eq(applyBinaryOperator(5.0, '^', 0.0, NULL), 1.0, 1e-9));
    return true;
}

bool test_getOperatorPrecedence(void) {
    /* Higher precedence has smaller rank; unrecognized returns 99 */
    CHECK(getOperatorPrecedence("^") < getOperatorPrecedence("*"));
    CHECK(getOperatorPrecedence("*") < getOperatorPrecedence("+"));
    CHECK(getOperatorPrecedence("/") < getOperatorPrecedence("-"));
    CHECK(getOperatorPrecedence("*") == getOperatorPrecedence("/"));
    CHECK(getOperatorPrecedence("+") == getOperatorPrecedence("-"));
    CHECK(getOperatorPrecedence("unknown") == 99);
    CHECK(getOperatorPrecedence("42") == 99);
    return true;
}

bool test_hasHigherPrecedence(void) {
    CHECK(hasHigherPrecedence("^", "*"));
    CHECK(hasHigherPrecedence("*", "+"));
    CHECK(hasHigherPrecedence("/", "-"));
    CHECK(!hasHigherPrecedence("+", "*"));
    CHECK(!hasHigherPrecedence("-", "/"));
    CHECK(!hasHigherPrecedence("+", "-"));
    CHECK(!hasHigherPrecedence("*", "/"));
    return true;
}

bool test_findHighestPrecedenceOp(void) {
    /* * beats + */
    const char *w1[] = {"1", "+", "2", "*", "3"};
    Calc *c = create_test_calc(5, w1);
    uint idx = 0;
    CHECK_CALC(findHighestPrecedenceOp(c, &idx) && idx == 3, c);
    freeCalc(c);

    /* ^ beats * and + */
    const char *w2[] = {"1", "+", "2", "*", "3", "^", "2"};
    c = create_test_calc(7, w2);
    idx = 0;
    CHECK_CALC(findHighestPrecedenceOp(c, &idx) && idx == 5, c);
    freeCalc(c);

    /* Same precedence -> picks leftmost */
    const char *w3[] = {"10", "-", "3", "+", "2"};
    c = create_test_calc(5, w3);
    idx = 0;
    CHECK_CALC(findHighestPrecedenceOp(c, &idx) && idx == 1, c);
    freeCalc(c);

    /* No operations -> returns false */
    const char *w4[] = {"42"};
    c = create_test_calc(1, w4);
    idx = 99;
    CHECK_CALC(!findHighestPrecedenceOp(c, &idx) && idx == 99, c);
    freeCalc(c);

    return true;
}

bool test_evalUnaryOperation(void) {
    char *r1 = evalUnaryOperation("sin", "(0)");
    CHECK(r1 != NULL);
    CHECK(approx_eq(stringToDouble(r1), 0.0, 1e-4));
    free(r1);

    char *r2 = evalUnaryOperation("cos", "(0)");
    CHECK(r2 != NULL);
    CHECK(approx_eq(stringToDouble(r2), 1.0, 1e-4));
    free(r2);

    char *r3 = evalUnaryOperation("tan", "(0)");
    CHECK(r3 != NULL);
    CHECK(approx_eq(stringToDouble(r3), 0.0, 1e-4));
    free(r3);
    return true;
}

bool test_solveUnaryOperation(void) {
    const char *w[] = {"cos", "(0)"};
    Calc *c = create_test_calc(2, w);
    char *res = solveUnaryOperation(c, 0);
    FILE *f =fopen("log","a");
    setbuf(f, NULL);
    fprintf(f,"0\n");
    CHECK_CALC(res != NULL, c);
    double val = stringToDouble(res);
    fprintf(f,"%s\n",res);
    free(res);
    freeCalc(c);
    fprintf(f,"%f\n",val);
    CHECK(approx_eq(val, 1.0, 1e-4));
    fprintf(f,"1\n");
    fclose(f);
    return true;
}

bool test_solveFlatExpression(void) {
    /* Precedence: 2 + 3 * 4 = 14 */
    const char *w1[] = {"2", "+", "3", "*", "4"};
    Calc *c1 = create_test_calc(5, w1);
    char *r1 = solveFlatExpression(c1);
    CHECK_CALC(r1 != NULL, c1);
    double v1 = stringToDouble(r1);
    free(r1);
    freeCalc(c1);
    CHECK(approx_eq(v1, 14.0, 1e-6));

    /* Power + subtraction + division: 2 ^ 3 + 10 / 2 - 3 = 10 */
    const char *w2[] = {"2", "^", "3", "+", "10", "/", "2", "-", "3"};
    Calc *c2 = create_test_calc(9, w2);
    char *r2 = solveFlatExpression(c2);
    CHECK_CALC(r2 != NULL, c2);
    double v2 = stringToDouble(r2);
    free(r2);
    freeCalc(c2);
    CHECK(approx_eq(v2, 10.0, 1e-6));

    /* Left-to-right subtraction: 10 - 3 - 2 = 5 */
    const char *w3[] = {"10", "-", "3", "-", "2"};
    Calc *c3 = create_test_calc(5, w3);
    char *r3 = solveFlatExpression(c3);
    CHECK_CALC(r3 != NULL, c3);
    double v3 = stringToDouble(r3);
    free(r3);
    freeCalc(c3);
    CHECK(approx_eq(v3, 5.0, 1e-6));

    /* Single number: 42 */
    const char *w4[] = {"42"};
    Calc *c4 = create_test_calc(1, w4);
    char *r4 = solveFlatExpression(c4);
    CHECK_CALC(r4 != NULL, c4);
    double v4 = stringToDouble(r4);
    free(r4);
    freeCalc(c4);
    CHECK(approx_eq(v4, 42.0, 1e-6));

    return true;
}

bool test_evalDelimitedExpression(void) {
    /* Simple group: ( 2 + 3 ) * 4 -> 5 * 4 */
    const char *w1[] = {"(", "2", "+", "3", ")", "*", "4"};
    Calc *c1 = create_test_calc(7, w1);
    evalDelimitedExpression(c1);
    CHECK_CALC(c1->word[0] != NULL, c1);
    CHECK_CALC(approx_eq(stringToDouble(c1->word[0]), 5.0, 1e-6), c1);
    CHECK_CALC(strcmp(c1->word[1], "*") == 0, c1);
    CHECK_CALC(strcmp(c1->word[2], "4") == 0, c1);
    freeCalc(c1);

    /* Nested brackets: [ ( 1 + 2 ) * 3 ] -> 9 */
    const char *w2[] = {"[", "(", "1", "+", "2", ")", "*", "3", "]"};
    Calc *c2 = create_test_calc(9, w2);
    evalDelimitedExpression(c2);
    CHECK_CALC(c2->word[0] != NULL, c2);
    CHECK_CALC(approx_eq(stringToDouble(c2->word[0]), 9.0, 1e-6), c2);
    freeCalc(c2);

    return true;
}

bool test_evalUnaryOperations(void) {
    const char *w[] = {"cos", "(0)"};
    Calc *c = create_test_calc(2, w);
    evalUnaryOperations(c);
    CHECK_CALC(c->word[0] != NULL, c);
    CHECK_CALC(approx_eq(stringToDouble(c->word[0]), 1.0, 1e-4), c);
    freeCalc(c);
    return true;
}

bool test_solveString(void) {
    char *r1 = solveString("2+3*4");
    CHECK(r1 != NULL);
    CHECK(approx_eq(stringToDouble(r1), 14.0, 1e-6));
    free(r1);

    char *r2 = solveString("(2+3)*4");
    CHECK(r2 != NULL);
    CHECK(approx_eq(stringToDouble(r2), 20.0, 1e-6));
    free(r2);

    char *r3 = solveString("((1+2)*3)^2");
    CHECK(r3 != NULL);
    CHECK(approx_eq(stringToDouble(r3), 81.0, 1e-6));
    free(r3);

    return true;
}

bool test_solve(void) {
    /* Basic parentheses */
    const char *w1[] = {"(", "2", "+", "3", ")", "*", "4"};
    char *r = run_solve(7, w1);
    CHECK(r != NULL);
    CHECK(approx_eq(stringToDouble(r), 20.0, 1e-6));
    free(r);

    /* Deeply nested mixed delimiters: { [ ( 2 + 3 ) * 2 ] - 4 } / 3 = 2 */
    const char *w2[] = {"{", "[", "(", "2", "+", "3", ")", "*", "2", "]", "-", "4", "}", "/", "3"};
    r = run_solve(15, w2);
    CHECK(r != NULL);
    CHECK(approx_eq(stringToDouble(r), 2.0, 1e-6));
    free(r);

    /* Single compact string argument (like interactive mode) */
    const char *w3[] = {"10+20*3-5"};
    r = run_solve(1, w3);
    CHECK(r != NULL);
    CHECK(approx_eq(stringToDouble(r), 65.0, 1e-6));
    free(r);

    /* Invalid delimiters -> must return NULL */
    const char *w4[] = {"(", "2", "+", "3"};
    r = run_solve(4, w4);
    CHECK(r == NULL);

    /* Empty input -> must return NULL */
    char *empty[] = {NULL};
    r = solve(empty);
    CHECK(r == NULL);

    return true;
}

/* ============================================================================
 * 4. Math Tests (math.h)
 * ============================================================================ */

bool test_power(void) {
    CHECK(approx_eq(power(2.0, 0.0), 1.0, 1e-9));
    CHECK(approx_eq(power(2.0, -3.0), 1.0, 1e-9));
    CHECK(approx_eq(power(2.0, 1.0), 2.0, 1e-9));
    CHECK(approx_eq(power(2.0, 10.0), 1024.0, 1e-6));
    CHECK(approx_eq(power(3.0, 3.0), 27.0, 1e-6));
    CHECK(approx_eq(power(-2.0, 3.0), -8.0, 1e-6));
    CHECK(approx_eq(power(-2.0, 4.0), 16.0, 1e-6));
    return true;
}

bool test_mod(void) {
    /* Per math.h: repeatedly subtracts last from first while first > last, returns last */
    CHECK(approx_eq(mod(10.0, 3.0), 1.0, 1e-9));
    CHECK(approx_eq(mod(2.0, 5.0), 2.0, 1e-9));
    CHECK(approx_eq(mod(6.0, 6.0), 6.0, 1e-9));
    
    return true;
}

bool test_computeTrig(void) {
    /* Radians */
    CHECK(approx_eq(computeTrig(0.0, true, false), 0.0, 1e-6));
    CHECK(approx_eq(computeTrig(0.0, false, false), 1.0, 1e-6));
    CHECK(approx_eq(computeTrig(PI / 2.0, true, false), 1.0, 1e-5));
    CHECK(approx_eq(computeTrig(PI / 2.0, false, false), 0.0, 1e-5));
    CHECK(approx_eq(computeTrig(PI, true, false), 0.0, 1e-5));
    CHECK(approx_eq(computeTrig(PI, false, false), -1.0, 1e-5));

    /* Degrees */
    CHECK(approx_eq(computeTrig(0.0, true, true), 0.0, 1e-6));
    CHECK(approx_eq(computeTrig(90.0, true, true), 1.0, 1e-5));
    CHECK(approx_eq(computeTrig(180.0, false, true), -1.0, 1e-5));
    CHECK(approx_eq(computeTrig(30.0, true, true), 0.5, 1e-5));
    CHECK(approx_eq(computeTrig(60.0, false, true), 0.5, 1e-5));

    return true;
}

/* ============================================================================
 * 5. Stack Tests (stack.h)
 * ============================================================================ */

bool test_stack(void) {
    CHECK(StackIsNULL(NULL));
    Stack *s = StackCreateEmpty();
    CHECK(!StackIsNULL(s));
    CHECK(StackIsEmpty(s));
    CHECK(StackSize(s) == 0);
    s = StackPush(s, '(');
    CHECK(!StackIsEmpty(s));
    CHECK(StackSize(s) == 1);
    CHECK(StackGetValue(s) == '(');
    s = StackPush(s, '[');
    s = StackPush(s, '{');
    CHECK(StackSize(s) == 3);
    CHECK(StackGetValue(s) == '{');
    CHECK(StackNext(s) != NULL);
    CHECK(StackGetValue(StackNext(s)) == '[');
    s = StackPop(s);
    CHECK(StackSize(s) == 2);
    CHECK(StackGetValue(s) == '[');
    s = StackPop(s);
    CHECK(StackSize(s) == 1);
    CHECK(StackGetValue(s) == '(');
    s = StackPop(s);
    CHECK(StackIsEmpty(s));
    CHECK(StackSize(s) == 0);
    StackFree(s);
    StackFree(NULL); /* Must be safe on NULL */
    return true;
}

/* ============================================================================
 * Test Registry & Runner
 * ============================================================================ */

typedef struct {
    const char *name;
    bool (*fn)(void);
} TestEntry;

static const TestEntry TESTS[] = {
    /* calc_core */
    {"getCalc",                      test_getCalc},
    {"freeCalc",                     test_freeCalc},
    {"printCalc",                    test_printCalc},
    {"getCalcSize",                     test_getCalcSize},
    {"replaceRangeWithResult",                   test_replaceRangeWithResult},
    /* calc_parser */
    {"isMatchingDelimiter",                   test_isMatchingDelimiter},
    {"areDelimitersValid",           test_areDelimitersValid},
    {"trimTrailingZeros",          test_trimTrailingZeros},
    {"countOpeningDelimiters",       test_countOpeningDelimiters},
    {"getCharType",                     test_getCharType},
    {"isUnaryOperation",            test_isUnaryOperation},
    {"getTokenType",                  test_getTokenType},
    {"stringToDouble",           test_stringToDouble},
    {"doubleToString",           test_doubleToString},
    {"combineDigits",                test_combineDigits},
    /* calc_eval */
    {"hasMixedOperations",               test_hasMixedOperations},
    {"applyBinaryOperator",              test_applyBinaryOperator},
    {"getOperatorPrecedence",            test_getOperatorPrecedence},
    {"hasHigherPrecedence",                       test_hasHigherPrecedence},
    {"findHighestPrecedenceOp",                      test_findHighestPrecedenceOp},
    {"evalUnaryOperation",    test_evalUnaryOperation},
    {"solveUnaryOperation",         test_solveUnaryOperation},
    {"solveFlatExpression",                  test_solveFlatExpression},
    {"evalDelimitedExpression", test_evalDelimitedExpression},
    {"evalUnaryOperations",      test_evalUnaryOperations},
    {"solveString",                     test_solveString},
    {"solve",                        test_solve},
    /* math & stack */
    {"power",                        test_power},
    {"mod",                          test_mod},
    {"computeTrig",                test_computeTrig},
    {"stack",                        test_stack},
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "Error: no argument given!\n");
        return EXIT_FAILURE;
    }

    if (strcmp("dummy", argv[1]) == 0) {
        bool ok = test_dummy(argv[1]);
        fprintf(stderr, "Test \"dummy\" finished: %s\n", ok ? "SUCCESS" : "FAILURE");
        return ok ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    size_t num_tests = sizeof(TESTS) / sizeof(TESTS[0]);

    if (strcmp("all", argv[1]) == 0) {
        size_t passed = 0;
        for (size_t i = 0; i < num_tests; i++) {
            bool ok = TESTS[i].fn();
            fprintf(stderr, "Test \"%s\": %s\n", TESTS[i].name, ok ? "SUCCESS" : "FAILURE");
            if (ok) passed++;
        }
        fprintf(stderr, "\nSummary: %zu/%zu tests passed.\n", passed, num_tests);
        return (passed == num_tests) ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    for (size_t i = 0; i < num_tests; i++) {
        if (strcmp(TESTS[i].name, argv[1]) == 0) {
            bool ok = TESTS[i].fn();
            fprintf(stderr, "Test \"%s\" finished: %s\n", argv[1], ok ? "SUCCESS" : "FAILURE");
            return ok ? EXIT_SUCCESS : EXIT_FAILURE;
        }
    }

    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    return EXIT_FAILURE;
}