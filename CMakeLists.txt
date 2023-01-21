cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set (CMAKE_CXX_STANDARD 20) 

cmake_policy(SET CMP0054 NEW)

# Project
project (RemotePortMapper)

list (APPEND    CMAKE_MODULE_PATH   "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include (env)
include (test)

# Targets.
# Common.
file (GLOB_RECURSE COMMON_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/source/common/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/common/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/common/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/common/*.C"
    )

add_library (RemotePortMapperCommon STATIC
    ${COMMON_SOURCES}
)

# Router.
file (GLOB_RECURSE ROUTER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/source/router/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/router/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/router/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/router/*.C"
    )

add_executable (RemotePortMapperRouter 
    ${ROUTER_SOURCES}
)

target_link_libraries (
    RemotePortMapperRouter
    RemotePortMapperCommon
)

# Node common.
file (GLOB_RECURSE NODE_COMMON_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/source/node_common/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/node_common/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/node_common/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/node_common/*.C"
    )

add_library (RemotePortMapperNodeCommon STATIC
    ${NODE_COMMON_SOURCES}
)


# Node.
file (GLOB_RECURSE NODE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/source/node/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/node/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/node/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/node/*.C"
    )

add_executable (RemotePortMapperNode
    ${NODE_SOURCES}
)

target_link_libraries (
    RemotePortMapperNode
    RemotePortMapperNodeCommon
    RemotePortMapperCommon
)

# Node GUI.
file (GLOB_RECURSE NODE_GUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/source/node_gui/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/node_gui/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/node_gui/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/node_gui/*.C"
    )

add_executable (RemotePortMapperNodeGUI
    ${NODE_GUI_SOURCES}
)

target_link_libraries (
    RemotePortMapperNodeGUI
    RemotePortMapperNodeCommon
    RemotePortMapperCommon
)

# Tests
add_test_case (
    NAME            "result"
    LINK_LIBRARIES  "RemotePortMapperCommon"
)
