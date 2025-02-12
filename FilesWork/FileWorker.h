#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>

struct TreeNode {
    std::unordered_map<char, std::unique_ptr<TreeNode>> children;
    size_t id_term = 0;
};

class Tree {
public:
    Tree();

    void Insert(const std::string& word, size_t id_term);

    std::unique_ptr<TreeNode> BORTree;

};

class FileWorker {
public:
    void WritePostingLists(const std::unordered_map<std::string, std::vector<std::pair<size_t, size_t>>>& index, const size_t size_all_doc, const size_t count_docs);

    void WriteTree(const Tree& tree);

    void WriteIDDoc(const std::unordered_map<std::string, std::pair<size_t, size_t>>& id_documents_);

    std::pair<size_t, std::string> ReadIDDoc(size_t id_term);

    void ReadTree(Tree& tree);

    std::unordered_map<size_t, std::vector<size_t>> ReadPostingLists(size_t start_position);

    void ReadSizeAndCountDoc();

    std::string GetNameDirectory();

    size_t GetSizeAllDoc();

    size_t GetCount_docs();
private:
    std::string name_result_directory_ = "C:/labs/c++/labwork11-krmplov/cmake-build-debug-mingw/bin_index/index";
    std::string name_result_index_ = "file_index.term";
    std::string name_result_tree = "tree.term";
    std::string name_result_id_doc = "id_doc.term";
    size_t size_all_doc_ = 0;
    size_t count_docs_ = 0;

    void WriteNode(const TreeNode* node, std::ofstream& file);

    void ReadNode(TreeNode* node, std::ifstream& file);
};
