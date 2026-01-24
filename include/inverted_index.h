#pragma once
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <future>
#include <sstream>

struct Entry {
    size_t _doc_id, _count;
    Entry(size_t doc_id, size_t count) : _doc_id(doc_id), _count(count){}
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
        return (_doc_id == other._doc_id &&
                _count == other._count);
    }
};



class InvertedIndex {
public:
    InvertedIndex() = default;

    InvertedIndex(const InvertedIndex& other) : freq_dictionary(other.freq_dictionary){}

    void updateDocumentBase(const std::vector<std::string>& input_docs);

    std::vector<Entry> getWordCount(const std::string& word);

    std::map<std::string, size_t> indexSingleDocument(size_t docID, const std::string& text);

private:
    std::map<std::string, std::vector<Entry>> freq_dictionary;
    
    std::vector<std::string> docs;
    
};
