macro(msvc_dirtctory_fix Target libPath runtimePath)
        if(MSVC)
                set_target_properties(${Target} PROPERTIES 
                RUNTIME_OUTPUT_DIRECTORY            ${runtimePath}
                RUNTIME_OUTPUT_DIRECTORY_DEBUG      ${runtimePath}/Debug
                LIBRARY_OUTPUT_DIRECTORY            ${libPath}
                LIBRARY_OUTPUT_DIRECTORY_DEBUG      ${libPath}/Debug
                ARCHIVE_OUTPUT_DIRECTORY            ${libPath}
                ARCHIVE_OUTPUT_DIRECTORY_DEBUG      ${libPath}/Debug)
        endif()
endmacro()
