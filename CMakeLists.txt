cmake_minimum_required(VERSION 3.21)

project(michel-tiago-game-project-1 LANGUAGES CXX)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCE_FILES_PATH 
        src/*.cpp
        src/Common/*.cpp 
        src/Input/*.cpp
        src/GameUI/*.cpp
        src/Model/*.cpp 
	src/Rules/*.cpp
	src/Actions/*.cpp
        src/UI/Core/*.cpp 
        src/UI/Game/*.cpp 
        src/Tools/*.cpp
        dependencies/imgui/*.cpp 
        dependencies/imgui/backends/imgui_impl_sdl3.cpp 
        dependencies/imgui/backends/imgui_impl_sdlrenderer3.cpp
)

add_executable(michel-tiago-game-project-1 ${SOURCE_FILES_PATH})

target_include_directories(michel-tiago-game-project-1 PUBLIC 
        dependencies/imgui
        dependencies/imgui/backends
        src
)

set_target_properties(michel-tiago-game-project-1 PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(michel-tiago-game-project-1 PRIVATE SDL3::SDL3)
target_link_libraries(michel-tiago-game-project-1 PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)
target_link_libraries(michel-tiago-game-project-1 PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(michel-tiago-game-project-1 PRIVATE pugixml)

add_custom_command(
        TARGET michel-tiago-game-project-1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/textures
                $<TARGET_FILE_DIR:michel-tiago-game-project-1>/textures)

add_custom_command(
        TARGET michel-tiago-game-project-1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/fonts
                $<TARGET_FILE_DIR:michel-tiago-game-project-1>/fonts)

add_custom_command(
        TARGET michel-tiago-game-project-1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/ui
                $<TARGET_FILE_DIR:michel-tiago-game-project-1>/ui)