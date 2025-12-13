cmake_minimum_required(VERSION 3.30)
project(NoteManager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADER_FILES
    include/Note.h
    include/Collection.h
    include/ImportantCollection.h
    include/CollectionCounter.h
    include/Subject.h
    include/Observer.h
)

set(SOURCE_FILES 
    src/Note.cpp 
    src/Collection.cpp 
    src/ImportantCollection.cpp
    src/CollectionCounter.cpp
)

include_directories(include)

add_library(notemanager_core STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(notemanager_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(NoteManager main.cpp)
target_link_libraries(NoteManager PRIVATE notemanager_core)

if(MINGW)
    target_compile_options(notemanager_core PRIVATE -Wall -Wextra)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
elseif(MSVC)
    target_compile_options(notemanager_core PRIVATE /W4)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build tests: ${BUILD_TESTS}")