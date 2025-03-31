file(GLOB_RECURSE SOURCES *.cpp *.h)
add_executable(TestProj ${SOURCES} ${COMMON_FILES})
target_link_libraries(TestProj ${LIBS})

set_target_properties(TestProj PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)