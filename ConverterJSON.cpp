#include "ConverterJSON.hpp"
#include <fstream>
#include <sstream>
#include <exception>
#include <map>
#include <iostream>

json ConverterJSON::ReadFile(const std::string& filePath) const {
    std::ifstream file(filePath);
    if(!file.is_open()) {
        if (filePath == "config.json") {
            throw std::runtime_error("config file is missing");
        }
        return json::object();
    }

    try {
        json j;
        file >> j;
        return j;
    } catch(const json::parse_error& e) {
        if (filePath == "config.json") {
            throw std::runtime_error("config file is empty");
        }
        std::cerr << "Error parsing JSON file " << filePath << ": " << e.what() << '\n';
        return json::object();
    }    
}

void ConverterJSON::CheckConfigFile(const json& configData) const {
    if (!configData.contains("config")) {
        throw std::runtime_error("config file is empty (missing 'config' field)");
    }

    const auto& config = configData["config"];

    std::string appVersion = "0.1";
    if (config.contains("version") && config["version"].get<std::string>() != appVersion) {
        throw std::runtime_error("config.json has incorrect file version");
    }

    if (config.contains("name")) {
        std::cout << "Starting " << config["name"].get<std::string>() << std::endl;
    } else {
        std::cout << "Starting SearchEngine (name not specified in config)" << std::endl;
    }
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> documents;
    json configData = ReadFile("config.json");

    CheckConfigFile(configData);

    if (!configData.contains("files") || !configData["files"].is_array()) {
        std::cerr << "Warning: 'files' field missing or incorrect in config.json" << std::endl;
        return documents;
    }

    for (const auto& filePath : configData["files"]) {
        std::string path = filePath.get<std::string>();
        std::ifstream file(path);

        if (!file.is_open()) {
            std::cerr << "Error: file not found at path: " << path << std::endl;
            continue;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        documents.push_back(buffer.str());
    }

    return documents;
}

int ConverterJSON::GetResponsesLimit() {
    json configData = ReadFile("config.json");

    if (configData.contains("config") && configData["config"].contains("max_responses")) {
        int limit = configData["config"]["max_responses"].get<int>();
        return (limit > 0) ? limit : 5;
    }
    return 5;
}

std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> requests;
    json requestsData = ReadFile("requests.json");

    if (!requestsData.contains("requests") || !requestsData["requests"].is_array()) {
        std::cerr << "Warning: 'requests' field missing or incorrect in requests.json. Returning empty list." << std::endl;
        return requests;
    }

    for (const auto& request : requestsData["requests"]) {
        requests.push_back(request.get<std::string>());
    }

    return requests;
}

void ConverterJSON::putAnswers(const std::vector<std::vector<RelativeIndex>>& answers) {
    json answersJSON;
    json answersMap = json::object();

    for (size_t i = 0; i < answers.size(); i++) {
        std::string requestId = "request" + std::to_string(i + 1);

        char buffer[10];
        snprintf(buffer, sizeof(buffer), "request%03zu", i + 1);
        requestId = buffer;

        json requestAnswer;
        const auto& relevanceList = answers[i];

        if (relevanceList.empty()) {
            requestAnswer["result"] = "false";
        } else {
            requestAnswer["result"] = "true";

            json relevanceArray = json::array();
            for (const auto& rel : relevanceList) {
                json docInfo;
                docInfo["docid"] = rel.doc_id;
                docInfo["rank"] = std::round(rel.rank * 1000.0f) / 1000.0f;
                relevanceArray.push_back(docInfo);
            }
            requestAnswer["relevance"] = relevanceArray;
        }

        answersMap[requestId] = requestAnswer;
    }

    answersJSON["answers"] = answersMap;
    
    std::ofstream file("answers.json");
    if (file.is_open()) {
        file << answersJSON.dump(4);
    } else {
        std::cerr << "Error: Could not open answers.json for writing." << std::endl;
    }
}