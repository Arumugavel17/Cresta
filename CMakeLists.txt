
cmake_minimum_required(VERSION 3.20)

project(Engine) 

include(FetchContent)

# The configurations we support
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Distribution")

# When turning this option on, the library will be compiled using doubles for positions. This allows for much bigger worlds.
set(DOUBLE_PRECISION OFF)

# When turning this option on, the library will be compiled with debug symbols
set(GENERATE_DEBUG_SYMBOLS ON)

# When turning this option on, the library will override the default CMAKE_CXX_FLAGS_DEBUG/RELEASE values, otherwise they will use the platform defaults
set(OVERRIDE_CXX_FLAGS ON)

# When turning this option on, the library will be compiled in such a way to attempt to keep the simulation deterministic across platforms
set(CROSS_PLATFORM_DETERMINISTIC OFF)

# When turning this option on, the library will be compiled with interprocedural optimizations enabled, also known as link-time optimizations or link-time code generation.
# Note that if you turn this on you need to use SET_INTERPROCEDURAL_OPTIMIZATION() or set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON) to enable LTO specifically for your own project as well.
# If you don't do this you may get an error: /usr/bin/ld: libJolt.a: error adding symbols: file format not recognized
set(INTERPROCEDURAL_OPTIMIZATION ON)

# When turning this on, in Debug and Release mode, the library will emit extra code to ensure that the 4th component of a 3-vector is kept the same as the 3rd component 
# and will enable floating point exceptions during simulation to detect divisions by zero. 
# Note that this currently only works using MSVC. Clang turns Float2 into a SIMD vector sometimes causing floating point exceptions (the option is ignored).
set(FLOATING_POINT_EXCEPTIONS_ENABLED OFF)

# When turning this on, the library will be compiled with C++ exceptions enabled.
# This adds some overhead and Jolt doesn't use exceptions so by default it is off.
set(CPP_EXCEPTIONS_ENABLED OFF)

# When turning this on, the library will be compiled with C++ RTTI enabled.
# This adds some overhead and Jolt doesn't use RTTI so by default it is off.
set(CPP_RTTI_ENABLED OFF)

# Number of bits to use in ObjectLayer. Can be 16 or 32.
set(OBJECT_LAYER_BITS 16)

# Select X86 processor features to use, by default the library compiles with AVX2, if everything is off it will be SSE2 compatible.
set(USE_SSE4_1 ON)
set(USE_SSE4_2 ON)
set(USE_AVX ON)
set(USE_AVX2 ON)
set(USE_AVX512 OFF)
set(USE_LZCNT ON)
set(USE_TZCNT ON)
set(USE_F16C ON)
set(USE_FMADD ON)

# Include Jolt
FetchContent_Declare(
        JoltPhysics
        GIT_REPOSITORY "https://github.com/jrouwe/JoltPhysics"
        GIT_TAG "v5.2.0"
		SOURCE_SUBDIR "Build"
)
FetchContent_MakeAvailable(JoltPhysics)

# Requires C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
	# 64 bit architecture
	set(CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE "x64")

	# Set runtime library
	if (USE_STATIC_MSVC_RUNTIME_LIBRARY)
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endif()
endif()

list(FILTER SOURCES EXCLUDE REGEX "${PROJECT_SOURCE_DIR}/src/exclude_folder/.*")

