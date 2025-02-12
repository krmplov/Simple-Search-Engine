cmake_minimum_required(VERSION 3.12)

project(
        SimpleSearchEngine
        VERSION 1.0
        DESCRIPTION "C++ Year 2024 labwork11"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)


add_subdirectory(Index)
add_subdirectory(Search)
add_subdirectory(FilesWork)
add_subdirectory(bin_index)
add_subdirectory(bin_search)

enable_testing()
add_subdirectory(tests)