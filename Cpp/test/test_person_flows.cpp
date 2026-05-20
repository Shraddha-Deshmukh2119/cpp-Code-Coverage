#include "Complaint_C.h"
#include "mock_thread.hpp"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>


namespace fs = std::filesystem;

namespace {

class DataFixture : public ::testing::Test
{
protected:
    fs::path dir;
    fs::path prev;

    void SetUp() override
    {
        prev = fs::current_path();
        dir = fs::temp_directory_path() /
              ("person_test_" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
        fs::create_directories(dir);
        fs::current_path(dir);
    }

    void TearDown() override
    {
        fs::current_path(prev);
        std::error_code ec;
        fs::remove_all(dir, ec);
    }

    static void write_goods_default(std::ostream& g)
    {
        for (int i = 0; i < GOODS_COUNT; ++i)
        {
            g << "Name: Item" << (i + 1) << "\n"
              << "Stock: 100\n"
              << "Cost: 1\n"
              << "Price: 10\n"
              << "Reorder Level: 2\n"
              << "Reorder Amount: 20\n\n";
        }
    }

    static void write_cash(std::ostream& o, double initial, double finl)
    {
        o << "Initial cash: " << initial << "\n"
          << "Cash in: 0\n"
          << "Cash out: 0\n"
          << "Final cash: " << finl << "\n";
    }

    static void write_user_record(std::ostream& o, const std::string& user, const std::string& pass,
                                  const std::string& balance)
    {
        o << "Username: " << user << "\n"
          << "Password: " << pass << "\n"
          << "Name: N\n"
          << "Age: 20\n"
          << "Sex: M\n"
          << "Date of birth: 1/1/2000\n"
          << "CNIC: 111\n"
          << "Email: e@e.com\n"
          << "Phone number: 1\n"
          << "Balance: " << balance << "\n\n";
    }
};

} // namespace

TEST_F(DataFixture, CustomerStartExit)
{
    MockThread m;
    TestCustomerHarness c(m);
    m.push_recv("3");
    c.start();
}

TEST_F(DataFixture, CustomerLoginThenExitHome)
{
    {
        std::ofstream f("customer.txt");
        write_user_record(f, "u1", "p1", "5000");
    }
    MockThread m;
    TestCustomerHarness c(m);
    m.push_recv("2");
    m.push_recv("u1");
    m.push_recv("p1");
    m.push_recv("4");
    m.push_recv("3");
    c.start();
    ASSERT_FALSE(m.sent.empty());
    EXPECT_NE(m.sent.front().find("Correct"), std::string::npos);
}

TEST_F(DataFixture, CustomerSignupSuccess)
{
    MockThread m;
    TestCustomerHarness c(m);
    m.push_recv("1");
    m.push_recv("n");
    m.push_recv("20");
    m.push_recv("M");
    m.push_recv("01");
    m.push_recv("02");
    m.push_recv("2000");
    m.push_recv("cnic");
    m.push_recv("mail");
    m.push_recv("phone");
    m.push_recv("newuser");
    m.push_recv("newpass");
    m.push_recv("3");
    c.start();
    EXPECT_TRUE(std::ifstream("customer.txt").good());
}

TEST_F(DataFixture, CustomerProfileFromHome)
{
    {
        std::ofstream f("customer.txt");
        write_user_record(f, "prof", "pp", "50");
    }
    MockThread m;
    TestCustomerHarness c(m);
    m.push_recv("2");
    m.push_recv("prof");
    m.push_recv("pp");
    m.push_recv("3");
    m.push_recv("4");
    m.push_recv("3");
    c.start();
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s.find("prof") != std::string::npos; }));
}

TEST_F(DataFixture, CustomerComplainMenuRoundTrip)
{
    {
        std::ofstream f("customer.txt");
        write_user_record(f, "bob", "pb", "0");
    }
    MockThread m;
    TestCustomerHarness c(m);
    m.push_recv("2");
    m.push_recv("bob");
    m.push_recv("pb");
    m.push_recv("4");
    m.push_recv("3");
    c.start();
}

