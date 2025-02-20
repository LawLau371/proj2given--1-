#include "../include/DSVReader.h"
#include <sstream>

struct CDSVReader::SImplementation {
    std::shared_ptr<CDataSource> Source;
    char Delimiter;

    SImplementation(std::shared_ptr<CDataSource> src, char delimiter)
        : Source(std::move(src)), Delimiter(delimiter) {}
};

CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter)
    : DImplementation(
          std::make_unique<SImplementation>(std::move(src), delimiter)) {}

CDSVReader::~CDSVReader() = default;

bool CDSVReader::End() const { return DImplementation->Source->End(); }

bool CDSVReader::ReadRow(std::vector<std::string> &row) {
    row.clear();
    std::string line;
    char ch;
    bool inQuotes = false;
    std::ostringstream currentField;

    while (!DImplementation->Source->End()) {
        if (!DImplementation->Source->Get(ch)) {
            break;
        }

        if (ch == '"') {
            inQuotes = !inQuotes;
        } else if (ch == DImplementation->Delimiter && !inQuotes) {
            row.push_back(currentField.str());
            currentField.str("");
            currentField.clear();
        } else if (ch == '\n' && !inQuotes) {
            row.push_back(currentField.str());
            return true;
        } else {
            currentField << ch;
        }
    }

    if (!currentField.str().empty()) {
        row.push_back(currentField.str());
    }

    return !row.empty();
}
