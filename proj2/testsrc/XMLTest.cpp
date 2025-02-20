#include <gtest/gtest.h>
#include "XMLReader.h" 
#include "XMLWriter.h" 
#include "XMLEntity.h" 
#include "StringDataSource.h"
#include "StringDataSink.h"

TEST(XMLReaderTest, ReadSimpleElement) {
    auto source = std::make_shared<CStringDataSource>("<root>hello</root>");
    CXMLReader reader(source);
    SXMLEntity entity;

    EXPECT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "root");

    EXPECT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData, "hello");

    EXPECT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "root");
}

TEST(XMLReaderTest, SkipCData) {
    auto source = std::make_
