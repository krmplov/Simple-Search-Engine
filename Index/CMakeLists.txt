add_library(Indexer Indexer.h Indexer.cpp)
add_library(QueryParserIndex QueryParserIndex.h QueryParserIndex.cpp)

target_include_directories(Indexer PRIVATE /FilesWork)

target_link_libraries(Indexer PRIVATE FileWorker)
target_link_libraries(Indexer PRIVATE QueryParserIndex)