#include "ConverterJSON.hpp"
#include "InvertedIndex.hpp"
#include "SearchServer.hpp"
#include <iostream>

using json = nlohmann::json;

int main() {
    try {
        ConverterJSON converter;

        std::vector<std::string> docs = converter.GetTextDocuments();
        if (docs.empty()) {
            throw std::runtime_error("Document base is empty. Check config.json and file paths.");
        }

        InvertedIndex index;
        index.UpdateDocumentBase(docs);

        SearchServer srv(index);

        std::vector<std::string> requests = converter.GetRequests();
        if (requests.empty()) {
            throw std::runtime_error("No search requests found in requests.json.");
        }

        size_t maxResponses = converter.GetResponsesLimit();

        std::vector<std::vector<RelativeIndex>> answers = srv.search(requests, maxResponses);

        converter.putAnswers(answers);

        std::cout << "Search complete. Results written to answers.json." << std::endl;
    }
    catch(const std::runtime_error& e) {
        std::cerr << "EXCEPTION CAUGHT: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}