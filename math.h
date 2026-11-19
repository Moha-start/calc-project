#include "calc.h"
#define PI 3.14159265358979323846
#define E 2.71828182845904523536
#define PRECISION 100
/**
 * Computes a repeated-multiplication power.
 *
 * Multiplication continues while last is positive, decrementing it by one
 * each iteration. Consequently, positive fractional exponents use a ceiling
 * number of multiplications, and zero or negative exponents return 1.
 *
 * @param first The value to multiply.
 * @param last The positive iteration count or exponent.
 * @return The result of repeated multiplication.
 */
double power(double first,double last);

/**
 * Repeatedly subtracts last from first while first is greater than last, then
 * returns last. This is not a conventional remainder operation. If
 * first > last, last must be positive for the loop to terminate.
 *
 * @param first The initial value to compare and reduce.
 * @param last The value subtracted during each iteration and returned.
 * @return last.
 */
double mod(double first ,double last);

/**
 * Approximates the sine or cosine of an angle in radians with a finite Taylor
 * series using the configured PRECISION.
 *
 * @param nb The angle.
 * @param isSin true to approximate sine; false to approximate cosine.
 * @param is_deg true is the angle is in degres and false if it is in radian.
 * @return The computed approximation.
 */
double computeTrig(double nb,bool isSin,bool is_deg);