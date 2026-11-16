#ifndef CALC_H
#define CALC_H

typedef unsigned int uint;

#include <stdbool.h>
#include "stack.h"
typedef enum Type {
    NUMBER =0 ,
    OPERATION,
    VARIABLE,
    DELIMITER,
    SINGLEOPERATION,
} Type; 
#define PI 3.14159265358979323846
#define E 2.71828182845904523536
#define PRECISION 100
/**
 * Holds an expression as a NULL-terminated array of strings.
 *
 * Each string may represent a character or a larger token, depending on
 * whether the expression has been tokenized. The Calc object owns the array
 * and its strings.
 */
typedef struct {
    char **word;
} Calc;



/**
 * Prompts for and reads an expression from standard input.
 *
 * Spaces in the entered line are ignored. The returned object contains the
 * expression as one string in its NULL-terminated word array.
 *
 * @return A newly allocated Calc containing the entered expression. The
 *         caller is responsible for releasing it with FreeCalc().
 * @note Exits with EXIT_FAILURE if memory cannot be allocated or if EOF is
 *       reached before any input is read.
 */
Calc * getCalc(void);

/**
 * Determines whether two delimiters are a matching opening and closing pair.
 *
 * @param open The opening delimiter to check: '(', '[' or '{'.
 * @param close The closing delimiter to check: ')', ']' or '}'.
 * @return true if both characters are the corresponding delimiter pair;
 *         otherwise false.
 */
bool IsSameOpen(char open, char close);

/**
 * Releases a Calc and all expression strings owned by it.
 *
 * @param calc The calculation to release. It must be a valid Calc pointer.
 */
void FreeCalc(Calc *calc);

/**
 * Checks that the expression's parentheses, square brackets and braces are
 * properly nested and matched.
 *
 * @param calc The calculation whose expression is to be checked.
 * @return true if every opening delimiter has a matching closing delimiter
 *         in the correct order; otherwise false.
 */
bool AreDelimitersValid(Calc *calc);

/**
 * Removes trailing zeroes from a decimal string, also removing the decimal
 * point if no fractional digits remain.
 *
 * @param str The mutable string to trim.
 */
void trim_trailing_zeros(char *str);

/**
 * Counts the opening parentheses, square brackets and braces in an
 * expression.
 *
 * @param calc The calculation to examine.
 * @return The total number of opening delimiters in the expression.
 */
uint countOpeningDelimiters(Calc *calc);

/**
 * Evaluates a single string as an expression.
 *
 * @param c The expression string to evaluate. The string remains owned by
 *          the caller.
 * @return A newly allocated result string, or NULL if evaluation fails. The
 *         caller is responsible for freeing the returned string.
 */
char *solveStr(char *c);

/**
 * Writes the calculation's words to standard output for inspection.
 *
 * @param calc The calculation to display.
 */
void printCalc(Calc *calc);

/**
 * Classifies a single character as part of a number, an operation, a
 * variable, or a delimiter.
 *
 * @param c The character to classify.
 * @return The matching Type value, or 99 if the character is not recognized.
 */
uint calctype(char c);

/**
 * Checks whether a token is the name of a supported single-argument
 * operation (ln, log, sin, cos or tan).
 *
 * @param c The operation token to check.
 * @return true if c names a supported single-argument operation; otherwise
 *         false.
 */
bool isSingleOperation(char *c);

/**
 * Classifies a word in an expression.
 *
 * @param c The word to classify. Single-character words are classified by
 *          their character; supported named operations are classified as
 *          SINGLEOPERATION, and other longer words are treated as numbers.
 * @return The matching Type value, or 99 for an unrecognized one-character
 *         word.
 */
uint getCalctype(char *c);

/**
 * Returns the number of entries in a calculation's word array, including
 * its terminating NULL entry.
 *
 * @param calc The calculation whose array size is being counted.
 * @return The number of words plus one for the NULL terminator.
 */
uint sizeCalc(Calc *calc);

/**
 * Replaces a range of expression words with one result word.
 *
 * The supplied result string becomes owned by the Calc and the replaced
 * words are released.
 *
 * @param calc The calculation to update.
 * @param first Index of the first word to replace.
 * @param last Index of the last word to replace, inclusive.
 * @param result The replacement string, which must be dynamically allocated
 *               and must not alias a word already in the calculation.
 */
void moveResult(Calc *calc, uint first, uint last, char *result);

