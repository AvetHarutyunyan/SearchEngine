#pragma once
#include <string>
#include <vector>
#include "server.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class ConverterJSON {
    std::string _config_path;
    std::string _request_path;
    std::string _answers_path;

    std::string _name;
    std::string _version;
    int _max_responses;
public:
    ConverterJSON(const std::string& config_path = "./../../config/config.json",
                  const std::string& request_path = "./../../config/request.json",
                  const std::string& answers_path = "./../../config/answers.json"
                  );

    json ReadFile(const std::string& filePath);

    void CheckConfigFile(const json& configData);

    std::vector<std::string> getTextDocuments();

    int getResponsesLimit();

    std::vector<std::string> getRequests();

    void putAnswers(std::vector<std::vector<RelativeIndex>> answers);

    const std::string &getName() const;

    const std::string &getVersion() const;
};
