find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy-7 clang-tidy)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE)

# Add target to lint the given source files
function(add_clang_tidy_target TARGET)
    set(FILES ${ARGN})

    if(NOT FILES)
        add_custom_target(${TARGET})
        return()
    endif()

    # Remove duplicates & sort
    list(REMOVE_DUPLICATES FILES)
    list(SORT FILES)

    # Add target
    add_custom_target(${TARGET}
        COMMAND ${CMAKE_COMMAND} -E chdir
                ${CMAKE_CURRENT_SOURCE_DIR}
                ${CLANG_TIDY_EXECUTABLE}
                "-quiet"
                "-p=${CMAKE_CURRENT_BINARY_DIR}"
                ${FILES}
        COMMENT "Running ${TARGET}"
        VERBATIM)
endfunction()
