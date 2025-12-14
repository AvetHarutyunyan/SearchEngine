#pragma once
#include <vector>
#include <iostream>
#include <map>

struct Entry {
    size_t doc_id;
    size_t count;

    bool operator==(const Entry& other) const{
        return (doc_id == other.doc_id && count == other.count);
    }
};


class InvertedIndex {
private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freqDictionary;
    std::map<std::string, size_t> IndexSingleDocument(size_t docID, const std::string& text);
public:
    InvertedIndex() = default;
    void UpdateDocumentBase(const std::vector<std::string>& inputDocs);
    std::vector<Entry> GetWordCount(const std::string& word) const;
};