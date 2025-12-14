#pragma once
#include <vector>
#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct RelativeIndex {
    size_t doc_id;
    float rank;

    bool operator==(const RelativeIndex& other) const{
        return (doc_id == other.doc_id && std::fabs(rank - other.rank) < 0.0001f);
    }
};

class ConverterJSON {
private:
    json ReadFile(const std::string& filePath) const;
    void CheckConfigFile(const json& configData) const;
public:
    ConverterJSON() = default;
    std::vector<std::string> GetTextDocuments();
    int GetResponsesLimit();
    std::vector<std::string> GetRequests();
    void putAnswers(const std::vector<std::vector<RelativeIndex>>& answers);
};
