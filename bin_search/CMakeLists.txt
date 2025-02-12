add_executable(SearchApp main.cpp)

target_link_libraries(SearchApp PRIVATE Searcher)
target_link_libraries(SearchApp PRIVATE QueryParserSearch)
target_link_libraries(SearchApp PRIVATE BM25)
target_link_libraries(SearchApp PRIVATE TF_IDF)
target_link_libraries(SearchApp PRIVATE FileWorker)
target_include_directories(SearchApp PUBLIC ${PROJECT_SOURCE_DIR})