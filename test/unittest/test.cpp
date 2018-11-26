#include "gtest/gtest.h"

TEST(TestCase, assert) {
    ASSERT_TRUE(true);
    ASSERT_FALSE(false);
    ASSERT_EQ(1,1); // ==
    ASSERT_NE(1,2); // !=
    ASSERT_LT(1,2); // <
    ASSERT_LE(1,2); // <=
    ASSERT_GT(2,1); // >
    ASSERT_GE(2,1); // >=

    ASSERT_STREQ("123","123");
    ASSERT_STRNE("123","12");
    ASSERT_STRCASEEQ("ABC", "abc"); //ignoring case
    ASSERT_STRCASENE("ABC", "ab");
}


TEST(TestCase, expect) {
    EXPECT_TRUE(true);
    EXPECT_FALSE(false);
    EXPECT_EQ(1,1); // ==
    EXPECT_NE(1,2); // !=
    EXPECT_LT(1,2); // <
    EXPECT_LE(1,2); // <=
    EXPECT_GT(2,1); // >
    EXPECT_GE(2,1); // >=

    EXPECT_STREQ("123","123");
    EXPECT_STRNE("123","12");
    EXPECT_STRCASEEQ("ABC", "abc"); //ignoring case
    EXPECT_STRCASENE("ABC", "ab");
}
