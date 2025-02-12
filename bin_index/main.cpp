#include <Index/Indexer.h>
#include <Search/Search.h>
#include <FilesWork/FileWorker.h>
#include <iostream>

int main(int argc, char** argv) {
    try {
        Indexer indexer;
        indexer.Run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
    return 0;
}