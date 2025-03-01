#include "DSVWriter.h"
#include <sstream>

struct CDSVWriter::SImplementation {
    std::shared_ptr<CDataSink> Sink;
    char Delimiter;
    bool QuoteAll;

    SImplementation(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall)
        : Sink(std::move(sink)), Delimiter(delimiter), QuoteAll(quoteall) {}
};

CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall)
    : DImplementation(std::make_unique<SImplementation>(std::move(sink), delimiter, quoteall)) {}

CDSVWriter::~CDSVWriter() = default;

bool CDSVWriter::WriteRow(const std::vector<std::string> &row) {
    std::ostringstream output;
    
    for (size_t i = 0; i < row.size(); ++i) {
        const std::string &field = row[i];
        bool needsQuotes = DImplementation->QuoteAll || 
                           field.find(DImplementation->Delimiter) != std::string::npos || 
                           field.find('"') != std::string::npos || 
                           field.find('\n') != std::string::npos;

        if (needsQuotes) {
            output << '"';
            for (char ch : field) {
                if (ch == '"') {
                    output << "\"\"";  // Escape quotes by doubling them
                } else {
                    output << ch;
                }
            }
            output << '"';
        } else {
            output << field;
        }

        if (i < row.size() - 1) {
            output << DImplementation->Delimiter;
        }
    }
    output << '\n';

    // Convert output to a vector<char> and write to the sink
    std::string outStr = output.str();
    return DImplementation->Sink->Write(std::vector<char>(outStr.begin(), outStr.end()));
}
