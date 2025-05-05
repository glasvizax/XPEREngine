#include <gtest/gtest.h>

TEST(ExampleTest, BasicAssertions)
{
	EXPECT_EQ(7 * 6, 42);
	EXPECT_STRNE("hello", "world");
}

TEST(ExampleTest, MoreTests)
{
	ASSERT_TRUE(1 < 2);
}