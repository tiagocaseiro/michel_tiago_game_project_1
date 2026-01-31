cmake_minimum_required(VERSION 3.21)

add_subdirectory(dependencies/SDL)

project(michel-tiago-game-project-1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCE_FILES_PATH src/*.cpp)

add_executable(michel-tiago-game-project-1 ${SOURCE_FILES_PATH}) 

set_target_properties(michel-tiago-game-project-1 PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(michel-tiago-game-project-1 PRIVATE SDL3::SDL3)

add_custom_command(
        TARGET michel-tiago-game-project-1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                $<TARGET_FILE_DIR:SDL3::SDL3> 
                $<TARGET_FILE_DIR:michel-tiago-game-project-1>)

add_custom_command(
        TARGET michel-tiago-game-project-1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/textures
                $<TARGET_FILE_DIR:michel-tiago-game-project-1>/textures)


