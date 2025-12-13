cmake_minimum_required(VERSION 3.30)

add_subdirectory(lib/googletest)

include_directories(
    ${gtest_SOURCE_DIR}/include
    ${gtest_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

set(TEST_FILES 
    RunAllTests.cpp
    NoteTest.cpp
    CollectionTest.cpp
    ObserverTest.cpp
    IntegrationTest.cpp
    NoteManagerTest.cpp
)

add_executable(RunAllTests ${TEST_FILES})

target_link_libraries(RunAllTests 
    gtest
    notemanager_core
)