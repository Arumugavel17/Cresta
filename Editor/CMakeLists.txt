# Create an executable for ProjectB
add_executable(Editor
    src/EditorLayer.cpp
) 

add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_BINARY_DIR}/Engine/vendor/assimp/bin/assimp-vc143-mtd.dll
    ${CMAKE_BINARY_DIR}/Editor/assimp-vc143-mtd.dll)

add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_BINARY_DIR}/Engine/vendor/glfw/src/glfw3d.dll
    ${CMAKE_BINARY_DIR}/Editor/glfw3d.dll)

add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_BINARY_DIR}/Engine/vendor/glfw/src/glfw3ddll.lib
    ${CMAKE_BINARY_DIR}/Editor/glfw3ddll.lib)

target_link_libraries(Editor PRIVATE Engine)