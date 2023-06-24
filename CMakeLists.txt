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
    ${DEPENDENCE_LIBS}
)

# Server.
file (GLOB_RECURSE SERVER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/source/server/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/server/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/server/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/server/*.C"
    )

add_executable (RemotePortMapperServer 
    ${SERVER_SOURCES}
)

target_link_libraries (
    RemotePortMapperServer
    RemotePortMapperCommon
    ${DEPENDENCE_LIBS}
)

# Client.
file (GLOB_RECURSE CLIENT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/source/client/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/client/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/client/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/client/*.C"
    )

add_executable (RemotePortMapperClient 
    ${CLIENT_SOURCES}
)

target_link_libraries (
    RemotePortMapperClient
    RemotePortMapperCommon
    ${DEPENDENCE_LIBS}
)

# Client GUI.
file (GLOB_RECURSE CLIENT_GUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/source/client_gui/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/client_gui/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/client_gui/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/client_gui/*.C"
    )

add_executable (RemotePortMapperClientGUI 
    ${CLIENT_GUI_SOURCES}
)

target_link_libraries (
    RemotePortMapperClientGUI
    RemotePortMapperCommon
    ${DEPENDENCE_LIBS}
)

# Tests
add_test_case (
    NAME            "type_trait"
    LINK_LIBRARIES  "RemotePortMapperCommon"
                    ${DEPENDENCE_LIBS}
)
add_test_case (
    NAME            "functional"
    LINK_LIBRARIES  "RemotePortMapperCommon"
                    ${DEPENDENCE_LIBS}
)
add_test_case (
    NAME            "result"
    LINK_LIBRARIES  "RemotePortMapperCommon"
                    ${DEPENDENCE_LIBS}
)
add_test_case (
    NAME            "thread_pool"
    LINK_LIBRARIES  "RemotePortMapperCommon"
                    ${DEPENDENCE_LIBS}
)
add_test_case (
    NAME            "socket_address"
    LINK_LIBRARIES  "RemotePortMapperCommon"
                    ${DEPENDENCE_LIBS}
)
add_test_case (
    NAME            "nargs"
    LINK_LIBRARIES  "RemotePortMapperCommon"
                    ${DEPENDENCE_LIBS}
)
add_test_case (
    NAME            "flags"
    LINK_LIBRARIES  "RemotePortMapperCommon"
                    ${DEPENDENCE_LIBS}
)
