cmake_minimum_required(VERSION 3.26)

project (3dengine)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "-DUNICODE")

# CPP Files
set(SOURCES
    src/main.cpp
    
    src/Math/V3d.cpp
    src/Math/M4x4.cpp
    src/Math/Triangle.cpp
    src/Math/MathUtility.cpp

    src/Render/Geometry.cpp
    src/Render/Mesh.cpp

    src/Render/Window.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        include
        ${OPENGL_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        winmm.lib
        ${OPENGL_LIBRARIES}
)

#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-mwindows")