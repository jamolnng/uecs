file(GLOB_RECURSE TEST_SOURCES
    "*.cpp")

function(src_test src)
    get_filename_component(test_name ${src} NAME)
    string(LENGTH ${test_name} TEST_NAME_LENGTH)
    math(EXPR TEST_NAME_LENGTH "${TEST_NAME_LENGTH}-4")
    string(SUBSTRING ${test_name} 0 ${TEST_NAME_LENGTH} test_name)
    string(REPLACE " " "_" test_name ${test_name})
    add_executable(${test_name} ${src})
    add_test(
        NAME ${test_name}
        COMMAND $<TARGET_FILE:${test_name}>)
    target_link_libraries(${test_name} ${PROJECT_NAME})
    target_include_directories(${test_name} PRIVATE .)
endfunction(src_test)

if(UECS_TEST_ENABLE)
    if (CMAKE_CONFIGURATION_TYPES)
        add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} 
            --force-new-ctest-process --output-on-failure 
            --build-config "$<CONFIGURATION>")
    else()
        add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} 
            --force-new-ctest-process --output-on-failure)
    endif()
    foreach(test_src ${TEST_SOURCES})
      src_test(${test_src})
    endforeach()
endif(UECS_TEST_ENABLE)