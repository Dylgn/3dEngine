cmake_minimum_required(VERSION 3.26)

project (3dengine)

set(CMAKE_CXX_FLAGS "-DUNICODE")

# CPP Files
set(SOURCES
    src/main.cpp
    src/GameEngine.cpp
    src/json.cpp
    
    src/Math/V3d.cpp
    src/Math/M4x4.cpp
    src/Math/Triangle.cpp
    src/Math/MathUtility.cpp
    src/Math/V2d.cpp
    src/Math/Simplex.cpp
    src/Math/M3x4.cpp
    src/Math/M3x3.cpp
    src/Math/Quaternion.cpp

    src/Render/Geometry.cpp
    src/Render/Mesh.cpp
    src/Render/Window.cpp
    src/Render/Texture.cpp

    src/Physics/PolyCollider.cpp
    src/Physics/Collision.cpp
    src/Physics/Body.cpp

    src/GameObjects/Object.cpp
    src/GameObjects/Player.cpp
    src/GameObjects/ObjectUtility.cpp
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

#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-mwindows")