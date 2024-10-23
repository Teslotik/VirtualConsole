cmake_minimum_required(VERSION 3.5.0)

project(VirtualConsole)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)
cmake_policy(SET CMP0074 NEW)

set(SOURCE_FILES
    src/main.cpp

    src/sandbox.cpp
    "src/program/JsProgram.cpp"
    # "src/program/scheme.cpp"
)


set(SDLIMAGE_AVIF OFF)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build a SDL static library (if available)")

add_subdirectory(modules/SDL EXCLUDE_FROM_ALL)
add_subdirectory(modules/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(modules/tinyjs)

add_executable(VirtualConsole ${SOURCE_FILES})

include_directories(src)
include_directories(modules/tinyjs)
include_directories(modules/tinyjs/include)
include_directories(modules/tinyjs/include/tinyjs)

target_link_libraries(VirtualConsole PRIVATE SDL3_image::SDL3_image-static SDL3::SDL3)
target_link_libraries(VirtualConsole PRIVATE ${SDL3_LIBRARIES})
target_link_libraries(VirtualConsole PRIVATE ${SDL3IMAGE_LIBRARIES})
target_link_libraries(VirtualConsole PRIVATE TinyJS)

add_custom_command(TARGET VirtualConsole POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)

add_custom_command(TARGET VirtualConsole POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/script"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/script"
)