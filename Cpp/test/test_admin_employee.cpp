#include "mock_thread.hpp"
#include "Complaint_E.h"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>


namespace fs = std::filesystem;

namespace {

class AdminFixture : public ::testing::Test
{
protected:
    fs::path dir;
    fs::path prev;

    void SetUp() override
    {
        prev = fs::current_path();
        dir = fs::temp_directory_path() /
              ("admin_test_" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
        fs::create_directories(dir);
        fs::current_path(dir);
    }

    void TearDown() override
    {
        fs::current_path(prev);
        std::error_code ec;
        fs::remove_all(dir, ec);
    }

    static void write_goods(std::ostream& g)
    {
        for (int i = 0; i < GOODS_COUNT; ++i)
        {
            g << "Name: Item" << (i + 1) << "\n"
              << "Stock: 10\n"
              << "Cost: 2\n"
              << "Price: 5\n"
              << "Reorder Level: 1\n"
              << "Reorder Amount: 5\n\n";
        }
    }

    static void write_cash(std::ostream& o, double finl)
    {
        o << "Initial cash: " << finl << "\n"
          << "Cash in: 0\n"
          << "Cash out: 0\n"
          << "Final cash: " << finl << "\n";
    }

    static void write_user(std::ostream& o, const std::string& u, const std::string& p, const std::string& cnic)
    {
        o << "Username: " << u << "\n"
          << "Password: " << p << "\n"
          << "Name: N\n"
          << "Age: 20\n"
          << "Sex: M\n"
          << "Date of birth: 1/1/2000\n"
          << "CNIC: " << cnic << "\n"
          << "Email: e@e.com\n"
          << "Phone number: 1\n"
          << "Balance: 100\n\n";
    }
};

} // namespace

TEST_F(AdminFixture, StockRejectsUnknownProduct)
{
    {
        std::ofstream g("goods.txt");
        write_goods(g);
        std::ofstream c("cash.txt");
        write_cash(c, 1000);
    }
    MockThread m;
    TestAdminHarness a(m);
    m.push_recv("1");
    m.push_recv("UnknownProduct");
    a.stock();
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s == "FALSE"; }));
}

TEST_F(AdminFixture, StockRestockHappyPath)
{
    {
        std::ofstream g("goods.txt");
        write_goods(g);
        std::ofstream c("cash.txt");
        write_cash(c, 1000);
    }
    MockThread m;
    TestAdminHarness a(m);
    m.push_recv("1");
    m.push_recv("Item1");
    m.push_recv("1");
    a.stock();
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s == "TRUE"; }));
}

TEST_F(AdminFixture, StockUpdatePriceBranch)
{
    {
        std::ofstream g("goods.txt");
        write_goods(g);
        std::ofstream c("cash.txt");
        write_cash(c, 1000);
    }
    MockThread m;
    TestAdminHarness a(m);
    m.push_recv("2");
    m.push_recv("Item2");
    m.push_recv("9.99");
    m.push_recv("3");
    m.push_recv("4");
    a.stock();
    EXPECT_FALSE(std::ifstream("goods.txt").fail());
}

TEST_F(AdminFixture, StockDefaultBranchNoOp)
{
    {
        std::ofstream g("goods.txt");
        write_goods(g);
        std::ofstream c("cash.txt");
        write_cash(c, 1000);
    }
    MockThread m;
    TestAdminHarness a(m);
    m.push_recv("0");
    a.stock();
}

TEST_F(AdminFixture, StockRestockRejectedWhenCashTooLow)
{
    {
        std::ofstream g("goods.txt");
        write_goods(g);
        std::ofstream c("cash.txt");
        write_cash(c, 1);
    }
    MockThread m;
    TestAdminHarness a(m);
    m.push_recv("1");
    m.push_recv("Item1");
    m.push_recv("999");
    a.stock();
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s == "FALSE"; }));
}

