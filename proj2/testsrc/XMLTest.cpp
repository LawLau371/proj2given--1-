#include <gtest/gtest.h>
#include "XMLReader.h"
#include "XMLWriter.h"
#include "XMLEntity.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

#include "StringDataSource.cpp"
#include "StringDataSink.cpp"

TEST(XMLReaderTest, ReadSimpleElement) {
    CStringDataSource source("<root>hello</root>");
    CXMLReader reader(std::make_shared<CStringDataSource>(source));
    SXMLEntity entity;
    EXPECT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "root");

    EXPECT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData, "hello");
    EXPECT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
}

TEST(XMLReaderTest, SkipCData) {
    CStringDataSource source("<root>Hello<child>data</child>World</root>");
    CXMLReader reader(std::make_shared<CStringDataSource>(source));
    SXMLEntity entity;
    
    EXPECT_TRUE(reader.ReadEntity(entity, true)); // StartElement: root
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "root");
    
    EXPECT_TRUE(reader.ReadEntity(entity, true)); // StartElement: child
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "child");
    
    EXPECT_TRUE(reader.ReadEntity(entity, true)); // EndElement: child
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "child");
    
    EXPECT_TRUE(reader.ReadEntity(entity, true)); // EndElement: root
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "root");
    
    EXPECT_FALSE(reader.ReadEntity(entity, true)); // No more entities
}

TEST(XMLWriterTest, WriteSimpleElement) {
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);
    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::StartElement;
    entity.DNameData = "root";
    EXPECT_TRUE(writer.WriteEntity(entity));
    entity.DType = SXMLEntity::EType::CharData;
    entity.DNameData = "hello";
    EXPECT_TRUE(writer.WriteEntity(entity));
    entity.DType = SXMLEntity::EType::EndElement;
    entity.DNameData = "root";
    EXPECT_TRUE(writer.WriteEntity(entity));
    EXPECT_EQ(sink->String(), "<root>hello</root>");
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