# Set linker flags
set(CMAKE_EXE_LINKER_FLAGS_DISTRIBUTION "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

# Enable link time optimization in Release and Distribution mode if requested and available
SET_INTERPROCEDURAL_OPTIMIZATION()

#add_subdirectory(Editor)
add_subdirectory(Engine/vendor/json)
add_subdirectory(Engine/vendor/assimp)
add_subdirectory(Engine/vendor/glfw)
add_subdirectory(Engine/vendor/spdlog)
add_subdirectory(Engine/vendor/yaml-cpp)
add_subdirectory(Engine/vendor/sol2-3.3.0)

include_directories(Engine/src/)
include_directories(Engine/vendor/entt)
include_directories(Engine/vendor/json/include)
include_directories(Engine/vendor/lua/include)
include_directories(Engine/vendor/assimp/include)
include_directories(Engine/vendor/yaml-cpp/include)
include_directories(Engine/vendor/sol2-3.3.0/include)


file(GLOB IMGUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/imgui/imgui/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/imgui/imgui/*.h")
file(GLOB IMGUIZMO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/ImGuizmo/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/ImGuizmo/*.h")

file(GLOB_RECURSE ENGINE_SRC_SOURCES 
    ${PROJECT_SOURCE_DIR}/Engine/src/*.cpp)

file(GLOB_RECURSE ENGINE_SRC_HEADERS 
    ${PROJECT_SOURCE_DIR}/Engine/src/*.hpp)

file(GLOB_RECURSE ENGINE_VENDOR_SOURCES 
    ${PROJECT_SOURCE_DIR}/Engine/vendor/glad/*.cpp 
    ${PROJECT_SOURCE_DIR}/Engine/vendor/imgui/*.cpp 
    ${PROJECT_SOURCE_DIR}/Engine/vendor/stbi_image/*.cpp)

file(GLOB_RECURSE ENGINE_VENDOR_HEADERS 
    ${PROJECT_SOURCE_DIR}/Engine/vendor/glad/*.hpp 
    ${PROJECT_SOURCE_DIR}/Engine/vendor/imgui/*.hpp 
    ${PROJECT_SOURCE_DIR}/Engine/vendor/stbi_image/*.hpp)

message(STATUS "UTF-8 Unicode support is enabled")

add_library(Engine
STATIC
		${IMGUI_SOURCES}
		${IMGUIZMO_SOURCES}
		"Engine/src/EntryPoint.cpp"
		"Engine/vendor/glad/glad/glad.c" 
		"Engine/vendor/stbi_image/stbi_image.cpp"
		${ENGINE_SRC_SOURCES} 
		${ENGINE_SRC_HEADERS}
		${ENGINE_VENDOR_SOURCES} 
		${ENGINE_VENDOR_HEADERS})

# Include directories for the Engine executable
target_include_directories(Engine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/src
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/glad
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/ImGuizmo
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/stbi_image
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/assimp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/spdlog/include  
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/yaml-cpp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/entt/entt/single_include
	${JoltPhysics_SOURCE_DIR}/..
)

message("SOURCES " ${CMAKE_BINARY_DIR})
set(LUA_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/lua)
message("LUA PATH: " ${LUA_LIBRARY})
target_link_libraries(Engine
        glfw
		assimp
		Jolt
        yaml-cpp
        ${LUA_LIBRARY}/lua54.lib
        nlohmann_json::nlohmann_json
)

target_compile_definitions(Engine PRIVATE 
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
    $<$<CONFIG:RelWithDebInfo>:RELWITHDEBINFO>
    $<$<CONFIG:MinSizeRel>:MINSIZEREL>
)

target_compile_options(Engine PRIVATE "/utf-8")

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "Engine")
project(Editor)

file(GLOB_RECURSE ENGINE_SRC_SOURCES ${PROJECT_SOURCE_DIR}/Editor/src/*.cpp)

file(GLOB_RECURSE ENGINE_SRC_HEADERS ${PROJECT_SOURCE_DIR}/Editor/src/*.hpp)

add_executable(Editor
    ${ENGINE_SRC_SOURCES}
    ${ENGINE_SRC_HEADERS}
    ) 


set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/Engine/assets")
set(ASSETS_DEST_DIR "${CMAKE_BINARY_DIR}/assets")

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_SOURCE_DIR} ${ASSETS_DEST_DIR}
    COMMENT "Copying assets to the build directory..."
)
add_dependencies(Engine copy_assets)

add_custom_target(copy_lua ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${LUA_LIBRARY}/Lua54.dll ${CMAKE_BINARY_DIR}/Lua54.dll
    COMMENT "Copying LUA_Dll to the build directory..."
)
add_dependencies(Engine copy_lua)

target_compile_definitions(Editor PRIVATE 
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
    $<$<CONFIG:RelWithDebInfo>:RELWITHDEBINFO>
    $<$<CONFIG:MinSizeRel>:MINSIZEREL>
)

include_directories(Engine/src)
include_directories(Engine/vendor)

target_compile_options(Editor PRIVATE "/utf-8")
target_link_libraries(Editor PRIVATE Engine)

message("SOURCE_DIR" ${PROJECT_SOURCE_DIR} " " ${CMAKE_CURRENT_SOURCE_DIR} " " ${CMAKE_SOURCE_DIR} " " ${CMAKE_BINARY_DIR})