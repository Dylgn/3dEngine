cmake_minimum_required(VERSION 3.26)

project (3dengine)

set(CMAKE_CXX_FLAGS "-DUNICODE")

# CPP Files
set(SOURCES
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        winmm.lib
)