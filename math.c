#include "math.h"

double power(double first, double last) {
    double result = 1;
    while (last > 0) {
        result *= first;
        last--;
    }
    return result;
}

double mod(double first, double last) {
    while (first < 0) {
        first += last;
    }
    while (first > last) {
        first -= last;
    }
    return first;
}

double trigonometrie(double nb, bool isSin, bool is_deg) {
    if (is_deg) {
        nb = nb * (PI / 180.0);
    }
    
    nb = mod(nb, 2 * PI);

    double fact = 1.0;
    bool plus = true;
    double result = 0.0;
    uint i = isSin ? 1 : 0;

    for (; i < PRECISION * 2; i += 2) {
        if (plus) {
            result += power(nb, i) / fact;
        } else {
            result -= power(nb, i) / fact;
        }

        fact = fact * (i + 1);
        fact = fact * (i + 2);
        plus = !plus;
    }

    return result;
}