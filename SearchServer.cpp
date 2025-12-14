#include "SearchServer.hpp"

std::vector<std::string> SearchServer::GetSortedUniqueWords(const std::string& query) {
    std::set<std::string> uniqueWordsSet;
    std::stringstream ss(query);
    std::string word;
    while (ss >> word) {
        uniqueWordsSet.insert(word);
    }
    std::vector<std::string> uniqueWords(uniqueWordsSet.begin(), uniqueWordsSet.end());

    std::map<std::string, size_t> wordTotalFrequency;

    for (const auto& uniqueWord : uniqueWords) {
        std::vector<Entry> entries = _index.GetWordCount(uniqueWord);
        size_t totalCount = 0;
        for (const auto& entry : entries) {
            totalCount += entry.count;
        }
        wordTotalFrequency[uniqueWord] = totalCount;
    }

    std::sort(uniqueWords.begin(), uniqueWords.end(),
        [&wordTotalFrequency](const std::string& a, const std::string& b) {
            return wordTotalFrequency[a] < wordTotalFrequency[b];
    });

    return uniqueWords;
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input, size_t maxResponses){
    std::vector<std::vector<RelativeIndex>> finalResults;

    for (const auto& query : queries_input) {
        std::vector<RelativeIndex> currentQueryResults;

        std::vector<std::string> sortedWords = GetSortedUniqueWords(query);

        if (sortedWords.empty() || _index.GetWordCount(sortedWords.front()).empty()) {
            finalResults.push_back({});
            continue;
        }

        std::set<size_t> commonDocs;
        for (const auto& entry : _index.GetWordCount(sortedWords.front())) {
            commonDocs.insert(entry.doc_id);
        }

        for (size_t i = 1; i < sortedWords.size(); i++) {
            std::vector<Entry> currentWordEntries = _index.GetWordCount(sortedWords[i]);
            std::set<size_t> currentWordDocIds;
            for (const auto& entry : currentWordEntries) {
                currentWordDocIds.insert(entry.doc_id);
            }

            std::set<size_t> nextCommonDocs;
            for (size_t docId : commonDocs) {
                if (currentWordDocIds.count(docId)) {
                    nextCommonDocs.insert(docId);
                }
            }
            commonDocs = nextCommonDocs;
            if (commonDocs.empty()) break;
        }
    
        if (commonDocs.empty()) {
            finalResults.push_back({});
            continue;
        }

        std::map<size_t, float> absRelevance;
        float maxRabs = 0.0f;
        
        for (size_t docId : commonDocs) {
            float Rabs = 0.0f;
            for (const std::string& word : sortedWords) {
                std::vector<Entry> entries = _index.GetWordCount(word);
                for (const auto& entry : entries) {
                    if (entry.doc_id == docId) {
                        Rabs += entry.count;
                        break;
                    }
                }
            }
            absRelevance[docId] = Rabs;
            if (Rabs > maxRabs) {
                maxRabs = Rabs;
            }
        }

        for (const auto& pair : absRelevance) {
            RelativeIndex rel;
            rel.doc_id = pair.first;
            rel.rank = (maxRabs > 0) ? pair.second / maxRabs : 0.0f;
            currentQueryResults.push_back(rel);
        }

        std::sort(currentQueryResults.begin(), currentQueryResults.end(),
        [](const RelativeIndex& a, const RelativeIndex& b) {
            if (std::fabs(a.rank - b.rank) > 0.0001f) {
                return a.rank > b.rank;
            } 
            return a.doc_id < b.doc_id;
        });

        if (currentQueryResults.size() > maxResponses) {
            currentQueryResults.resize(maxResponses);
        }

        finalResults.push_back(currentQueryResults);
    }

    return finalResults;
}