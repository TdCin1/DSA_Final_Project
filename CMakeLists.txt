cmake_minimum_required(VERSION 3.9)
project(project3)
set(CMAKE_CXX_STANDARD 17)

# SDL2 setup
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Include directories for both SDL2 and SDL2_image
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TEXT_INCLUDE_DIR})

# Add the executable
add_executable(project3
        src/main.cpp
        src/TextureManager.cpp
        src/Game.cpp
        src/Searching.cpp
        src/TUI.cpp
        src/TUI.h
)

# Link both SDL2 and SDL2_image libraries
target_link_libraries(project3
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
)

# Set the linker to console mode explicitly (use -mwindows for GUI apps)
if(WIN32)
    target_link_options(project3 PRIVATE -mconsole)
endif()
