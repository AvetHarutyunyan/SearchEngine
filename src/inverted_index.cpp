#include "inverted_index.h"

void InvertedIndex::updateDocumentBase(const std::vector<std::string> &input_docs) {
    docs = input_docs;
    freq_dictionary.clear();

    if (docs.empty()) {
        return;
    }

    std::vector<std::future<std::map<std::string, size_t>>> futures;

    for (size_t docID = 0; docID < docs.size(); docID++)
    {
        futures.push_back(
            std::async(std::launch::async,
                        &InvertedIndex::indexSingleDocument, this,
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
            freq_dictionary[word].push_back(newEntry);
        }  
    }
}

std::vector<Entry> InvertedIndex::getWordCount(const std::string &word)
{
    auto it = freq_dictionary.find(word);
    if (it != freq_dictionary.end()) {
        return it->second;
    }
    return {};
}

std::map<std::string, size_t> InvertedIndex::indexSingleDocument(size_t docID, const std::string &text)
{
    std::map<std::string, size_t> wordFrequency;
    std::stringstream ss(text);
    std::string word;

    while (ss >> word)
        wordFrequency[word]++;

    return wordFrequency;
}
