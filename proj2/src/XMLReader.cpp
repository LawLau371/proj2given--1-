#include "../include/XMLReader.h"
#include "../include/DataSource.h"
#include "../include/XMLEntity.h"
#include <expat.h>
#include <memory>
#include <string>
#include <vector>
#include <queue>

struct CXMLReader::SImplementation {
    std::shared_ptr<CDataSource> Source;
    XML_Parser Parser;
    SXMLEntity CurrentEntity;
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

CXMLReader::~CXMLReader() { XML_ParserFree(DImplementation->Parser); }

bool CXMLReader::End() const { return DImplementation->EndOfFile; }

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    char buffer[4096];
    size_t len;

    while (true) {
        // Check if we have entities in the queue first
        while (!DImplementation->EntityQueue.empty()) {
            if (skipcdata && DImplementation->EntityQueue.front().DType == SXMLEntity::EType::CharData) {
                DImplementation->EntityQueue.pop();
                continue;
            }
            entity = DImplementation->EntityQueue.front();
            DImplementation->EntityQueue.pop();
            return true;
        }
        
        if (DImplementation->Source->End()) {
            DImplementation->EndOfFile = true;
            return false;
        }
        
        len = 0;
        while (len < sizeof(buffer) && !DImplementation->Source->End()) {
            char ch;
            if (DImplementation->Source->Get(ch)) {
                buffer[len++] = ch;
            } else {
                break;
            }
        }
        
        if (len == 0) {
            DImplementation->EndOfFile = true;
            return false;
        }
        
        if (!XML_Parse(DImplementation->Parser, buffer, static_cast<int>(len), DImplementation->Source->End())) {
            return false;
        }
    }
}
