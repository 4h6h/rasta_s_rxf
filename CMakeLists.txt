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
else()
    target_sources(${PROJECT_NAME} PRIVATE 
        src/RastaS_Lcfg_client.c )
endif(USE_CFG_SERVER)

if(USE_TEST_CMOCKA)
    # cmocka test framework
    add_library(cmocka SHARED IMPORTED)
    set_target_properties(cmocka PROPERTIES
        IMPORTED_LOCATION "/usr/local/lib/libcmocka.dylib"
        INTERFACE_INCLUDE_DIRECTORIES "/usr/local/include"    
    )

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
