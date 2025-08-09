cmake_minimum_required(VERSION 3.5)

project(SDL_1 LANGUAGES C)

list(APPEND CMAKE_PREFIX_PATH D:/MinGW/LIBS/SDL2-2.28.5/x86_64-w64-mingw32)

list(APPEND CMAKE_PREFIX_PATH D:/MinGW/LIBS/SDL2_image-2.8.2/x86_64-w64-mingw32)

list(APPEND CMAKE_PREFIX_PATH D:/MinGW/LIBS/SDL2_ttf-2.22.0/x86_64-w64-mingw32)

find_package(SDL2 REQUIRED)

find_package(SDL2_image REQUIRED)

find_package(sdl2_ttf REQUIRED)

add_executable(SDL_1 main.c
    menuButton.h
    colors.h
    inputTextBox.h
    textBox.h


)

target_link_libraries(SDL_1 PRIVATE SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

include(GNUInstallDirs)
install(TARGETS SDL_1
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
