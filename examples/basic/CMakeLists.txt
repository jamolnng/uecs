file(GLOB_RECURSE SOURCES
    "src/*.cpp")
file(GLOB_RECURSE PUBLIC_HEADERS
    "include/*.hpp"
    "include/*.h")
file(GLOB_RECURSE PRIVATE_HEADERS
    "src/*.hpp"
    "src/*.h")
file(GLOB_RECURSE SHADERS
    "src/*.vert"
    "src/*.frag"
    "src/*.tesc")

add_executable(basic ${SOURCES} ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SHADERS})
target_include_directories(basic PRIVATE src)
target_include_directories(basic PUBLIC include)
target_link_libraries(basic ${PROJECT_NAME})

assign_source_group(${SOURCES})
assign_source_group(${PUBLIC_HEADERS})
assign_source_group(${PRIVATE_HEADERS})
assign_source_group(${SHADERS})