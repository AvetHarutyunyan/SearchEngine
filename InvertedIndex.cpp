#include "InvertedIndex.hpp"
#include <sstream>
#include <future>

std::map<std::string, size_t> InvertedIndex::IndexSingleDocument(size_t doc_id, const std::string& text) {
    std::map<std::string, size_t> wordFrequency;
    std::stringstream ss(text);
    std::string word;

    while (ss >> word)
        wordFrequency[word]++;

    return wordFrequency;
}

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& inputDocs) {
    docs = inputDocs;
    freqDictionary.clear();

    if (docs.empty()) {
        return;
    }

    std::vector<std::future<std::map<std::string, size_t>>> futures;

    for (size_t docID = 0; docID < docs.size(); docID++)
    {
        futures.push_back(
            std::async(std::launch::async,
                        &InvertedIndex::IndexSingleDocument, this,
                        docID, docs[docID]
            )
        );
    }
    
    for (size_t docID = 0; docID < futures.size(); docID++) {
        std::map<std::string, size_t> docWordFreq = futures[docID].get();

        for (const auto& pair : docWordFreq) {
            const std::string& word = pair.first;
            size_t count = pair.second;

            Entry newEntry = {docID, count};
            freqDictionary[word].push_back(newEntry);
        }  
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) const {
    auto it = freqDictionary.find(word);
   
    if (it != freqDictionary.end()) {
        return it->second;
    } else {
        return std::vector<Entry>();
    }
}