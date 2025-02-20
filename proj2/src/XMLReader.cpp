#include "XMLReader.h"
#include <expat.h>
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <stdexcept>

struct CXMLReader::SImplementation {
    std::shared_ptr<CDataSource> Source;
    XML_Parser Parser;
    bool EndOfFile;
    std::queue<SXMLEntity> EntityQueue;
};

void CXMLReader::StartElementHandler(void *userData, const char *name, const char **atts) {
    auto *impl = static_cast<CXMLReader::SImplementation *>(userData);

    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::StartElement;
    entity.DNameData = name;

    for (size_t i = 0; atts[i] && atts[i + 1]; i += 2) {
        entity.DAttributes.emplace_back(atts[i], atts[i + 1]);
    }

    impl->EntityQueue.push(entity);
}

void CXMLReader::EndElementHandler(void *userData, const char *name) {
    auto *impl = static_cast<CXMLReader::SImplementation *>(userData);

    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::EndElement;
    entity.DNameData = name;

    impl->EntityQueue.push(entity);
}

void CXMLReader::CharDataHandler(void *userData, const char *data, int len) {
    auto *impl = static_cast<CXMLReader::SImplementation *>(userData);

    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::CharData;
    entity.DNameData.assign(data, len);

    impl->EntityQueue.push(entity);
}

CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src)
    : DImplementation(std::make_unique<SImplementation>()) {
    DImplementation->Source = std::move(src);
    DImplementation->Parser = XML_ParserCreate(nullptr);
    
    XML_SetUserData(DImplementation->Parser, DImplementation.get());
    XML_SetElementHandler(DImplementation->Parser, StartElementHandler, EndElementHandler);
    XML_SetCharacterDataHandler(DImplementation->Parser, CharDataHandler);
    
    DImplementation->EndOfFile = false;
}

CXMLReader::~CXMLReader() { 
    XML_ParserFree(DImplementation->Parser); 
}

bool CXMLReader::End() const { 
    return DImplementation->EndOfFile; 
}

// Helper function to process entities in the queue
bool CXMLReader::ProcessQueue(SXMLEntity &entity, bool skipcdata) {
    while (!DImplementation->EntityQueue.empty()) {
        entity = DImplementation->EntityQueue.front();
        DImplementation->EntityQueue.pop();

        // If skipping CDATA, continue to next entity
        if (skipcdata && entity.DType == SXMLEntity::EType::CharData) {
            continue;
        }

        return true;
    }
    return false;
}

// Helper function to process data from the source and fill the buffer
bool CXMLReader::ProcessSourceData(char *buffer, size_t &len) {
    len = 0;
    while (len < 4096 && !DImplementation->Source->End()) {
        char ch;
        if (DImplementation->Source->Get(ch)) {
            buffer[len++] = ch;
        } else {
            break;
        }
    }
    return len > 0;  // Ensure we read some data
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    char buffer[4096];
    size_t len = 0;

    // First, check the entity queue
    if (ProcessQueue(entity, skipcdata)) {
        return true;  // Entity found in the queue
    }

    // If the source has reached EOF, set the EOF flag and return false
    if (DImplementation->Source->End()) {
        DImplementation->EndOfFile = true;
        return false;
    }

    // Start reading and parsing data
    while (true) {
        // Read data into the buffer
        if (!ProcessSourceData(buffer, len)) {
            // No data read, mark EOF and return false
            DImplementation->EndOfFile = true;
            return false;
        }

        // Parse the data read into the buffer
        if (!XML_Parse(DImplementation->Parser, buffer, static_cast<int>(len), DImplementation->Source->End())) {
            // Parsing error, handle it
            const char *error = XML_ErrorString(XML_GetErrorCode(DImplementation->Parser));
            throw std::runtime_error("XML Parsing error: " + std::string(error));
        }

        // Try to get an entity from the queue after parsing
        if (ProcessQueue(entity, skipcdata)) {
            return true;  // Entity found after parsing
        }

        // If the source has reached EOF, set the EOF flag and return false
        if (DImplementation->Source->End()) {
            DImplementation->EndOfFile = true;
            return false;
        }
    }
}