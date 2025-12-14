#pragma once
#include "InvertedIndex.hpp"
#include "ConverterJSON.hpp"
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <iostream> 


class SearchServer {
private:
    InvertedIndex& _index;
    std::vector<std::string> GetSortedUniqueWords(const std::string& query);
public:
    SearchServer(InvertedIndex& idx) : _index(idx) {};
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input, size_t maxResponses = 5);
};