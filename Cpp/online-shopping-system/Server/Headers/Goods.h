#pragma once

#include "Admin.h"
using namespace std;

class Goods
{
    // Attributes
    string name;
    int quantity, reorder_level, reorder_amount, stock;
    double price, cost;
    bool bought; // true if item has been bought, false otherwise

    friend class Person;
    friend void Admin::stock(void);
};