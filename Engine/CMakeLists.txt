cmake_minimum_required(VERSION 3.14)

# Project Name
project(Engine)

# Add JoltPhysics as a FetchContent 

# Gather ImGui sources
file(GLOB IMGUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui/*.h")

file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/vendor/*.cpp)

file(GLOB_RECURSE HEADERS ${SOURCE_DIR}/vendor/*.hpp)

# Create an executable for the Engine application
add_library(Engine
STATIC
    ${IMGUI_SOURCES}
    "vendor/glad/glad/glad.c" 
    "vendor/stbi_image/stbi_image.cpp"
    ${SOURCES} 
    ${HEADERS}
 )

# Include directories for the Engine executable
target_include_directories(Engine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stbi_image
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm  
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Jolt  
)

include_directories(vendor/Jolt)

add_subdirectory(vendor/assimp)
add_subdirectory(vendor/glfw)

target_link_libraries(Engine
    PRIVATE
        assimp
        glfw
)

# Copy all required DLLs to the executable's output directory after building
add_custom_command(TARGET Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:assimp>
        $<TARGET_FILE_DIR:Engine>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:glfw>
        $<TARGET_FILE_DIR:Engine>
)

# Optional: Add compile options or definitions
target_compile_options(Engine PRIVATE /W4)

# Message to verify paths during configuration
message(STATUS "LOCATION: ${CMAKE_BINARY_DIR}")
