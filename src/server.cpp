#include "server.h"


std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input, size_t max_responses)
{
    std::vector<std::vector<RelativeIndex>> result;

    for (const auto& query : queries_input) {
        std::vector<RelativeIndex> query_results;
        handleRequest(query, query_results, max_responses);
        result.push_back(query_results);
    }

    return result;
}

std::map<std::string, size_t> SearchServer::get_indexes_for_request_words(std::vector<std::string> &vec)
{
    std::map<std::string, size_t> word_freq_map;
    for (const auto& word : vec) {
        size_t total_count = 0;
        auto entries = _index->getWordCount(word);
        for (const auto& entry : entries) {
            total_count += entry._count;
        }
        word_freq_map[word] = total_count;
    }
    return word_freq_map;
}

void SearchServer::handleRequest(const std::string &request, std::vector<RelativeIndex> &responses, size_t max_responses)
{
    std::stringstream ss(request);
    std::string word;
    std::set<std::string> unique_words;
    
    while (ss >> word) {
        unique_words.insert(word);
    }

    std::map<size_t, size_t> doc_relevance;
    size_t max_absolute_relevance = 0;

    for (const auto& w : unique_words) {
        auto word_entries = _index->getWordCount(w);
        for (const auto& entry : word_entries) {
            doc_relevance[entry._doc_id] += entry._count;
            if (doc_relevance[entry._doc_id] > max_absolute_relevance) {
                max_absolute_relevance = doc_relevance[entry._doc_id];
            }
        }
    }

    if (doc_relevance.empty()) return;

    for (auto const& [doc_id, absolute_rank] : doc_relevance) {
        float rank = static_cast<float>(absolute_rank) / max_absolute_relevance;
        responses.emplace_back(doc_id, rank);
    }

    std::sort(responses.begin(), responses.end(), [](const RelativeIndex& a, const RelativeIndex& b) {
        if (a._rank != b._rank) return a._rank > b._rank;
        return a._doc_id < b._doc_id;
    });

    if (responses.size() > max_responses) {
        responses.erase(responses.begin() + max_responses, responses.end());
    }
}
