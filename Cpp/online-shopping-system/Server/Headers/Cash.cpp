#include "Cash.h"

void Cash::operator+(double i)
{
    in += i;
    finl += i;
}

void Cash::operator-(double i)
{
    out += i;
    finl -= i;
}

double Cash::get_final_cash(void)
{
    return finl;
}
