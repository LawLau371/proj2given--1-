#include "DSVReader.h"
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

          bool CDSVReader::ReadRow(std::vector<std::string> &row) {
            row.clear();
            char ch;
            bool inQuotes = false;
            std::ostringstream currentField;
        
            while (!DImplementation->Source->End()) {
                if (!DImplementation->Source->Get(ch)) {
                    break;
                }
        
                if (ch == '"') {
                    if (inQuotes) {
                        // Check for double quote escaping
                        if (!DImplementation->Source->End()) {
                            char nextChar;
                            if (DImplementation->Source->Peek(nextChar) && nextChar == '"') {
                                DImplementation->Source->Get(nextChar);  // Consume the second quote
                                currentField << '"';  // Append literal quote
                                continue;
                            }
                        }
                    }
                    inQuotes = !inQuotes;  // Toggle inQuotes state
                } else if (ch == DImplementation->Delimiter && !inQuotes) {
                    // End of field
                    row.push_back(currentField.str());
                    currentField.str("");
                    currentField.clear();
                } else if (ch == '\n' && !inQuotes) {
                    // End of row
                    row.push_back(currentField.str());
                    return true;
                } else {
                    // Regular character in field
                    currentField << ch;
                }
            }
        
            // Add the last field if there is any content
            if (!currentField.str().empty()) {
                row.push_back(currentField.str());
            }
        
            return !row.empty();
        }
        