TEST_F(DataFixture, CustomerComplainWriteAndReview)
{
    {
        std::ofstream f("customer.txt");
        write_user_record(f, "carol", "pc", "10");
    }
    MockThread m;
    TestCustomerHarness c(m);
    m.push_recv("2");
    m.push_recv("carol");
    m.push_recv("pc");
    m.push_recv("2");
    m.push_recv("1");
    m.push_recv("my text");
    m.push_recv("2");
    m.push_recv("4");
    m.push_recv("4");
    m.push_recv("3");
    c.start();
    bool sawTrue = false;
    for (const auto& s : m.sent)
    {
        if (s == "TRUE")
            sawTrue = true;
    }
    EXPECT_TRUE(sawTrue);
}

TEST_F(DataFixture, CustomerComplainUpdatePaths)
{
    {
        std::ofstream f("customer.txt");
        write_user_record(f, "dave", "pd", "10");
    }
    MockThread m;
    TestCustomerHarness c(m);
    m.push_recv("2");
    m.push_recv("dave");
    m.push_recv("pd");
    m.push_recv("2");
    m.push_recv("1");
    m.push_recv("first");
    m.push_recv("3");
    m.push_recv("second");
    m.push_recv("4");
    m.push_recv("4");
    m.push_recv("3");
    c.start();
}

TEST_F(DataFixture, LoginMissingFileDoesNotLoop)
{
    MockThread m;
    PersonProbe p(m);
    p.do_login("no_such_login_file.txt");
}

TEST_F(DataFixture, ProfileMissingFileEndsServer)
{
    MockThread m;
    PersonProbe p(m);
    p.set_lookup("Username: ghost");
    p.do_profile("missing_profile.txt", "Username: ghost");
    EXPECT_TRUE(m.server_ended);
}

TEST_F(DataFixture, ProfileSendsRecord)
{
    {
        std::ofstream f("customer.txt");
        write_user_record(f, "erin", "pe", "3");
    }
    MockThread m;
    PersonProbe p(m);
    p.set_lookup("Username: erin");
    p.do_profile("customer.txt", "Username: erin");
    bool ok = false;
    for (const auto& s : m.sent)
    {
        if (s.find("erin") != std::string::npos)
            ok = true;
    }
    EXPECT_TRUE(ok);
}

TEST_F(DataFixture, BuyExitsImmediately)
{
    {
        std::ofstream g("goods.txt");
        write_goods_default(g);
        std::ofstream cash("cash.txt");
        write_cash(cash, 1000, 1000);
        std::ofstream cu("customer.txt");
        write_user_record(cu, "buyer", "pb", "1000");
    }
    MockThread m;
    PersonProbe p(m);
    p.set_lookup("Username: buyer");
    m.push_recv("3");
    p.do_buy("customer.txt");
}

TEST_F(DataFixture, BuyNotEnoughStock)
{
    {
        std::ofstream g("goods.txt");
        g << "Name: Item1\n"
          << "Stock: 2\n"
          << "Cost: 1\n"
          << "Price: 10\n"
          << "Reorder Level: 0\n"
          << "Reorder Amount: 1\n\n";
        for (int i = 2; i <= GOODS_COUNT; ++i)
        {
            g << "Name: Item" << i << "\n"
              << "Stock: 100\n"
              << "Cost: 1\n"
              << "Price: 10\n"
              << "Reorder Level: 0\n"
              << "Reorder Amount: 1\n\n";
        }
        std::ofstream cash("cash.txt");
        write_cash(cash, 1000, 1000);
        std::ofstream cu("customer.txt");
        write_user_record(cu, "buyer", "pb", "1000");
    }
    MockThread m;
    PersonProbe p(m);
    p.set_lookup("Username: buyer");
    m.push_recv("1");
    m.push_recv("1");
    m.push_recv("5");
    m.push_recv("3");
    p.do_buy("customer.txt");
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s.find("NOT-ENOUGH") != std::string::npos; }));
}

TEST_F(DataFixture, BuyNoOrderCheckout)
{
    {
        std::ofstream g("goods.txt");
        write_goods_default(g);
        std::ofstream cash("cash.txt");
        write_cash(cash, 1000, 1000);
        std::ofstream cu("customer.txt");
        write_user_record(cu, "buyer", "pb", "1000");
    }
    MockThread m;
    PersonProbe p(m);
    p.set_lookup("Username: buyer");
    m.push_recv("2");
    m.push_recv("3");
    p.do_buy("customer.txt");
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s.find("NO-ORDER") != std::string::npos; }));
}

