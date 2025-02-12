#pragma once

#include <iostream>
#include <string>
#include <set>
#include <algorithm>

class ParserIndex {
public:
    void ParseIndex(int argc, char** argv);

    std::string GetNameDirectory() const;

    std::set<std::string> GetExclude() const;

    bool GetFLag() const;

private:
    bool parse_ = false;
    bool help = false;
    std::string name_directory;
    std::set<std::string> exclude_;

    void OutPutHelp();
};