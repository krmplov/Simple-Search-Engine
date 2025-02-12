#pragma once // роверить порядок

#include "QueryParserSearch.h"
#include "TF_IDF.h"
#include <FileWorker.h>

#include <algorithm>

class Search {
public:
    void Run(std::string& request);
private:
    TF_IDF tf_idf_;
    FileWorker file_output_;
    std::set<std::string> posting_lists_;
    Tree tree_bor;
    std::unordered_map<size_t, std::pair<size_t, std::string>> name_doc_;
    std::unordered_map<size_t, std::vector<size_t>> line_index_;

    std::unordered_map<size_t, double> RelevanceOfWords(Node* tree);

    size_t FindPosition(const std::string& word);
};