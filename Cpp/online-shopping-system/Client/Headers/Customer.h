#pragma once
#include "Person.h"

class Customer: public Person
{   
    void signup();
    void home() override;
    void complain();

    public:
        Customer(Client& client) : Person(client) {}
        void start();
};
