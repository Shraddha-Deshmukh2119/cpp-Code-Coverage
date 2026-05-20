#pragma once
#include "Employee.h"

class Admin: public Employee
{
    protected:

    void home() override;
    void add_employee(void);
    void view(const string &head);
    void search();
    void search(const string &str);
    void Delete();
    void balance();
    void accounts();

    public:
        void stock(void); // friend of goods
        Admin(Client& client) : Employee(client) {}
};