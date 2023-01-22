option (BUILD_TEST      OFF)

if (BUILD_TEST)
    enable_testing ()

    find_package (GTest REQUIRED)

    set (TEST_OUTPUT_DIRECTORY  "${CMAKE_CURRENT_SOURCE_DIR}/bin/${OUTPUT_SUB_DIR}/test")
    set (TEST_DATA_DIRECTORY    "${CMAKE_CURRENT_SOURCE_DIR}/test_data")
    
    # Add test case.
    function (add_test_case)
        # Parse arguments.
        set (options            "")
        set (one_value_args     "NAME")
        set (multi_value_args   "LINK_LIBRARIES")
        cmake_parse_arguments (ARG 
            "${options}" 
            "${one_value_args}"
            "${multi_value_args}" 
            ${ARGN} 
        )   

        if (NOT ARG_NAME)
            message (FATAL_ERROR    "Missing argument \"NAME\"")

        endif ()
        
        # Add executable.
        set (target_name  "${ARG_NAME}_test")
        file (GLOB_RECURSE  sources
            "${CMAKE_CURRENT_SOURCE_DIR}/test/source/common/*.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/test/source/common/*.cc"
            "${CMAKE_CURRENT_SOURCE_DIR}/test/source/${ARG_NAME}/*.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/test/source/${ARG_NAME}/*.cc"
        )

        add_executable ("${target_name}"
            ${sources}
        )
        set_target_properties ("${target_name}"
            PROPERTIES  "RUNTIME_OUTPUT_DIRECTORY"  "${TEST_OUTPUT_DIRECTORY}"
        )
        target_include_directories ("${target_name}"    PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
            "${CMAKE_CURRENT_SOURCE_DIR}/test/include"
            "${GTEST_INCLUDE_DIRS}"
        )
        target_link_libraries ("${target_name}"
            ${GTEST_BOTH_LIBRARIES}
        )
        if (ARG_LINK_LIBRARIES)
            target_link_libraries ("${target_name}"
                "${ARG_LINK_LIBRARIES}"
            )

        endif ()

        # Add ctest.
        set (working_dir    "${TEST_DATA_DIRECTORY}/${ARG_NAME}")
        add_test (
            NAME                "${ARG_NAME}"
            COMMAND             "${target_name}"
            WORKING_DIRECTORY   "${working_dir}"
        )

    endfunction (add_test_case)

else ()
    function (add_test_case)
    endfunction (add_test_case)

endif ()
