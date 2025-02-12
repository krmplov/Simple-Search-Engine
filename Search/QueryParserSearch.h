#pragma once // проверить порядок

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <stack>
#include <sstream>
#include <memory>


enum class Type {
    WORD,
    AND,
    OR,
    OPEN_PAREN,
    CLOSE_PAREN,
};

struct Element {
    Type type;
    std::string value;
};

struct Node {
    Element element;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

class ParserSearch {
public:
    static std::unique_ptr<Node> GetTree(const std::string& request, std::set<std::string>& posting_lists);

private:
    std::unique_ptr<Node> search_tree_;

    static std::vector<Element> transformation(const std::string& request, std::set<std::string>& posting_lists);

    static std::unique_ptr<Node> build_tree(const std::vector<Element>& elements);
};