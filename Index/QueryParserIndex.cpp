#include "QueryParserIndex.h"

void ParserIndex::ParseIndex(int argc, char **argv) {
    for (size_t i = 1; i < argc; ++i) {
        std::string query(argv[i]);
        if (query.substr(0, 2) == "--") {
            query = query.substr(2, query.size() - 1);
            if (query == "help") {
                this->help = true;
                this->OutPutHelp();
            } else if (query == "exclude") {
                if ((i = argc - 1) || (std::string(argv[i + 1]).substr(0, 2) == "--")) {
                    throw std::runtime_error("there isn't enough data for the argument");
                }
                while (i < argc - 1 && std::string(argv[i + 1]).substr(0, 2) != "--") {
                    ++i;
                    this->exclude_.insert(std::string(argv[i]));
                }
            } else if (query == "path") {
                this->parse_ = true;
                ++i;
                this->name_directory = std::string(argv[i]);
            } else {
                this->OutPutHelp();
            }
        } else {
            this->OutPutHelp();
        }
    }
    if (!this->help && !this->parse_) {
        this->OutPutHelp();
    }
}

bool ParserIndex::GetFLag() const {
    return this->parse_;
}

std::string ParserIndex::GetNameDirectory() const {
    return this->name_directory;
}

std::set<std::string> ParserIndex::GetExclude() const {
    return this->exclude_;
}

void ParserIndex::OutPutHelp() {
    std::cout << "to specify the directory for indexing, enter \"--path\" and then enter the name of the directory";
    std::cout << '\n';
    std::cout << "to indicate the names of files or directories that don't need to be taken into account when indexing,";
    std::cout << "enter \"--exclude\" and then enter all the names of files or directories";
}