TEST_F(AdminFixture, AccountsThroughHome)
{
    {
        std::ofstream c("cash.txt");
        write_cash(c, 500);
        std::ofstream a("admin.txt");
        write_user(a, "root", "rp", "111");
    }
    MockThread m;
    Admin adm(m);
    m.push_recv("root");
    m.push_recv("rp");
    m.push_recv("8");
    m.push_recv("17");
    adm.login(adminFile);
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s.find("Final cash") != std::string::npos; }));
}

TEST_F(AdminFixture, ViewCustomerFile)
{
    {
        std::ofstream cu("customer.txt");
        write_user(cu, "c1", "p1", "222");
        std::ofstream a("admin.txt");
        write_user(a, "root", "rp", "111");
    }
    MockThread m;
    Admin adm(m);
    m.push_recv("root");
    m.push_recv("rp");
    m.push_recv("5");
    m.push_recv("17");
    adm.login(adminFile);
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s.find("c1") != std::string::npos; }));
}

TEST_F(AdminFixture, AddEmployeeFlow)
{
    {
        std::ofstream a("admin.txt");
        write_user(a, "root", "rp", "111");
    }
    MockThread m;
    Admin adm(m);
    m.push_recv("root");
    m.push_recv("rp");
    m.push_recv("4");
    m.push_recv("Ename");
    m.push_recv("30");
    m.push_recv("F");
    m.push_recv("06");
    m.push_recv("07");
    m.push_recv("1991");
    m.push_recv("cnicE");
    m.push_recv("eE@e.com");
    m.push_recv("555");
    m.push_recv("empnew");
    m.push_recv("emppass");
    m.push_recv("17");
    adm.login(adminFile);
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s == "TRUE" || s == "FALSE"; }));
}

TEST_F(AdminFixture, SearchCustomerNotFound)
{
    {
        std::ofstream cu("customer.txt");
        write_user(cu, "c1", "p1", "222");
        std::ofstream a("admin.txt");
        write_user(a, "root", "rp", "111");
    }
    MockThread m;
    Admin adm(m);
    m.push_recv("root");
    m.push_recv("rp");
    m.push_recv("10");
    m.push_recv("4");
    m.push_recv("nobody");
    m.push_recv("6");
    m.push_recv("17");
    adm.login(adminFile);
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s == "FALSE"; }));
}

TEST_F(AdminFixture, SearchEmployeeByCnicFound)
{
    {
        std::ofstream em("emp.txt");
        write_user(em, "e1", "p1", "777");
        std::ofstream a("admin.txt");
        write_user(a, "root", "rp", "111");
    }
    MockThread m;
    Admin adm(m);
    m.push_recv("root");
    m.push_recv("rp");
    m.push_recv("9");
    m.push_recv("1");
    m.push_recv("777");
    m.push_recv("6");
    m.push_recv("17");
    adm.login(adminFile);
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s == "TRUE"; }));
}

TEST_F(AdminFixture, DeleteCustomerRecord)
{
    {
        std::ofstream cu("customer.txt");
        write_user(cu, "gone", "pg", "222");
        std::ofstream a("admin.txt");
        write_user(a, "root", "rp", "111");
    }
    MockThread m;
    Admin adm(m);
    m.push_recv("root");
    m.push_recv("rp");
    m.push_recv("12");
    m.push_recv("gone");
    m.push_recv("17");
    adm.login(adminFile);
    std::ifstream check("customer.txt");
    std::string content((std::istreambuf_iterator<char>(check)), std::istreambuf_iterator<char>());
    EXPECT_EQ(content.find("gone"), std::string::npos);
}

TEST_F(AdminFixture, BalanceUpdateTrue)
{
    {
        std::ofstream cu("customer.txt");
        write_user(cu, "pay", "pp", "222");
        std::ofstream a("admin.txt");
        write_user(a, "root", "rp", "111");
    }
    MockThread m;
    Admin adm(m);
    m.push_recv("root");
    m.push_recv("rp");
    m.push_recv("13");
    m.push_recv("pay");
    m.push_recv("999");
    m.push_recv("17");
    adm.login(adminFile);
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s == "TRUE"; }));
}

