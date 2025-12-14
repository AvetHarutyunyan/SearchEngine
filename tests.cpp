#include "gtest/gtest.h"
#include "InvertedIndex.hpp"
#include "SearchServer.hpp"
#include "ConverterJSON.hpp"
#include <iostream>

using std::string;

void TestInvertedIndexFunctionality(const std::vector<string>& docs, const std::vector<string>& requests,
                                    const std::vector<std::vector<Entry>>& expected) 
{
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;

    idx.UpdateDocumentBase(docs);

    for (const auto& request : requests) {
        std::vector<Entry> wordCount = idx.GetWordCount(request);
        result.push_back(wordCount);
    }
    
    ASSERT_EQ(result.size(), expected.size());
    for (size_t i = 0; i < result.size(); i++) {
        ASSERT_EQ(result[i].size(), expected[i].size()) << "Mismatch in request " << requests[i];
        for (size_t j = 0; j < result[i].size(); j++)
        {
            EXPECT_EQ(result[i][j], expected[i][j]) << "Mismatch in request " << requests[i]
                                                    << " at document index " << j;
        }
    }
}

void TestSearchServerFunctionality(const std::vector<string>& docs, const std::vector<string>& requests,
                                    const std::vector<std::vector<RelativeIndex>>& expected,
                                    size_t maxResponses = 5
) {
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);

    SearchServer srv(idx); 

    std::vector<std::vector<RelativeIndex>> result = srv.search(requests, maxResponses);
    
    ASSERT_EQ(result.size(), expected.size());
    for (size_t i = 0; i < result.size(); ++i) {
        ASSERT_EQ(result[i].size(), expected[i].size()) << "Mismatch in request count " << requests[i];
        for (size_t j = 0; j < result[i].size(); ++j) {
            EXPECT_EQ(result[i][j].doc_id, expected[i][j].doc_id) << "Mismatch in doc_id, request " << i << " index " << j;
            
            EXPECT_NEAR(result[i][j].rank, expected[i][j].rank, 0.000001f) << "Mismatch in rank, request " << i << " index " << j;
        }
    }
}

TEST(TestCaseInvertedIndex, TestBasic) {
    const std::vector<std::string> docs = {
        "london is the capital of great britain",
        "big ben is the nickname for the great bell of the striking clock"
    };
    const std::vector<string> requests = {"london", "the"};
    const std::vector<std::vector<Entry>> expected = {
        {
            {0, 1}
        },
        {
            {0, 1},
            {1, 3}
        }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestBasic2) {
    const std::vector<string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
    };
    const std::vector<string> requests = {"milk", "water", "cappuccino"};
    const std::vector<std::vector<Entry>> expected = {
        {
            {0, 4},
            {1, 1},
            {2, 5}
        },
        {
            {0, 3},
            {1, 2},
            {2, 5}
        },
        {
            {3, 1}
        }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST (TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    const std::vector<string> docs = {
        "abcdefghijkl",
        "statement"
    };
    const std::vector<string> requests = {"m", "statement"};
    const std::vector<std::vector<Entry>> expected = {
        {},
        {
            {1, 1}
        }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseSearchServer, TestSimple) { 
    const std::vector<string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
    };
    const std::vector<string> request = {"milk water", "sugar"};
    
    
    const std::vector<std::vector<RelativeIndex>> expected = {
        {
            {2, 1.0f},
            {0, 0.7f},
            {1, 0.3f}
        },
        {}
    };
    TestSearchServerFunctionality(docs, request, expected);
}

TEST(TestCaseSearchServer, TestTop5)
{ 
    const std::vector<string> docs = {
        "london is the capital of great britain", "paris is the capital of france",
        "berlin is the capital of germany", "rome is the capital of italy",
        "madrid is the capital of spain", "lisboa is the capital of portugal",
        "bern is the capital of switzerland", "moscow is the capital of russia",
        "kiev is the capital of ukraine", "minsk is the capital of belarus",
        "astana is the capital of kazakhstan", "beijing is the capital of china",
        "tokyo is the capital of japan", "bangkok is the capital of thailand",
        "welcome to moscow the capital of russia the third rome", "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland", "oslo is the capital of norway",
        "stockholm is the capital of sweden", "riga is the capital of latvia",
        "tallinn is the capital of estonia", "warsaw is the capital of poland",
    };
    const std::vector<string> request = {"moscow is the capital of russia"};

    const std::vector<std::vector<RelativeIndex>> expected = {
        {
            {7, 1.0f},
            {14, 1.0f}, 
            {0, 0.666666687f},
            {1, 0.666666687f},
            {2, 0.666666687f}
        }
    };
    TestSearchServerFunctionality(docs, request, expected, 5);
}