
function(copy_file_post_build target directory)
    if(NOT EXISTS ${directory})
        file(MAKE_DIRECTORY ${directory})
    endif()
    foreach(arg IN LISTS ARGN)
        add_custom_command(
            TARGET ${target}
            POST_BUILD
            COMMAND
            ${CMAKE_COMMAND} -E copy ${arg} ${directory}
        )
    endforeach()
endfunction()