TEST_F(AdminFixture, BalanceUpdateFalseWhenUserMissing)
{
    {
        std::ofstream cu("customer.txt");
        write_user(cu, "pay", "pp", "222");
        std::ofstream a("admin.txt");
        write_user(a, "root", "rp", "111");
    }
    MockThread m;
    Admin adm(m);
    m.push_recv("root");
    m.push_recv("rp");
    m.push_recv("13");
    m.push_recv("missing_user");
    m.push_recv("1");
    m.push_recv("17");
    adm.login(adminFile);
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s == "FALSE"; }));
}

TEST_F(AdminFixture, GetActivityCase)
{
    {
        std::ofstream a("admin.txt");
        write_user(a, "root", "rp", "111");
    }
    MockThread m;
    Admin adm(m);
    m.push_recv("root");
    m.push_recv("rp");
    m.push_recv("15");
    m.push_recv("17");
    adm.login(adminFile);
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s.find("mock-activity") != std::string::npos; }));
}

TEST_F(AdminFixture, EndServerCase)
{
    {
        std::ofstream a("admin.txt");
        write_user(a, "root", "rp", "111");
    }
    MockThread m;
    Admin adm(m);
    m.push_recv("root");
    m.push_recv("rp");
    m.push_recv("16");
    adm.login(adminFile);
    EXPECT_TRUE(m.server_ended);
}

TEST_F(AdminFixture, ComplainShortcutExitFromAdminHome)
{
    {
        std::ofstream a("admin.txt");
        write_user(a, "root", "rp", "111");
    }
    MockThread m;
    Admin adm(m);
    m.push_recv("root");
    m.push_recv("rp");
    m.push_recv("2");
    m.push_recv("7");
    m.push_recv("17");
    adm.login(adminFile);
}

TEST_F(AdminFixture, EmployeeHomeDefaultThenExit)
{
    {
        std::ofstream em("emp.txt");
        write_user(em, "ex", "px", "333");
    }
    MockThread m;
    TestEmployeeHarness e(m);
    m.push_recv("999");
    m.push_recv("4");
    e.home_only();
}

TEST_F(AdminFixture, EmployeeLoginBuyExit)
{
    {
        std::ofstream g("goods.txt");
        for (int i = 0; i < GOODS_COUNT; ++i)
        {
            g << "Name: Item" << (i + 1) << "\n"
              << "Stock: 10\n"
              << "Cost: 1\n"
              << "Price: 2\n"
              << "Reorder Level: 0\n"
              << "Reorder Amount: 1\n\n";
        }
        std::ofstream cash("cash.txt");
        write_cash(cash, 1000);
        std::ofstream em("emp.txt");
        write_user(em, "eb", "pb", "444");
    }
    MockThread m;
    Employee emp(m);
    m.push_recv("eb");
    m.push_recv("pb");
    m.push_recv("1");
    m.push_recv("3");
    m.push_recv("4");
    emp.login(empFile);
}

TEST_F(AdminFixture, EmployeeComplainSeeFalse)
{
    {
        std::ofstream em("emp.txt");
        write_user(em, "ex", "px", "333");
    }
    MockThread m;
    TestEmployeeHarness e(m);
    m.push_recv("1");
    m.push_recv("nobody");
    m.push_recv("7");
    e.complain_only();
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s == "FALSE"; }));
}

TEST_F(AdminFixture, EmployeeComplainSeeTrueAndAnswer)
{
    {
        std::ofstream em("emp.txt");
        write_user(em, "ex", "px", "333");
        Complaint_E ce("complaint.dat");
        ASSERT_TRUE(ce.write("issue text", "clientZ"));
    }
    MockThread m;
    TestEmployeeHarness e(m);
    m.push_recv("1");
    m.push_recv("clientZ");
    m.push_recv("3");
    m.push_recv("clientZ");
    m.push_recv("fixed");
    m.push_recv("4");
    m.push_recv("5");
    m.push_recv("6");
    m.push_recv("7");
    e.complain_only();
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s == "TRUE"; }));
}

