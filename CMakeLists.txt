cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
cmake_policy(VERSION 3.2)

set(MAX_ENTITY_COMPONENTS 64)
set(MAX_SYSTEMS 64)
set(ID_TYPE uint64_t)
set(UECS_VERSION 0.1.0)
project(uecs VERSION ${UECS_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(UECS_TEST_ENABLE "Build unit tests" ON)
option(UECS_BUILD_EXAMPLES "Build examples" ON)
if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -pedantic-errors -Wall -Wextra \
    -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 \
    -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept \
    -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow \
    -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 \
    -Wswitch-default -Wundef -Werror -Wno-unused -Winline -Wconversion -Wfloat-equal\
    -Weffc++")
endif(UNIX)

enable_testing()

file(GLOB_RECURSE SOURCES
    "src/*.cpp")
file(GLOB_RECURSE PUBLIC_HEADERS
    "include/*.hpp"
    "include/*.h")
file(GLOB_RECURSE PRIVATE_HEADERS
    "src/*.hpp"
    "src/*.h")
file(GLOB_RECURSE INPUT_HEADERS
    "include/*.hpp.in"
    "include/*.h.in")

foreach(INPUT_HEADER ${INPUT_HEADERS})
    string(LENGTH ${INPUT_HEADER} INPUT_HEADER_LENGTH)
    math(EXPR INPUT_HEADER_LENGTH "${INPUT_HEADER_LENGTH}-3")
    string(SUBSTRING ${INPUT_HEADER} 0 ${INPUT_HEADER_LENGTH} OUTPUT_HEADER)
    list(APPEND PUBLIC_HEADERS ${OUTPUT_HEADER})
    configure_file(${INPUT_HEADER} ${OUTPUT_HEADER})
endforeach(INPUT_HEADER ${INPUT_HEADERS} @ONLY)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${PUBLIC_HEADERS} ${PRIVATE_HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME} PUBLIC include)

function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

assign_source_group(${SOURCES})
assign_source_group(${PUBLIC_HEADERS})
assign_source_group(${PRIVATE_HEADERS})
assign_source_group(${SHADERS})

add_subdirectory(examples)
add_subdirectory(tests)