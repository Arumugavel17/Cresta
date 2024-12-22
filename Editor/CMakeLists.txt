# Create an executable for ProjectB
add_executable(Editor
    src/Main.cpp
) 

# Link ProjectA's static library to ProjectB
target_link_libraries(Editor PRIVATE ProjectA)

# Include directories from ProjectA (inherited automatically if PUBLIC)
