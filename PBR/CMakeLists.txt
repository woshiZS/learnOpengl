file(GLOB_RECURSE SOURCES *.cpp *.h)
add_executable(PBR ${SOURCES} ${COMMON_FILES})
target_link_libraries(PBR ${LIBS})

set_target_properties(PBR PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)