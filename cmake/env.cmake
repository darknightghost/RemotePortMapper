# Build type
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Debug")

endif ()

set (CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}"  CACHE STRING "${CMAKE_BUILD_TYPE}" FORCE)

if (GENERATOR_IS_MULTI_CONFIG OR CMAKE_CONFIGURATION_TYPES)
    set (CMAKE_CONFIGURATION_TYPES "${CMAKE_BUILD_TYPE}" CACHE STRING "${CMAKE_BUILD_TYPE}" FORCE)

endif()

message (STATUS "Build Type - ${CMAKE_BUILD_TYPE}.")
message (STATUS "Host System - ${CMAKE_HOST_SYSTEM_NAME}.")
message (STATUS "Host Processor - ${CMAKE_HOST_SYSTEM_PROCESSOR}.")
message (STATUS "Target System - ${CMAKE_SYSTEM_NAME}.")
message (STATUS "Target Processor - ${CMAKE_SYSTEM_PROCESSOR}.")

# Environment.
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include")
add_definitions ("-DQIODEVICE_BUFFERSIZE=Q_INT64_C(64 * 1024 * 1024)")

if (WIN32)
    include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/windows")
    add_definitions ("-DOS_WINDOWS=1")

elseif (UNIX)
    add_definitions ("-DOS_LINUX=1")

endif ()

# Output path.
if (UNIX)
    execute_process (
        COMMAND             lsb_release -ir
        RESULT_VARIABLE     lsb_result
        OUTPUT_VARIABLE     lsb_output)

    if (lsb_result EQUAL 0)
        string (REGEX REPLACE   ".*Distributor[ \t]+ID:[ \t]+([^\n]+).*"
            "\\1"
            DISTRO_NAME
            "${lsb_output}")
        string (REGEX REPLACE   ".*Release:[ \t]+([^\n]+).*"
            "\\1"
            DISTRO_RELEASE
            "${lsb_output}")
        if (${DISTRO_RELEASE} STREQUAL "rolling")
            set (HOST_DISTRO        "${DISTRO_NAME}" CACHE STRING "Host distro." FORCE)

        else ()
            set (HOST_DISTRO        "${DISTRO_NAME} ${DISTRO_RELEASE}" CACHE STRING "Host distro." FORCE)

        endif ()

    else ()
        set (HOST_DISTRO        "Unknow" CACHE STRING "Host distro." FORCE)

    endif ()

    unset (lsb_result)
    unset (lsb_output)

    if (NOT TARGET_DISTRO)
        set (TARGET_DISTRO      "${HOST_DISTRO}" CACHE STRING "Target distro." FORCE)

    endif ()

    set (OUTPUT_SUB_DIR "${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/${TARGET_DISTRO}/${CMAKE_SYSTEM_PROCESSOR}")

    message (STATUS "Host Distro - ${HOST_DISTRO}.")
    message (STATUS "Target Distro - ${TARGET_DISTRO}.")

else ()
    set (OUTPUT_SUB_DIR "${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}")

endif ()

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "${CMAKE_CURRENT_SOURCE_DIR}/lib/${OUTPUT_SUB_DIR}")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_CURRENT_SOURCE_DIR}/lib/${OUTPUT_SUB_DIR}")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_CURRENT_SOURCE_DIR}/bin/${OUTPUT_SUB_DIR}")

# Platform options.
# Debug/Release.
# Enable address sanitizer in debug mode if possible.
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"
            OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang"
            OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
        # Enable address sanitizer.
        add_compile_options (-fno-omit-frame-pointer -fsanitize=address)
        add_link_options (-fno-omit-frame-pointer -fsanitize=address)
        message (STATUS "Address sanitizer is enabled.")

    endif()

endif ()

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_definitions ("-DQT_NO_DEBUG=1")

endif ()

# Compiler.
if (MSVC)
    # Set compile options.
    if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64")
        set (CMAKE_C_FLAGS_DEBUG "/DAMD64 /DDEBUG /RTC1 /Od ${C_CXX_FLAGS}")
        set (CMAKE_C_FLAGS_RELEASE "/DAMD64 /DNDEBUG /O2 ${C_CXX_FLAGS}")

    elseif (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86")
        set (CMAKE_C_FLAGS_DEBUG "/DDEBUG /RTC1 /Od ${C_CXX_FLAGS} /arch:IA32")
        set (CMAKE_C_FLAGS_RELEASE "/DNDEBUG /O2 ${C_CXX_FLAGS} /arch:IA32")

    else ()
        message (FATAL_ERROR "Unsupported Platform: ${CMAKE_SYSTEM_PROCESSOR}")

    endif ()

    # Output directory.
    set (MSVC_CONFIG_TYPE ${CMAKE_BUILD_TYPE})
    string (TOUPPER ${CMAKE_BUILD_TYPE} OUTPUTCONFIG)
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_CURRENT_SOURCE_DIR}/bin/${OUTPUT_SUB_DIR}")
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_CURRENT_SOURCE_DIR}/lib/${OUTPUT_SUB_DIR}")
    set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_CURRENT_SOURCE_DIR}/lib/${OUTPUT_SUB_DIR}")

    # Ignore warnings of functions like strcpy(), strcmp()...
    add_compile_options ("-D_CRT_SECURE_NO_WARNINGS")
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        add_link_options ("/subsystem:console" "/entry:mainCRTStartup")

    elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        add_link_options ("/subsystem:windows" "/entry:mainCRTStartup")

    endif ()

    if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG /OPT:REF /OPT:ICF")

    endif ()
else ()
    if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g")

    endif ()

endif ()

# Dependencies
set (DEPENDENCE_LIBS        "")

find_package (PythonInterp REQUIRED 3)

find_package (OpenSSL REQUIRED COMPONENTS SSL Crypto)
include_directories ("${OPENSSL_INCLUDE_DIR}")
if ("${OPENSSL_VERSION}" VERSION_LESS "1.1.1")
    message (FATAL_ERROR "The version of OpenSSL must be greater than 1.1.1.")

endif ()

find_package (Qt5Core REQUIRED)
find_package (Qt5Widgets REQUIRED)
find_package (Qt5Network REQUIRED)

if ("${Qt5Core_VERSION}" VERSION_LESS "5.14.1")
    message (FATAL_ERROR "The version of Qt5 must be greater than 5.14.1.")

endif ()

if (WIN32)
    list (APPEND    DEPENDENCE_LIBS
        "ws2_32"
    )

elseif (UNIX)
    list (APPEND    DEPENDENCE_LIBS
        "pthread"
        "anl"
    )

endif ()
