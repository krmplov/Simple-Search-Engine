#include "Search.h"

void Search::Run(std::string& request) {
    std::unique_ptr<Node> tree_search = ParserSearch::GetTree(request, this->posting_lists_);
    this->file_output_ = FileWorker{};
    this->file_output_.ReadSizeAndCountDoc();
    this->tf_idf_ = TF_IDF(this->file_output_.GetSizeAllDoc(), this->file_output_.GetCount_docs());
    this->file_output_.ReadTree(this->tree_bor);
    std::unordered_map<size_t, double> res = RelevanceOfWords(tree_search.get());
    std::vector<std::pair<size_t, double>> res_sort(res.begin(), res.end());
    std::sort(res_sort.begin(), res_sort.end(),[](std::pair<size_t, double> a, std::pair<size_t, double> b){return a.second > b.second;});
    for (const auto& pair : res_sort) {
        std::cout << this->name_doc_[pair.first].second << " :\n";
        for (const auto& line : this->line_index_[pair.first]) {
            std::cout << "- " << line << " line\n";
        }
        std::cout << '\n';
    }
    this->line_index_.clear();
}

std::unordered_map<size_t, double> Search::RelevanceOfWords(Node* tree) {
    if (tree->element.type == Type::WORD) {
        std::unordered_map<size_t, std::vector<size_t>> index_word = this->file_output_.ReadPostingLists(FindPosition(tree->element.value));
        for (const auto& pair : index_word) {
            this->line_index_[pair.first].insert(this->line_index_[pair.first].begin(), pair.second.begin(), pair.second.end());
        }
        std::unordered_map<size_t, double> res;
        for (const auto& pair : index_word) {
            if (this->name_doc_.count(pair.first) == 0) {
                this->name_doc_[pair.first] = this->file_output_.ReadIDDoc(pair.first);
            }
            res[pair.first] = this->tf_idf_.Score(pair.second.size(), this->name_doc_[pair.first].first, index_word.size());
        }
        return res;
    } else if (tree->element.type == Type::OR) {
        std::unordered_map<size_t, double> res_left = RelevanceOfWords(tree->left.get());
        std::unordered_map<size_t, double> res_right = RelevanceOfWords(tree->right.get());
        for (const auto& pair : res_left) {
            if (res_right.count(pair.first) == 0) {
                res_right[pair.first] = pair.second;
            } else {
                res_right[pair.first] += pair.second;
            }
        }
        return res_right;
    } else if (tree->element.type == Type::AND) {
        std::unordered_map<size_t, double> res_left = RelevanceOfWords(tree->left.get());
        std::unordered_map<size_t, double> res_right = RelevanceOfWords(tree->right.get());
        std::unordered_map<size_t, double> res;
        for (const auto& pair : res_left) {
            if (res_right.count(pair.first) != 0) {
                res[pair.first] = pair.second + res_right[pair.first];
            }
        }
        return res;
    }
}

size_t Search::FindPosition(const std::string& word) {
    TreeNode* temp = this->tree_bor.BORTree.get();
    for (const char c : word) {
        if (temp->children.count(c) == 0) {
            throw std::runtime_error("The word is not among the documents");
        }
        temp = temp->children[c].get();
    }
    return temp->id_term;
}