TEST_F(AdminFixture, EmployeeComplainCaseTwoPending)
{
    {
        std::ofstream em("emp.txt");
        write_user(em, "ex", "px", "333");
        Complaint_E ce("complaint.dat");
        ASSERT_TRUE(ce.write("pending", "clientQ"));
    }
    MockThread m;
    TestEmployeeHarness e(m);
    m.push_recv("2");
    m.push_recv("clientQ");
    m.push_recv("7");
    e.complain_only();
}

TEST_F(AdminFixture, EmployeeComplainInvalidChoice)
{
    {
        std::ofstream em("emp.txt");
        write_user(em, "ex", "px", "333");
    }
    MockThread m;
    TestEmployeeHarness e(m);
    m.push_recv("9999");
    m.push_recv("7");
    e.complain_only();
}

TEST_F(AdminFixture, AdminBuyAndProfileThroughHome)
{
    {
        std::ofstream g("goods.txt");
        write_goods(g);
        std::ofstream cash("cash.txt");
        write_cash(cash, 5000);
        std::ofstream a("admin.txt");
        write_user(a, "root", "rp", "111");
    }
    MockThread m;
    Admin adm(m);
    m.push_recv("root");
    m.push_recv("rp");
    m.push_recv("1");
    m.push_recv("3");
    m.push_recv("3");
    m.push_recv("17");
    adm.login(adminFile);
}

TEST_F(AdminFixture, ViewEmployeeFileThroughHome)
{
    {
        std::ofstream em("emp.txt");
        write_user(em, "e1", "p1", "888");
        std::ofstream a("admin.txt");
        write_user(a, "root", "rp", "111");
    }
    MockThread m;
    Admin adm(m);
    m.push_recv("root");
    m.push_recv("rp");
    m.push_recv("6");
    m.push_recv("17");
    adm.login(adminFile);
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s.find("e1") != std::string::npos; }));
}

TEST_F(AdminFixture, SearchCustomerByEmail)
{
    {
        std::ofstream cu("customer.txt");
        write_user(cu, "mailu", "pm", "222");
        std::ofstream a("admin.txt");
        write_user(a, "root", "rp", "111");
    }
    MockThread m;
    Admin adm(m);
    m.push_recv("root");
    m.push_recv("rp");
    m.push_recv("10");
    m.push_recv("2");
    m.push_recv("e@e.com");
    m.push_recv("6");
    m.push_recv("17");
    adm.login(adminFile);
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s == "TRUE"; }));
}

TEST_F(AdminFixture, DeleteEmployeeRecord)
{
    {
        std::ofstream em("emp.txt");
        write_user(em, "delme", "pe", "999");
        std::ofstream a("admin.txt");
        write_user(a, "root", "rp", "111");
    }
    MockThread m;
    Admin adm(m);
    m.push_recv("root");
    m.push_recv("rp");
    m.push_recv("11");
    m.push_recv("delme");
    m.push_recv("17");
    adm.login(adminFile);
    std::ifstream check("emp.txt");
    std::string content((std::istreambuf_iterator<char>(check)), std::istreambuf_iterator<char>());
    EXPECT_EQ(content.find("delme"), std::string::npos);
}

TEST_F(AdminFixture, BalanceEmployeeTrue)
{
    {
        std::ofstream em("emp.txt");
        write_user(em, "paye", "pe", "555");
        std::ofstream a("admin.txt");
        write_user(a, "root", "rp", "111");
    }
    MockThread m;
    Admin adm(m);
    m.push_recv("root");
    m.push_recv("rp");
    m.push_recv("14");
    m.push_recv("paye");
    m.push_recv("250");
    m.push_recv("17");
    adm.login(adminFile);
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s == "TRUE"; }));
}

TEST_F(AdminFixture, StockInitFailure)
{
    MockThread m;
    TestAdminHarness a(m);
    a.stock();
}

TEST_F(AdminFixture, ViewCustomerFileOpenFails)
{
    fs::create_directory(fs::path("customer.txt"));
    {
        std::ofstream a("admin.txt");
        write_user(a, "root", "rp", "111");
    }
    MockThread m;
    Admin adm(m);
    m.push_recv("root");
    m.push_recv("rp");
    m.push_recv("5");
    m.push_recv("17");
    adm.login(adminFile);
    fs::remove_all("customer.txt");
}
