add_library(Searcher Search.h Search.cpp)
add_library(QueryParserSearch QueryParserSearch.h QueryParserSearch.cpp)
add_library(BM25 BM25.h BM25.cpp)
add_library(TF_IDF TF_IDF.h TF_IDF.cpp)

target_include_directories(Indexer PRIVATE /FilesWork)

target_link_libraries(Searcher PRIVATE QueryParserSearch)
target_link_libraries(Searcher PRIVATE TF_IDF)
target_link_libraries(Searcher PRIVATE FileWorker)
target_link_libraries(TF_IDF PRIVATE BM25)