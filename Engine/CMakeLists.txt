# Create a static library for ProjectA
add_library(ProjectA STATIC
    src/App.cpp
)

# Include directories for ProjectA
target_include_directories(ProjectA PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Optional: Add compile options or definitions for ProjectA
target_compile_options(ProjectA PRIVATE /W4)