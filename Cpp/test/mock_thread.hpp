#pragma once

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

#include <queue>
#include <sstream>
#include <string>
#include <vector>

#include "Admin.h"
#include "Customer.h"
#include "Thread.h"

class MockThread : public Thread
{
public:
    mutable std::vector<std::string> sent;
    std::queue<std::string> recv_queue;
    bool server_ended = false;
    std::string activity_value = "mock-activity-log";

    void push_recv(std::string s) { recv_queue.push(std::move(s)); }

    int Rec(std::string& str) override
    {
        if (recv_queue.empty())
        {
            str.clear();
            return 0;
        }
        str = recv_queue.front();
        recv_queue.pop();
        return static_cast<int>(str.size());
    }

    int Send(const std::string& str) const override
    {
        const_cast<MockThread*>(this)->sent.push_back(str);
        return static_cast<int>(str.size()) + 1;
    }

    int Send(const int& var) const override { return Send(std::to_string(var)); }

    int Send(const double& var) const override
    {
        std::ostringstream ss;
        ss << var;
        return Send(ss.str());
    }

    void endServer() override { server_ended = true; }

    std::string getActivity() override { return activity_value; }

    void closeClient() override {}
};

class TestCustomerHarness : public Customer
{
public:
    explicit TestCustomerHarness(MockThread& t) : Customer(t) {}
    void home_only() { home(); }
};

class TestEmployeeHarness : public Employee
{
public:
    explicit TestEmployeeHarness(MockThread& t) : Employee(t) {}
    void home_only() { home(); }
    void complain_only() { complain(); }
};

class TestAdminHarness : public Admin
{
public:
    explicit TestAdminHarness(MockThread& t) : Admin(t) {}
    void home_only() { home(); }
};

class PersonProbe : public Customer
{
public:
    explicit PersonProbe(MockThread& m) : Customer(m) {}

    void set_lookup(const std::string& v) { this->lookup = v; }

    void do_login(const std::string& f) { login(f); }

    void do_profile(const std::string& f, const std::string& l) { profile(f, l); }

    void do_buy(const std::string& f) { buy(f); }
};

class EmployeeBuyProbe : public Employee
{
public:
    explicit EmployeeBuyProbe(MockThread& m) : Employee(m) {}

    void set_lookup(const std::string& v) { this->lookup = v; }

    void do_buy(const std::string& f) { buy(f); }
};
