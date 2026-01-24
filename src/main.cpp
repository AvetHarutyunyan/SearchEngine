#include <iostream>
#include "converter.h"
#include "server.h"
#include "inverted_index.h"
#include <fstream>

int main() {
    ConverterJSON converter;
    InvertedIndex index;
    index.updateDocumentBase(converter.getTextDocuments());
    SearchServer server(index);
    converter.putAnswers(server.search(converter.getRequests(), converter.getResponsesLimit()));
    std::cout << converter.getName() << "[v" << converter.getVersion() << "] finished the job\n";
}