/**
 * Converts a numeric string to a double.
 *
 * @param c A string containing a signed decimal number. Either a period or a
 *          comma may be used as the decimal separator.
 * @return The numeric value represented by the string.
 */
double fromStringToDouble(char *c);

/**
 * Creates a dynamically allocated decimal string representation of a
 * floating-point value.
 *
 * @param number The value to represent.
 * @return A newly allocated string, or NULL if memory allocation fails. The
 *         caller is responsible for freeing the returned string.
 */
char *fromDoobleToString(double number);

/**
 * Determines whether an expression contains both additive and
 * multiplicative operations.
 *
 * @param calc The calculation to examine.
 * @return true if the expression contains at least one '+' or '-' and at
 *         least one '*' or '/'; otherwise false.
 */
bool isMixOperation(Calc *calc);

/**
 * Applies one arithmetic operation to two values.
 *
 * @param first The left-hand operand.
 * @param operation The operator: '+', '-', '*', '/', '^' or '%'.
 * @param last The right-hand operand.
 * @return The result of applying operation to first and last.
 */
double calculateSimple(double first, char operation, double last,Calc * calc);

/**
 * Finds the highest-precedence operation in an expression.
 *
 * @param calc The calculation to search.
 * @param index Output location for the index of the selected operation. It
 *              is written only when an operation is found.
 * @return true if an operation was found; otherwise false.
 */
bool getPrio(Calc *calc, uint *index);

/**
 * Returns the precedence rank of an operation token; smaller values indicate
 * higher precedence.
 *
 * @param c The operation token.
 * @return Its precedence rank, or 99 if it is not a recognized operation.
 */
uint getValueOpeartion(char *c);

/**
 * Checks whether the first operation token has strictly higher precedence
 * than the second.
 *
 * @param c1 The first operation token.
 * @param c2 The second operation token.
 * @return true if c1 has higher precedence than c2; otherwise false.
 */
bool isPrio(char *c1, char *c2);

/**
 * Splits a parenthesized argument into one or two expressions and evaluates
 * them.
 *
 * @param number The argument string, beginning with '('; a comma separates
 *               the optional second expression.
 * @param value Output array with room for two result pointers. The first
 *              result is stored in value[0]; when a comma is present, the
 *              second is stored in value[1]. Returned strings are
 *              dynamically allocated and owned by the caller.
 * @note Exits with EXIT_FAILURE if the argument does not begin with '(' or
 *       memory allocation fails.
 */
void fromStringToDoubleDelimiteur(char *number, char **value);

/**
 * Evaluates a named single-argument operation on a numeric string.
 *
 * @param opearation The operation name.
 * @param number The numeric argument as a string.
 * @return A newly allocated result string, or NULL if allocation fails.
 */
char *calculateSingleOpearation(char *opearation, char *number);

/**
 * Solves the operand immediately following the operation at index and applies
 * that operation.
 *
 * @param calc The tokenized calculation.
 * @param index Index of the named operation token.
 * @return A newly allocated result string, or NULL if allocation fails.
 */
char *solveSingleOperation(Calc *calc, uint index);

/**
 * Evaluates an expression containing numeric operands and arithmetic
 * operators, applying higher-precedence operations before lower-precedence
 * ones.
 *
 * @param calc The calculation to evaluate; its words must already represent
 *             individual operands and operators.
 * @return A newly allocated string containing the result, or NULL if
 *         allocation fails. The caller is responsible for freeing it.
 */
char *solveSimple(Calc *calc);

/**
 * Evaluates the first parenthesized, bracketed or braced subexpression in a
 * calculation and replaces that group with its result.
 *
 * @param calc The calculation to update. Its delimiters are expected to be
 *             valid and its words tokenized.
 */
void evaluateDelimitersExpression(Calc *calc);

/**
 * Evaluates named operations whose argument is a parenthesized token and
 * replaces each operation and argument in the calculation with its result.
 *
 * @param calc The tokenized calculation to update.
 */
void evaluateSimpleOperation(Calc *calc);

/**
 * Evaluates an arithmetic expression supplied as a NULL-terminated array of
 * words.
 *
 * The words are copied before processing, so the input array and its strings
 * remain owned by the caller. Delimiters are checked and arithmetic is
 * evaluated according to operator precedence.
 *
 * @param words The expression words, terminated by a NULL pointer.
 * @return A newly allocated string containing the numeric result, or NULL if
 *         the expression is empty, delimiters are invalid, or allocation
 *         fails. The caller is responsible for freeing the returned string.
 */
char *solve(char **words);

#endif /* CALC_H */