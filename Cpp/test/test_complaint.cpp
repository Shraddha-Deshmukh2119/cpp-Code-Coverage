#include <chrono>
#include <filesystem>
#include <fstream>

#include "Complaint_Base.h"
#include "Complaint_C.h"
#include "Complaint_E.h"

#include <gtest/gtest.h>

namespace fs = std::filesystem;

namespace {

class ComplaintFixture : public ::testing::Test
{
protected:
    fs::path dir;
    fs::path prev;

    void SetUp() override
    {
        prev = fs::current_path();
        dir = fs::temp_directory_path() /
              ("complaint_test_" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
        fs::create_directories(dir);
        fs::current_path(dir);
    }

    void TearDown() override
    {
        fs::current_path(prev);
        std::error_code ec;
        fs::remove_all(dir, ec);
    }
};

} // namespace

TEST_F(ComplaintFixture, BaseWriteUniqueAndDuplicate)
{
    const std::string file = "c.dat";
    Complaint_Base base(file);
    const char client[] = "alice";
    const char text1[] = "first complaint";
    EXPECT_TRUE(base.write(text1, client));
    EXPECT_FALSE(base.write(text1, client));

    std::ostringstream os;
    Complaint c{};
    strcpy(c.client, client);
    strcpy(c.text, "x");
    base.view(c, os);
    EXPECT_NE(os.str().find("alice"), std::string::npos);
}

TEST_F(ComplaintFixture, BaseSeeSeenStatusAndUpdate)
{
    const std::string file = "c2.dat";
    Complaint_Base base(file);
    const char client[] = "bob";
    ASSERT_TRUE(base.write("msg", client));

    Complaint read{};
    ASSERT_TRUE(base.see(read, client, true));
    EXPECT_TRUE(read.seen);

    bool status = false;
    ASSERT_TRUE(base.update("updated text", client, status));
    EXPECT_FALSE(status);
}

TEST_F(ComplaintFixture, BaseUpdateBlockedWhenAnswered)
{
    const std::string file = "c3.dat";
    Complaint_E emp(file);
    const char client[] = "carl";
    ASSERT_TRUE(emp.write("body", client));
    ASSERT_TRUE(emp.answer("official reply", client));

    bool status = false;
    EXPECT_FALSE(emp.Complaint_Base::update("new body", client, status));
    EXPECT_TRUE(status);
}

TEST_F(ComplaintFixture, ComplaintCWriteReviewUpdate)
{
    const std::string file = "cust.dat";
    Complaint_C cc("dave", file);
    ASSERT_TRUE(cc.write("hello"));
    std::ostringstream os;
    cc.reView(os);
    EXPECT_NE(os.str().find("dave"), std::string::npos);

    bool st = false;
    ASSERT_TRUE(cc.update("rev2", st));
    EXPECT_FALSE(st);
}

TEST_F(ComplaintFixture, ComplaintEViewAnswerClear)
{
    const std::string file = "emp.dat";
    Complaint_E emp(file);
    ASSERT_TRUE(emp.write("issue", "eve"));

    Complaint read{};
    ASSERT_TRUE(emp.see(read, "eve", false));

    std::ostringstream shortList;
    emp.view(false, shortList);
    EXPECT_NE(shortList.str().find("eve"), std::string::npos);

    std::ostringstream detail;
    emp.view(true, detail);
    EXPECT_NE(detail.str().find("issue"), std::string::npos);

    ASSERT_TRUE(emp.answer("resolved", "eve"));

    emp.clear();
    std::ifstream verify(file, std::ios::binary);
    verify.seekg(0, std::ios::end);
    EXPECT_EQ(verify.tellg(), 0);
}

TEST_F(ComplaintFixture, ComplaintCReviewWhenMissing)
{
    const std::string file = "none.dat";
    Complaint_C cc("ghost", file);
    std::ostringstream os;
    cc.reView(os); // hits cerr path
    (void)os;
}
