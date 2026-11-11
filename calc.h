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
} Type; 
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
 * Counts the opening parentheses, square brackets and braces in an
 * expression.
 *
 * @param calc The calculation to examine.
 * @return The total number of opening delimiters in the expression.
 */
uint countOpeningDelimiters(Calc *calc);

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
 * Classifies a word in an expression.
 *
 * @param c The word to classify. A one-character word is classified by its
 *          character; a longer word is treated as a number.
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
 * Applies one basic arithmetic operation to two values.
 *
 * @param first The left-hand operand.
 * @param operation The operator: '+', '-', '*' or '/'.
 * @param last The right-hand operand.
 * @return The result of applying operation to first and last.
 */
double calculateSimple(double first, char operation, double last,Calc * calc);

/**
 * Finds the next multiplication or division operation in an expression.
 *
 * @param calc The calculation to search.
 * @param index Output location for the index of the first '*' or '/' word.
 *              It is written only when such an operation is found.
 * @return true if a multiplication or division was found; otherwise false.
 */
bool isPrio(Calc *calc, uint *index);

/**
 * Evaluates an expression that contains numeric operands and basic arithmetic
 * operators, applying multiplication and division before addition and
 * subtraction.
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
void evaluateParenthesizedExpression(Calc *calc);

/**
 * Evaluates an arithmetic expression supplied as a NULL-terminated array of
 * words.
 *
 * The words are copied before processing, so the input array and its strings
 * remain owned by the caller. Delimiters are checked and arithmetic is
 * evaluated with multiplication and division taking precedence over addition
 * and subtraction.
 *
 * @param words The expression words, terminated by a NULL pointer.
 * @return A newly allocated string containing the numeric result, or NULL if
 *         the expression is empty, delimiters are invalid, or allocation
 *         fails. The caller is responsible for freeing the returned string.
 */
char *solve(char **words);

void trim_trailing_zeros(char *str);
#endif /* CALC_H */