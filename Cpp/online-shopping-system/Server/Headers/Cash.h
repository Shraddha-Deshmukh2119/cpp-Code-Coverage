#pragma once

class Cash
{
    double initial, in, out, finl;
    friend class Person;

public:
    double get_final_cash(void);

    void operator+(double i);
    void operator-(double i);
};
