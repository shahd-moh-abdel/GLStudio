cmake_minimum_required(VERSION 3.20)

project(GLStudio)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++14")
set(source_dir "${PROJECT_SOURCE_DIR}/src/")

file(GLOB source_files "${source_dir}/*.cpp")

add_executable(GLStudio
		glad.c
		${source_files}
		)

target_include_directories(GLStudio PRIVATE
				    ${PROJECT_SOURCE_DIR}/include
				    ${GLM_INCLUDE_DIRS})

find_package(glfw3 3.3 REQUIRED)

target_link_libraries(GLStudio PRIVATE glfw)
		
