cmake_minimum_required(VERSION 3.0.0)
project( stack-machine VERSION 0.1.0 LANGUAGES C CXX)

add_executable( stack-machine visualisation-sdl/visualisation.cpp )

target_link_libraries(  stack-machine SDL2main SDL2 )
target_link_libraries(  stack-machine SDL2 SDL2_image)
target_link_libraries(  stack-machine SDL2_ttf SDL2)
