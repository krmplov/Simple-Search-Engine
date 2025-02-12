#include "Indexer.h"

void Indexer::Run(int argc, char** argv) {
    ParserIndex parser;
    parser.ParseIndex(argc, argv);
    if (parser.GetFLag()) {
        std::filesystem::create_directory(this->file_input_.GetNameDirectory());
        CreateIndex(parser.GetNameDirectory(), parser.GetExclude());
        this->file_input_.WritePostingLists(this->index_, (this->size_all_doc / this->count_documents_), this->count_documents_);
        CreateBORTree();
        this->file_input_.WriteIDDoc(this->id_documents_);
    }
}

void Indexer::CreateIndex(const std::string& directory, const std::set<std::string>& exclude) {
    for (const auto& file : std::filesystem::directory_iterator(directory)) {
        if (exclude.count(file.path().string()) == 0) {
            if (std::filesystem::is_directory(file)) {
                CreateIndex(file.path().string(), exclude);
            } else {
                IndexFile(file.path().string());
            }
        }
    }
}

void Indexer::IndexFile(const std::string& filename) {
    this->id_documents_[filename].first = this->count_documents_;
    ++this->count_documents_;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("the file didn't open");
    }
    std::string line;
    size_t line_number = 0;
    size_t size_doc = 0;
    while (std::getline(file, line)) {
        ++line_number;
        std::istringstream line_str(line);
        std::string word;
        while (line_str >> word) {
            ++size_doc;
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            this->index_[word].emplace_back(this->id_documents_[filename].first, line_number);
        }
    }
    this->id_documents_[filename].second = size_doc;
    this->size_all_doc += size_doc;
}

void Indexer::CreateBORTree() {
    size_t id_term = sizeof(this->size_all_doc) * 2;
    Tree tree;
    for (const auto& pair : this->index_) {
        tree.Insert(pair.first, id_term);
        id_term += sizeof(size_t) * (pair.second.size() * 2 + 1);
    }

    this->file_input_.WriteTree(tree);
}