TEST_F(DataFixture, BuyInsufficientFunds)
{
    {
        std::ofstream g("goods.txt");
        write_goods_default(g);
        std::ofstream cash("cash.txt");
        write_cash(cash, 1000, 1000);
        std::ofstream cu("customer.txt");
        write_user_record(cu, "poor", "pp", "5");
    }
    MockThread m;
    PersonProbe p(m);
    p.set_lookup("Username: poor");
    m.push_recv("1");
    m.push_recv("1");
    m.push_recv("1");
    m.push_recv("2");
    m.push_recv("3");
    p.do_buy("customer.txt");
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s.find("Cost of the order") != std::string::npos; }));
}

TEST_F(DataFixture, BuySuccessfulCustomerPrice)
{
    {
        std::ofstream g("goods.txt");
        write_goods_default(g);
        std::ofstream cash("cash.txt");
        write_cash(cash, 10000, 10000);
        std::ofstream cu("customer.txt");
        write_user_record(cu, "rich", "pr", "1000");
    }
    MockThread m;
    PersonProbe p(m);
    p.set_lookup("Username: rich");
    m.push_recv("1");
    m.push_recv("1");
    m.push_recv("1");
    m.push_recv("2");
    m.push_recv("3");
    p.do_buy("customer.txt");
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s.find("new balance") != std::string::npos; }));
}

TEST_F(DataFixture, BuyTriggersAutomaticReorder)
{
    {
        std::ofstream g("goods.txt");
        g << "Name: Item1\n"
          << "Stock: 4\n"
          << "Cost: 1\n"
          << "Price: 2\n"
          << "Reorder Level: 5\n"
          << "Reorder Amount: 10\n\n";
        for (int i = 2; i <= GOODS_COUNT; ++i)
        {
            g << "Name: Item" << i << "\n"
              << "Stock: 100\n"
              << "Cost: 1\n"
              << "Price: 2\n"
              << "Reorder Level: 0\n"
              << "Reorder Amount: 1\n\n";
        }
        std::ofstream cash("cash.txt");
        write_cash(cash, 10000, 10000);
        std::ofstream cu("customer.txt");
        write_user_record(cu, "reorder", "pr", "1000");
    }
    MockThread m;
    PersonProbe p(m);
    p.set_lookup("Username: reorder");
    m.push_recv("1");
    m.push_recv("1");
    m.push_recv("3");
    m.push_recv("2");
    m.push_recv("3");
    p.do_buy("customer.txt");
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s.find("new balance") != std::string::npos; }));
}

TEST_F(DataFixture, BuyEmployeeDiscountBranch)
{
    {
        std::ofstream g("goods.txt");
        write_goods_default(g);
        std::ofstream cash("cash.txt");
        write_cash(cash, 10000, 10000);
        std::ofstream em("emp.txt");
        write_user_record(em, "empu", "ep", "1000");
    }
    MockThread m;
    EmployeeBuyProbe p(m);
    p.set_lookup("Username: empu");
    m.push_recv("1");
    m.push_recv("1");
    m.push_recv("1");
    m.push_recv("2");
    m.push_recv("3");
    p.do_buy(empFile);
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s.find("new balance") != std::string::npos; }));
}

TEST_F(DataFixture, BuyInitFailureWithoutGoodsFile)
{
    MockThread m;
    PersonProbe p(m);
    p.set_lookup("Username: x");
    p.do_buy("customer.txt");
}

TEST_F(DataFixture, CustomerComplainWriteDuplicateReturnsFalse)
{
    {
        std::ofstream f("customer.txt");
        write_user_record(f, "dup", "pd", "10");
    }
    Complaint_C cc("dup", "complaint.dat");
    ASSERT_TRUE(cc.write("first"));
    MockThread m;
    TestCustomerHarness c(m);
    m.push_recv("2");
    m.push_recv("dup");
    m.push_recv("pd");
    m.push_recv("2");
    m.push_recv("1");
    m.push_recv("second");
    m.push_recv("4");
    m.push_recv("4");
    m.push_recv("3");
    c.start();
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s == "FALSE"; }));
}

TEST_F(DataFixture, SignupTransferFailsWhenCustomerPathIsDirectory)
{
    fs::create_directory(fs::path("customer.txt"));
    MockThread m;
    TestCustomerHarness c(m);
    m.push_recv("1");
    for (int i = 0; i < 11; ++i)
        m.push_recv("x");
    m.push_recv("3");
    c.start();
    EXPECT_TRUE(std::any_of(m.sent.begin(), m.sent.end(),
                            [](const std::string& s) { return s == "false"; }));
}
