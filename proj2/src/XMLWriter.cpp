#include "../include/XMLWriter.h"
#include "../include/XMLEntity.h"
#include "../include/DataSink.h"
#include <expat.h>
#include <memory>
#include <string>
#include <vector>
#include <sstream>

struct CXMLWriter::SImplementation {
    std::shared_ptr<CDataSink> Sink;
    std::ostringstream Buffer;
    
    SImplementation(std::shared_ptr<CDataSink> sink) : Sink(std::move(sink)) {}
};

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : DImplementation(std::make_unique<SImplementation>(std::move(sink))) {}

CXMLWriter::~CXMLWriter() {
    Flush();
}

bool CXMLWriter::Flush() {
    std::string output = DImplementation->Buffer.str();
    if (!output.empty()) {
        DImplementation->Sink->Write(std::vector<char>(output.begin(), output.end()));
        DImplementation->Buffer.str("");
        DImplementation->Buffer.clear();
    }
    return true;
}

bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    size_t prevSize = DImplementation->Buffer.str().size(); // Store previous size
    
    switch (entity.DType) {
        case SXMLEntity::EType::StartElement:
            DImplementation->Buffer << "<" << entity.DNameData;
            for (const auto &[key, value] : entity.DAttributes) {
                DImplementation->Buffer << " " << key << "=\"" << value << "\"";
            }
            DImplementation->Buffer << ">";
            break;
        case SXMLEntity::EType::EndElement:
            DImplementation->Buffer << "</" << entity.DNameData << ">";
            break;
        case SXMLEntity::EType::CharData:
            DImplementation->Buffer << entity.DNameData;
            break;
        case SXMLEntity::EType::CompleteElement:
            DImplementation->Buffer << "<" << entity.DNameData;
            for (const auto &[key, value] : entity.DAttributes) {
                DImplementation->Buffer << " " << key << "=\"" << value << "\"";
            }
            DImplementation->Buffer << "/>";
            break;
    }
    
    // Extract only the new part of the buffer
    std::string newOutput = DImplementation->Buffer.str().substr(prevSize);
    std::vector<char> chars(newOutput.begin(), newOutput.end());

    return DImplementation->Sink->Write(chars);
}
