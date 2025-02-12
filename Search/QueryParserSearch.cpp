#include <algorithm>
#include "QueryParserSearch.h"

std::unique_ptr<Node> ParserSearch::GetTree(const std::string &request, std::set<std::string>& posting_lists) {
    return build_tree(transformation(request, posting_lists));
}

std::vector<Element> ParserSearch::transformation(const std::string& request, std::set<std::string>& posting_lists) {
    std::vector<Element> result;
    std::istringstream request_str(request);
    std::string word;
    while (request_str >> word) {
        posting_lists.insert(word);
        if (word[0] == '(' && word[word.size() - 1] == ')') {
            result.push_back({Type::OPEN_PAREN, ""});
            word = word.substr(1, word.size() - 2);
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            result.push_back({Type::WORD, word});
            result.push_back({Type::CLOSE_PAREN, ""});
        } else if (word[0] == '(') {
            result.push_back({Type::OPEN_PAREN, ""});
            word = word.substr(1, word.size() - 1);
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            result.push_back({Type::WORD, word});
        } else if (word[word.size() - 1] == ')') {
            word = word.substr(0, word.size() - 1);
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            result.push_back({Type::WORD, word});
            result.push_back({Type::CLOSE_PAREN, ""});
        } else if (word == "AND") {
            result.push_back({Type::AND, ""});
        } else if (word == "OR") {
            result.push_back({Type::OR, ""});
        } else {
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            result.push_back({Type::WORD, word});
        }
    }
    return result;
}

std::unique_ptr<Node> ParserSearch::build_tree(const std::vector<Element>& elements) {
    std::stack<std::unique_ptr<Node>> nodes;
    std::stack<Type> priority;

    for (size_t i = 0; i < elements.size(); ++i) {
        const Element& element = elements[i];
        if (element.type == Type::AND || element.type == Type::OR) {
            if (i == 0 || (elements[i - 1].type != Type::WORD && elements[i - 1].type != Type::CLOSE_PAREN)) {
                throw std::runtime_error("Invalid query: AND/OR operator without preceding operand");
            }
            if (i == elements.size() - 1 || (elements[i + 1].type != Type::WORD && elements[i + 1].type != Type::OPEN_PAREN)) {
                throw std::runtime_error("Invalid query: the AND/OR operator without a subsequent operand");
            }
        }
    }
    for (size_t i = 0; i < elements.size() - 1; ++i) {
        const Element& current = elements[i];
        const Element& next = elements[i + 1];

        if (current.type == Type::WORD && next.type == Type::WORD) {
            throw std::runtime_error("Invalid query: two words go in a row without an operator");
        }
    }


    for (const Element& element : elements) {
        if (element.type == Type::WORD) {
            nodes.push(std::make_unique<Node>(Node{element, nullptr, nullptr}));
        } else if (element.type == Type::OPEN_PAREN) {
            priority.push(Type::OPEN_PAREN);
        } else if (element.type == Type::CLOSE_PAREN) {
            while (!priority.empty() && priority.top() != Type::OPEN_PAREN) {
                std::unique_ptr<Node> right = std::move(nodes.top());
                nodes.pop();
                std::unique_ptr<Node> left = std::move(nodes.top());
                nodes.pop();
                nodes.push(std::make_unique<Node>(Node{{priority.top(), ""}, std::move(left), std::move(right)}));
                priority.pop();
            }
            priority.pop();
        } else {
            while (!priority.empty() && priority.top() != Type::OPEN_PAREN &&
                   (element.type == Type::OR || (element.type == Type::AND && priority.top() == Type::AND))) {
                std::unique_ptr<Node> right = std::move(nodes.top());
                nodes.pop();
                std::unique_ptr<Node> left = std::move(nodes.top());
                nodes.pop();
                nodes.push(std::make_unique<Node>(Node{{priority.top(), ""}, std::move(left), std::move(right)}));
                priority.pop();
            }
            priority.push(element.type);
        }
    }

    while (!priority.empty()) {
        std::unique_ptr<Node> right = std::move(nodes.top());
        nodes.pop();
        std::unique_ptr<Node> left = std::move(nodes.top());
        nodes.pop();
        nodes.push(std::make_unique<Node>(Node{{priority.top(), ""}, std::move(left), std::move(right)}));
        priority.pop();
    }

    return std::move(nodes.top());
}
