#include <Index/Indexer.h>
#include <Search/Search.h>
#include <iostream>

int main(int argc, char** argv) {
    std::cout << "to exit the programs, enter: EXIT\n";
    try {
        Search search;
        while (true) {
            std::string res;
            std::getline(std::cin, res);
            if (res == "EXIT") {
                break;
            }
            search.Run(res);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
    return 0;
}