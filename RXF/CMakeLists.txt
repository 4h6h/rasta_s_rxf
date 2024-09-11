set(LIB_NAME rxff)

option(STATIC_LIB "build static or shared lib" ON)
message("-- library ${LIB_NAME}: STATIC_LIB=${STATIC_LIB}")
if (${STATIC_LIB})
    add_library(${LIB_NAME} STATIC)
else()
    add_library(${LIB_NAME} SHARED)
endif()

# List sources and include dirs for for the library
target_sources(${LIB_NAME} PRIVATE 
    src/RXF_EntryPoint.c
    src/RXF_Event.c
    src/RXF_EventQueue.c
    src/RXF_HighWaterMarks.c
    src/RXF_MainTask.c
    src/RXF_MemoryManager.c
    src/RXF_Reactive.c
    src/RXF_Startup.c
    src/RXF_Tick.c
    src/RXF_TimerManager.c)

#target_include_directories(${LIB_NAME} PRIVATE .)
