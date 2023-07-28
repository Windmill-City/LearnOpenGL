cmake_minimum_required(VERSION 3.4)
project(LearnOpenGL)

# ####################################
# Compile
# ####################################
set(CMAKE_CXX_STANDARD 20)

# ####################################
# Dependencies
# ####################################
add_subdirectory(glad EXCLUDE_FROM_ALL)
add_subdirectory(glfw EXCLUDE_FROM_ALL)

# ####################################
# Source Files
# ####################################
aux_source_directory(src SRC_LIST)

# ####################################
# Target
# ####################################
add_executable(LearnOpenGL ${SRC_LIST})
target_include_directories(LearnOpenGL PRIVATE include)
target_link_libraries(LearnOpenGL PRIVATE glfw glad)

# Copy file assets
add_custom_command(TARGET LearnOpenGL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets $<TARGET_FILE_DIR:LearnOpenGL>/assets
)

# ####################################
# Embed Assets Build Tools
# ####################################
add_executable(ResourceBuilder ResourceBuilder.cpp)
target_include_directories(ResourceBuilder PRIVATE include)

# Build embed assets
if(MSVC)
    add_executable(MSVC_bin2obj MSVC_bin2obj.cpp)
    target_include_directories(MSVC_bin2obj PRIVATE include)

    add_custom_target(BuildEmbed
        COMMAND $<TARGET_FILE:ResourceBuilder> -e ${CMAKE_CURRENT_SOURCE_DIR}/embed -i indexAssets -b blockAssets
        COMMAND $<TARGET_FILE:MSVC_bin2obj> -b indexAssets -o indexAssets.obj -s _embed_indexAssets
        COMMAND $<TARGET_FILE:MSVC_bin2obj> -b blockAssets -o blockAssets.obj -s _embed_blockAssets
    )
    add_dependencies(BuildEmbed ResourceBuilder MSVC_bin2obj)
else(MSVC)
    add_custom_target(BuildEmbed
        COMMAND $<TARGET_FILE:ResourceBuilder> -e ${CMAKE_CURRENT_SOURCE_DIR}/embed -i indexAssets -b blockAssets

        # objcopy .... todo
    )
    add_dependencies(BuildEmbed ResourceBuilder)
endif(MSVC)

target_link_libraries(LearnOpenGL PRIVATE ${CMAKE_BINARY_DIR}/indexAssets ${CMAKE_BINARY_DIR}/blockAssets)
add_dependencies(LearnOpenGL BuildEmbed)
