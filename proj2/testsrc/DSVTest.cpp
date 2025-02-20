#include <gtest/gtest.h>
#include "../include/DSVReader.h"
#include "../include/DSVWriter.h"
#include "../include/StringDataSource.h"
#include "../include/StringDataSink.h"

#include "../src/StringDataSource.cpp"
#include "../src/StringDataSink.cpp"

TEST(DSVReaderTest, ReadEmptyRow) {
    CStringDataSource source("");
    CDSVReader reader(std::make_shared<CStringDataSource>(source), ',');
    std::vector<std::string> row;
    EXPECT_FALSE(reader.ReadRow(row));
}

TEST(DSVReaderTest, ReadSingleRow) {
    CStringDataSource source("hello,world\n");
    CDSVReader reader(std::make_shared<CStringDataSource>(source), ',');
    std::vector<std::string> row;
    EXPECT_TRUE(reader.ReadRow(row));
    ASSERT_EQ(row.size(), 2);
    EXPECT_EQ(row[0], "hello");
    EXPECT_EQ(row[1], "world");
}

TEST(DSVWriterTest, WriteRow) {
    auto sink_ptr = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink_ptr, ',');
    std::vector<std::string> row = {"hello", "world"};
    EXPECT_TRUE(writer.WriteRow(row));
    EXPECT_EQ(sink_ptr->String(), "hello,world\n");
}

TEST(DSVWriterTest, WriteQuotedRow) {
    auto sink_ptr = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink_ptr, ',', true);
    std::vector<std::string> row = {"hello,world", "test"};
    EXPECT_TRUE(writer.WriteRow(row));
    EXPECT_EQ(sink_ptr->String(), "\"hello,world\",\"test\"\n");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
