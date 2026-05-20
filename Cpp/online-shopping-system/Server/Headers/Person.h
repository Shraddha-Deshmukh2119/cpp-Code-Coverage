#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <conio.h>
#include <cstdio>
#include <cstring>
#include "Server.h"
using namespace std;

// Number of goods in the catalog.
static constexpr int GOODS_COUNT = 10;

// Forward declaration of classes
class Person;
class Employee;
class Customer;
class Admin;
class Goods;
class Cash;

const string customerFile = "customer.txt";
const string adminFile = "admin.txt";
const string empFile = "emp.txt";

struct Date
{
    string day, month, year;
};

class Person
{
    protected:
        string name, CNIC, email, phone_num, username, password;
        string sex;
        string age;
        string balance;
        Date DOB;
        string lookup;
        Thread *server;

        bool input(const string& fileName);
        bool transfer_to_file(const string& fileName);
        
        // pure virtual function
        virtual void home() = 0; 

        // Methods for initialaizing and updating
        bool initialize_goods(Goods* goods);
        bool initialize_balance(string fileName);
        bool initialize_cash(Cash& cash);
        bool update_goods(Goods* goods);
        bool update_balance(string fileName);
        bool update_cash(Cash& cash);

        // Other methods
        void profile(const string& fileName, const string& Lookup);
        void buy(const string& fileName);

    public:
        Person(Thread& server) : server(&server) {}
        void login(const string& fileName);   
};
