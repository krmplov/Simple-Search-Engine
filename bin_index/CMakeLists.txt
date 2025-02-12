add_executable(IndexApp main.cpp)

target_link_libraries(IndexApp PRIVATE Indexer)
target_link_libraries(IndexApp PRIVATE QueryParserIndex)
target_link_libraries(IndexApp PRIVATE BM25)
target_link_libraries(IndexApp PRIVATE TF_IDF)
target_link_libraries(IndexApp PRIVATE FileWorker)
target_include_directories(IndexApp PUBLIC ${PROJECT_SOURCE_DIR})