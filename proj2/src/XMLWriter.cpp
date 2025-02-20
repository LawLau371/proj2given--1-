#include "XMLWriter.h"`
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
    std::ostringstream entityBuffer;
    
    switch (entity.DType) {
        case SXMLEntity::EType::StartElement:
            entityBuffer << "<" << entity.DNameData;
            for (const auto &[key, value] : entity.DAttributes) {
                entityBuffer << " " << key << "=\"" << value << "\"";
            }
            entityBuffer << ">";
            break;
        case SXMLEntity::EType::EndElement:
            entityBuffer << "</" << entity.DNameData << ">";
            break;
        case SXMLEntity::EType::CharData:
            entityBuffer << entity.DNameData;
            break;
        case SXMLEntity::EType::CompleteElement:
            entityBuffer << "<" << entity.DNameData;
            for (const auto &[key, value] : entity.DAttributes) {
                entityBuffer << " " << key << "=\"" << value << "\"";
            }
            entityBuffer << "/>";
            break;
    }

    std::string newOutput = entityBuffer.str();
    std::vector<char> chars(newOutput.begin(), newOutput.end());

    // Write the new entity directly to the sink
    return DImplementation->Sink->Write(chars);
}