#include "Thread.h"

#include <gtest/gtest.h>

TEST(ThreadState, InitiallyAvailable)
{
    Thread t;
    EXPECT_TRUE(t.isFree());
    EXPECT_FALSE(t.isDestroyed());
    EXPECT_TRUE(t.available());
}

TEST(ThreadState, SetFreeMarksDestroyed)
{
    Thread t;
    t.setFree();
    EXPECT_TRUE(t.isFree());
    EXPECT_TRUE(t.isDestroyed());
    EXPECT_FALSE(t.available());
}
