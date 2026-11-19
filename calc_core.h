#ifndef CALC_CORE_H
#define CALC_CORE_H

#include "calc_types.h"

/**
 * Prompts for and reads an expression from standard input.
 *
 * Spaces in the entered line are ignored. The returned object contains the
 * expression as one string in its NULL-terminated word array.
 *
 * @return A newly allocated Calc containing the entered expression. The
 *         caller is responsible for releasing it with freeCalc().
 * @note Exits with EXIT_FAILURE if memory cannot be allocated or if EOF is
 *       reached before any input is read.
 */
Calc * getCalc(void);

/**
 * Releases a Calc and all expression strings owned by it.
 *
 * @param calc The calculation to release. It must be a valid Calc pointer.
 */
void freeCalc(Calc *calc);

/**
 * Writes the calculation's words to standard output for inspection.
 *
 * @param calc The calculation to display.
 */
void printCalc(Calc *calc);

/**
 * Returns the number of entries in a calculation's word array, including
 * its terminating NULL entry.
 *
 * @param calc The calculation whose array size is being counted.
 * @return The number of words plus one for the NULL terminator.
 */
uint getCalcSize(Calc *calc);

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
void replaceRangeWithResult(Calc *calc, uint first, uint last, char *result);

#endif /* CALC_CORE_H */