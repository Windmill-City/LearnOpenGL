cmake_minimum_required(VERSION 3.4)
project(LearnOpenGL)

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