#include "FileWorker.h"

Tree::Tree() {
    this->BORTree = std::make_unique<TreeNode>();
}

void Tree::Insert(const std::string& word, size_t id_term) {
    TreeNode* temp = this->BORTree.get();
    for (const char c : word) {
        if (temp->children.count(c) == 0) {
            temp->children[c] = std::make_unique<TreeNode>();
        }
        temp = temp->children[c].get();
    }
    temp->id_term = id_term;
}

void FileWorker::WritePostingLists(const std::unordered_map<std::string, std::vector<std::pair<size_t, size_t>>>& index, const size_t size_all_doc, const size_t count_docs) {
    std::string filename = this->name_result_directory_ + "/" + this->name_result_index_;
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("the file file_index.term didn't open");
    }
    file.write(reinterpret_cast<const char*>(&size_all_doc), sizeof(size_all_doc));
    file.write(reinterpret_cast<const char*>(&count_docs), sizeof(count_docs));
    for (const auto& pair : index) {
        size_t count_doc = pair.second.size();
        file.write(reinterpret_cast<const char*>(&count_doc), sizeof(count_doc));
        for (const auto& doc : pair.second) {
            file.write(reinterpret_cast<const char*>(&doc.first), sizeof(doc.first));
            file.write(reinterpret_cast<const char*>(&doc.second), sizeof(doc.second));
        }
    }
    file.close();
}

void FileWorker::WriteNode(const TreeNode *node, std::ofstream &file) {
    size_t children_count = node->children.size();
    file.write(reinterpret_cast<const char*>(&node->id_term), sizeof(node->id_term));
    file.write(reinterpret_cast<const char*>(&children_count), sizeof(children_count));
    for (const auto& pair : node->children) {
        file.write(&pair.first, sizeof(pair.first));
        WriteNode(pair.second.get(), file);
    }
}

void FileWorker::WriteTree(const Tree& tree) {
    std::string filename = this->name_result_directory_ + "/" + this->name_result_tree;
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("the file tree.term didn't open");
    }
    WriteNode(tree.BORTree.get(), file);
    file.close();
}

void FileWorker::WriteIDDoc(const std::unordered_map<std::string, std::pair<size_t, size_t>>& id_documents_) {
    std::string filename = this->name_result_directory_ + "/" + this->name_result_id_doc;
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("the file id_doc.term didn't open");
    }
    for (const auto& pair : id_documents_) {
        size_t size_doc_name = pair.first.size();
        file.write(reinterpret_cast<const char*>(&pair.second.first), sizeof(pair.second.first));
        file.write(reinterpret_cast<const char*>(&size_doc_name), sizeof(size_doc_name));
        file.write(reinterpret_cast<const char*>(&pair.second.second), sizeof(pair.second.second));
        file.write(pair.first.c_str(), size_doc_name);
    }
    file.close();
}

std::pair<size_t, std::string> FileWorker::ReadIDDoc(size_t target_id) {
    std::string filename = this->name_result_directory_ + "/" + this->name_result_id_doc;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("the file id_doc.term didn't open");
    }
    size_t id;
    size_t size_doc_name;
    size_t len_doc;
    while (file.read(reinterpret_cast<char*>(&id), sizeof(id))) {
        if (file.eof()) {
            throw std::runtime_error("end of the file id_doc.term reached");
        }
        file.read(reinterpret_cast<char*>(&size_doc_name), sizeof(size_doc_name));
        if (file.eof()) {
            throw std::runtime_error("end of the file id_doc.term reached");
        }
        if (id == target_id) {
            file.read(reinterpret_cast<char*>(&len_doc), sizeof(len_doc));
            if (file.eof()) {
                throw std::runtime_error("end of the file id_doc.term reached");
            }
            std::string doc_name(size_doc_name, '\0');
            file.read(&doc_name[0], size_doc_name);
            return std::make_pair(len_doc, doc_name);
        } else {
            file.seekg(size_doc_name + sizeof(size_t), std::ios_base::cur);
            if (file.eof()) {
                throw std::runtime_error("end of the file id_doc.term reached");
            }
        }
    }
    file.close();
}


void FileWorker::ReadNode(TreeNode* node, std::ifstream& file) {
    size_t childrenCount;
    file.read(reinterpret_cast<char*>(&node->id_term), sizeof(node->id_term));
    if (file.eof()) {
        throw std::runtime_error("end of the file tree.term reached");
    }
    file.read(reinterpret_cast<char*>(&childrenCount), sizeof(childrenCount));
    if (file.eof()) {
        throw std::runtime_error("end of the file tree.term reached");
    }
    for (size_t i = 0; i < childrenCount; ++i) {
        char c;
        file.read(&c, sizeof(c));
        if (file.eof()) {
            throw std::runtime_error("end of the file tree.term reached");
        }
        node->children[c] = std::make_unique<TreeNode>();
        ReadNode(node->children[c].get(), file);
    }
}

void FileWorker::ReadTree(Tree& tree) {
    std::string filename = this->name_result_directory_ + "/" + this->name_result_tree;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("the file tree.term didn't open");
    }
    tree.BORTree = std::make_unique<TreeNode>();
    ReadNode(tree.BORTree.get(), file);
    file.close();
}

std::unordered_map<size_t, std::vector<size_t>> FileWorker::ReadPostingLists(size_t start_position) {
    std::string filename = this->name_result_directory_ + "/" + this->name_result_index_;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("the file file_index.term didn't open");
    }
    std::unordered_map<size_t, std::vector<size_t>> result;

    file.seekg(start_position, std::ios_base::beg);

    size_t count_doc;
    file.read(reinterpret_cast<char*>(&count_doc), sizeof(count_doc));
    if (file.eof()) {
        throw std::runtime_error("end of the file file_index.term reached");
    }

    for (size_t i = 0; i < count_doc; ++i) {
        std::pair<size_t, size_t> doc;
        file.read(reinterpret_cast<char*>(&doc.first), sizeof(doc.first));
        if (file.eof()) {
            throw std::runtime_error("end of the file file_index.term reached");
        }
        file.read(reinterpret_cast<char*>(&doc.second), sizeof(doc.second));
        if (file.eof()) {
            throw std::runtime_error("end of the file file_index.term reached");
        }
        result[doc.first].push_back(doc.second);
    }

    file.close();
    return result;
}

void FileWorker::ReadSizeAndCountDoc() {
    std::string filename = this->name_result_directory_ + "/" + this->name_result_index_;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("the file file_index.term didn't open");
    }
    file.read(reinterpret_cast<char*>(&this->size_all_doc_), sizeof(this->size_all_doc_));
    if (file.eof()) {
        throw std::runtime_error("end of the file file_index.term reached");
    }
    file.read(reinterpret_cast<char*>(&this->count_docs_), sizeof(this->count_docs_));
    if (file.eof()) {
        throw std::runtime_error("end of the file file_index.term reached");
    }
    file.close();
}


std::string FileWorker::GetNameDirectory() {
    return this->name_result_directory_;
}

size_t FileWorker::GetSizeAllDoc() {
    return this->size_all_doc_;
}

size_t FileWorker::GetCount_docs() {
    return this->count_docs_;
}
