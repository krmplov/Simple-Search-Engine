#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <set>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <vector>
#include <memory>


#include "FileWorker.h"
#include "QueryParserIndex.h"

class Indexer {
public:
    void Run(int argc, char** argv);
private:
    FileWorker file_input_;
    std::unordered_map<std::string, std::vector<std::pair<size_t, size_t>>> index_;
    std::unordered_map<std::string, std::pair<size_t, size_t>> id_documents_;
    size_t count_documents_ = 0;
    size_t size_all_doc = 0;

    void CreateIndex(const std::string& directory, const std::set<std::string>& exclude);

    void IndexFile(const std::string& filename);

    void CreateBORTree();
};