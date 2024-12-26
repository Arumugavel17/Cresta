# Create an executable for ProjectB
add_executable(Editor
    "src/EditorApplication.cpp"
) 

add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_BINARY_DIR}/Engine/assimp-vc143-mtd.lib
    ${CMAKE_BINARY_DIR}/Editor/assimp-vc143-mtd.lib)

add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_BINARY_DIR}/Engine/vendor/glfw/src/glfw3d.lib
    ${CMAKE_BINARY_DIR}/Editor/glfw3d.lib)

add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_BINARY_DIR}/Engine/Engine.lib
    ${CMAKE_BINARY_DIR}/Editor/Engine.lib)

add_compile_options(/W3 /utf-8 /wd4267)

target_compile_options(Editor PRIVATE "/utf-8")

target_link_libraries(Editor PRIVATE Engine)