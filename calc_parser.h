#ifndef CALC_PARSER_H
#define CALC_PARSER_H

#include "calc_types.h"

/**
 * Determines whether two delimiters are a matching opening and closing pair.
 *
 * @param open The opening delimiter to check: '(', '[' or '{'.
 * @param close The closing delimiter to check: ')', ']' or '}'.
 * @return true if both characters are the corresponding delimiter pair;
 *         otherwise false.
 */
bool isMatchingDelimiter(char open, char close);

/**
 * Checks that the expression's parentheses, square brackets and braces are
 * properly nested and matched.
 *
 * @param calc The calculation whose expression is to be checked.
 * @return true if every opening delimiter has a matching closing delimiter
 *         in the correct order; otherwise false.
 */
bool areDelimitersValid(Calc *calc);

/**
 * Removes trailing zeroes from a decimal string, also removing the decimal
 * point if no fractional digits remain.
 *
 * @param str The mutable string to trim.
 */
void trimTrailingZeros(char *str);

/**
 * Counts the opening parentheses, square brackets and braces in an
 * expression.
 *
 * @param calc The calculation to examine.
 * @return The total number of opening delimiters in the expression.
 */
uint countOpeningDelimiters(Calc *calc);

/**
 * Classifies a single character as part of a number, an operation, a
 * variable, or a delimiter.
 *
 * @param c The character to classify.
 * @return The matching Type value, or 99 if the character is not recognized.
 */
uint getCharType(char c);

/**
 * Checks whether a token is the name of a supported single-argument
 * operation (ln, log, sin, cos or tan).
 *
 * @param c The operation token to check.
 * @return true if c names a supported single-argument operation; otherwise
 *         false.
 */
bool isUnaryOperation(char *c);

/**
 * Classifies a word in an expression.
 *
 * @param c The word to classify. Single-character words are classified by
 *          their character; supported named operations are classified as
 *          SINGLEOPERATION, and other longer words are treated as numbers.
 * @return The matching Type value, or 99 for an unrecognized one-character
 *         word.
 */
uint getTokenType(char *c);


/**
 * Converts a numeric string to a double.
 *
 * @param c A string containing a signed decimal number. Either a period or a
 *          comma may be used as the decimal separator.
 * @return The numeric value represented by the string.
 */
double stringToDouble(char *c);

/**
 * Creates a dynamically allocated decimal string representation of a
 * floating-point value.
 *
 * @param number The value to represent.
 * @return A newly allocated string, or NULL if memory allocation fails. The
 *         caller is responsible for freeing the returned string.
 */
char *doubleToString(double number);

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
void parseAndEvalArgs(char *number, char **value);


void combineDigits(Calc *calc);

#endif /* CALC_PARSER_H */