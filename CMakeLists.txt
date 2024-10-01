cmake_minimum_required(VERSION 3.10)
option(USE_CFG_SERVER "Use RastaS_Lcfg_client else RastaS_Lcfg_server." OFF) # OFF by default
option(USE_TEST_CMOCKA "Use the code within the test framework cmocka; cmocka needs to be installed." ON) # ON by default 

# Project name
if(USE_CFG_SERVER)
    project(isc_server C)
else()
    project(isc_client C)
endif(USE_CFG_SERVER)


# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Disable in-source builds to prevent source tree corruption
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR '
        FATAL: In-source builds are not allowed.
            You should create a separate directory for build files.
    ')
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# Create an executable
add_executable(${PROJECT_NAME}
    src/RXF_SM_Event.c
    src/md4.c
    src/pdu.c
    src/sm.c
    src/iFec.c
    src/cFec.c
    src/iDisp.c
    src/cDisp.c
    src/iRastaS.c
    src/cRastaS.c
    src/cRastaS_impl.c
    src/log.c 
    src/SandboxOperation.c )

if(USE_CFG_SERVER)
    target_sources(${PROJECT_NAME} PRIVATE 
        src/RastaS_Lcfg_server.c )
    add_compile_definitions(PEER_1_IP=\"172.32.0.3\")
    add_compile_definitions(PEER_1_HASH=8243780227304282481)
else()
    target_sources(${PROJECT_NAME} PRIVATE 
        src/RastaS_Lcfg_client.c )
    add_compile_definitions(PEER_1_IP=\"172.32.0.4\")
    add_compile_definitions(PEER_1_HASH=8243780227304282482)
endif(USE_CFG_SERVER)

add_compile_definitions(TIMER_HASH=8243780227304282480)
add_compile_definitions(PEER_2_HASH=8243780227304282483)
add_compile_definitions(PEER_3_HASH=8243780227304282484)
add_compile_definitions(EXIT_HASH=8243780227304282485)

if(USE_TEST_CMOCKA)
    # cmocka test framework
    add_library(cmocka SHARED IMPORTED)
    if (APPLE)
        set_target_properties(cmocka PROPERTIES
            IMPORTED_LOCATION "/usr/local/lib/libcmocka.dylib"
            INTERFACE_INCLUDE_DIRECTORIES "/usr/local/include"
        )
    endif(APPLE)
    if(UNIX)
        set_target_properties(cmocka PROPERTIES
            IMPORTED_LOCATION "/usr/lib/x86_64-linux-gnu/libcmocka.so"
            INTERFACE_INCLUDE_DIRECTORIES "/usr/include"
            INTERFACE_INCLUDE_DIRECTORIES "/usr/include/cmockery"
        )
    endif(UNIX)

    target_sources(${PROJECT_NAME} PRIVATE
        src/test_sm_connection.c 
        src/test.c )
    target_link_libraries(${PROJECT_NAME} rxff rxff_gen cmocka)

else()
    # just main
    target_sources(${PROJECT_NAME} PRIVATE
        src/main.c )

    target_link_libraries(${PROJECT_NAME} rxff rxff_gen)
    
endif(USE_TEST_CMOCKA)
unset(USE_TEST_CMOCKA CACHE)

# Include directories
include_directories(
    RXFGeneratedModel/src
    RXF/src
    src)

# Subdirectories
add_subdirectory(RXF)
add_subdirectory(RXFGeneratedModel)
