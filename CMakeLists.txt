cmake_minimum_required(VERSION 3.9)
project(Final_Repo)
set(CMAKE_CXX_STANDARD 17)

# SDL2 setup
set(SDL2_PATH "C:/Users/Landon Agic/Documents/DSA Final/SDL2-2.30.9/x86_64-w64-mingw32")
set(SDL2_INCLUDE_DIR "${SDL2_PATH}/include/SDL2")
set(SDL2_LIBRARY "${SDL2_PATH}/lib/libSDL2.dll.a")
set(SDL2MAIN_LIBRARY "${SDL2_PATH}/lib/libSDL2main.a")

# SDL2_image setup
set(SDL2_IMAGE_PATH "C:/Users/Landon Agic/Documents/DSA Final/SDL2_image-2.8.2/x86_64-w64-mingw32")
set(SDL2_IMAGE_INCLUDE_DIR "${SDL2_IMAGE_PATH}/include/SDL2")
set(SDL2_IMAGE_LIBRARY "${SDL2_IMAGE_PATH}/lib/libSDL2_image.dll.a")

#SDL_ttf setup  C:\Users\Landon Agic\Documents\DSA Final\SDL2_ttf-2.22.0\x86_64-w64-mingw32\lib
set(SDL2_TEXT_PATH "C:/Users/Landon Agic/Documents/DSA Final/SDL2_ttf-2.22.0/x86_64-w64-mingw32")
set(SDL2_TEXT_INCLUDE_DIR "${SDL2_TEXT_PATH}/include/SDL2")
set(SDL2_TEXT_LIBRARY "${SDL2_TEXT_PATH}/lib/libSDL2_ttf.dll.a")


# Include directories for both SDL2 and SDL2_image
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TEXT_INCLUDE_DIR})

# Add the executable
add_executable(Final_Repo
        src/main.cpp
        src/Graph.h
        src/csvHandler.h
        src/TextureManager.cpp
        src/TextureManager.h
        src/Game.cpp
        src/Game.h
        src/Searching.h
        src/Searching.cpp
)

# Link both SDL2 and SDL2_image libraries
target_link_libraries(Final_Repo
        ${SDL2_LIBRARY}
        ${SDL2MAIN_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TEXT_LIBRARY}
)

# Set the linker to console mode explicitly (use -mwindows for GUI apps)
target_link_options(Final_Repo PRIVATE -mconsole)
