#ifndef CALC_EVAL_H
#define CALC_EVAL_H

#include <stdbool.h>
#include "calc_types.h"

/**
 * Evaluates a single string as an expression.
 *
 * @param c The expression string to evaluate. The string remains owned by
 *          the caller.
 * @return A newly allocated result string, or NULL if evaluation fails. The
 *         caller is responsible for freeing the returned string.
 */
char *solveString(char *c);

/**
 * Determines whether an expression contains both additive and
 * multiplicative operations.
 *
 * @param calc The calculation to examine.
 * @return true if the expression contains at least one '+' or '-' and at
 *         least one '*' or '/'; otherwise false.
 */
bool hasMixedOperations(Calc *calc);

/**
 * Applies one arithmetic operation to two values.
 *
 * @param first The left-hand operand.
 * @param operation The operator: '+', '-', '*', '/', '^' or '%'.
 * @param last The right-hand operand.
 * @return The result of applying operation to first and last.
 */
double applyBinaryOperator(double first, char operation, double last, Calc *calc);

/**
 * Finds the highest-precedence operation in an expression.
 *
 * @param calc The calculation to search.
 * @param index Output location for the index of the selected operation. It
 *              is written only when an operation is found.
 * @return true if an operation was found; otherwise false.
 */
bool findHighestPrecedenceOp(Calc *calc, uint *index);

/**
 * Returns the precedence rank of an operation token; smaller values indicate
 * higher precedence.
 *
 * @param c The operation token.
 * @return Its precedence rank, or 99 if it is not a recognized operation.
 */
uint getOperatorPrecedence(char *c);

/**
 * Checks whether the first operation token has strictly higher precedence
 * than the second.
 *
 * @param c1 The first operation token.
 * @param c2 The second operation token.
 * @return true if c1 has higher precedence than c2; otherwise false.
 */
bool hasHigherPrecedence(char *c1, char *c2);

/**
 * Evaluates a named single-argument operation on a numeric string.
 *
 * @param opearation The operation name.
 * @param number The numeric argument as a string.
 * @return A newly allocated result string, or NULL if allocation fails.
 */
char *evalUnaryOperation(char *opearation, char *number);

/**
 * Solves the operand immediately following the operation at index and applies
 * that operation.
 *
 * @param calc The tokenized calculation.
 * @param index Index of the named operation token.
 * @return A newly allocated result string, or NULL if allocation fails.
 */
char *solveUnaryOperation(Calc *calc, uint index);

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
char *solveFlatExpression(Calc *calc);

/**
 * Evaluates the first parenthesized, bracketed or braced subexpression in a
 * calculation and replaces that group with its result.
 *
 * @param calc The calculation to update. Its delimiters are expected to be
 *             valid and its words tokenized.
 */
void evalDelimitedExpression(Calc *calc);

/**
 * Evaluates named operations whose argument is a parenthesized token and
 * replaces each operation and argument in the calculation with its result.
 *
 * @param calc The tokenized calculation to update.
 */
void evalUnaryOperations(Calc *calc);

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

#endif /* CALC_EVAL_H */