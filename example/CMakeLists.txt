file(GLOB_RECURSE EXAMPLE_SOURCES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "*.cpp"
)

message(STATUS "Found example source files: ${EXAMPLE_SOURCES}")

foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)

    get_filename_component(EXAMPLE_RELATIVE_DIR ${EXAMPLE_SOURCE} DIRECTORY)

    set(EXECUTABLE_NAME "${EXAMPLE_RELATIVE_DIR}-${EXAMPLE_NAME}")

    message(STATUS "Adding example: ${EXECUTABLE_NAME} from ${EXAMPLE_SOURCE}")

    add_executable(${EXECUTABLE_NAME} ${EXAMPLE_SOURCE})

    target_link_libraries(${EXECUTABLE_NAME} PRIVATE CimpleServer)

    set_target_properties(${EXECUTABLE_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

    list(APPEND ALL_EXAMPLES ${EXECUTABLE_NAME})
endforeach()

add_custom_target(all-examples DEPENDS ${ALL_EXAMPLES})
