#include <gtest/gtest.h>
#include "../include/StringUtils.h"

// StringUtils Tests
TEST(StringUtilsTest, UpperCaseTest) {
    EXPECT_EQ(StringUtils::Upper("hello"), "HELLO");
    EXPECT_EQ(StringUtils::Upper("Hello"), "HELLO");
    EXPECT_EQ(StringUtils::Upper("HELLO"), "HELLO");
    EXPECT_EQ(StringUtils::Upper("hElLo"), "HELLO");
}

TEST(StringUtilsTest, LowerCaseTest) {
    EXPECT_EQ(StringUtils::Lower("HELLO"), "hello");
    EXPECT_EQ(StringUtils::Lower("Hello"), "hello");
    EXPECT_EQ(StringUtils::Lower("hello"), "hello");
    EXPECT_EQ(StringUtils::Lower("HeLLo"), "hello");
}

TEST(StringUtilsTest, CapitalizeTest) {
    EXPECT_EQ(StringUtils::Capitalize("hello"), "Hello");
    EXPECT_EQ(StringUtils::Capitalize("Hello"), "Hello");
    EXPECT_EQ(StringUtils::Capitalize("hELLO"), "Hello");
    EXPECT_EQ(StringUtils::Capitalize(""), "");
}

TEST(StringUtilsTest, StripTest) {
    EXPECT_EQ(StringUtils::Strip("  hello  "), "hello");
    EXPECT_EQ(StringUtils::Strip("hello"), "hello");
    EXPECT_EQ(StringUtils::Strip(" \thello\n"), "hello");
    EXPECT_EQ(StringUtils::Strip("   "), "");
}

TEST(StringUtilsTest, CenterTest) {
    EXPECT_EQ(StringUtils::Center("hello", 10), "  hello   ");
    EXPECT_EQ(StringUtils::Center("hello", 11, '-'), "---hello---");
    EXPECT_EQ(StringUtils::Center("hello", 5), "hello");
    EXPECT_EQ(StringUtils::Center("hello", 3), "hello");
}

TEST(StringUtilsTest, ReplaceTest) {
    EXPECT_EQ(StringUtils::Replace("hello world", "world", "there"), "hello there");
    EXPECT_EQ(StringUtils::Replace("abc abc abc", "abc", "xyz"), "xyz xyz xyz");
    EXPECT_EQ(StringUtils::Replace("hello", "x", "y"), "hello");
    EXPECT_EQ(StringUtils::Replace("", "a", "b"), "");
}

TEST(StringUtilsTest, JoinTest) {
    std::vector<std::string> words = {"hello", "world"};
    EXPECT_EQ(StringUtils::Join(" ", words), "hello world");
    EXPECT_EQ(StringUtils::Join("-", words), "hello-world");
    EXPECT_EQ(StringUtils::Join("", words), "helloworld");
    EXPECT_EQ(StringUtils::Join(",", {}), "");
}

TEST(StringUtilsTest, SplitTest) {
    EXPECT_EQ(StringUtils::Split("hello world", " "), (std::vector<std::string>{"hello", "world"}));
    EXPECT_EQ(StringUtils::Split("one,two,three", ","), (std::vector<std::string>{"one", "two", "three"}));
    EXPECT_EQ(StringUtils::Split("hello", "x"), (std::vector<std::string>{"hello"}));
    EXPECT_EQ(StringUtils::Split("", ","), (std::vector<std::string>{""}));
}

TEST(StringUtilsTest, SliceTest) {
    EXPECT_EQ(StringUtils::Slice("hello world", 0, 5), "hello");
    EXPECT_EQ(StringUtils::Slice("hello world", 6), "world");
    EXPECT_EQ(StringUtils::Slice("hello world", -5), "world");
    EXPECT_EQ(StringUtils::Slice("hello world", 0, 50), "hello world");
}

TEST(StringUtilsTest, LStripTest) {
    EXPECT_EQ(StringUtils::LStrip("  hello"), "hello");
    EXPECT_EQ(StringUtils::LStrip("hello"), "hello");
    EXPECT_EQ(StringUtils::LStrip("\t\n hello"), "hello");
    EXPECT_EQ(StringUtils::LStrip("   " ), "");
}

TEST(StringUtilsTest, RStripTest) {
    EXPECT_EQ(StringUtils::RStrip("hello  "), "hello");
    EXPECT_EQ(StringUtils::RStrip("hello"), "hello");
    EXPECT_EQ(StringUtils::RStrip("hello \t\n"), "hello");
    EXPECT_EQ(StringUtils::RStrip("   " ), "");
}

TEST(StringUtilsTest, LJust) {
    EXPECT_EQ(StringUtils::LJust("hello", 10, '-'), "hello-----");
    EXPECT_EQ(StringUtils::LJust("hello", 5, '-'), "hello");
    EXPECT_EQ(StringUtils::LJust("hello", 3, '-'), "hello");
}

TEST(StringUtilsTest, RJust) {
    EXPECT_EQ(StringUtils::RJust("hello", 10, '-'), "-----hello");
    EXPECT_EQ(StringUtils::RJust("hello", 5, '-'), "hello");
    EXPECT_EQ(StringUtils::RJust("hello", 3, '-'), "hello");
}

TEST(StringUtilsTest, ExpandTabs) {
    EXPECT_EQ(StringUtils::ExpandTabs("hello\tworld", 4), "hello   world");
    EXPECT_EQ(StringUtils::ExpandTabs("\thello", 4), "    hello");
    EXPECT_EQ(StringUtils::ExpandTabs("hello", 4), "hello");
}

TEST(StringUtilsTest, EditDistance) {
    EXPECT_EQ(StringUtils::EditDistance("kitten", "sitting", false), 3);
    EXPECT_EQ(StringUtils::EditDistance("flaw", "lawn", false), 2);
    EXPECT_EQ(StringUtils::EditDistance("hello", "hello", false), 0);
    EXPECT_EQ(StringUtils::EditDistance("HELLO", "hello", true), 0);
    EXPECT_EQ(StringUtils::EditDistance("HELLO", "hello", false), 5);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
