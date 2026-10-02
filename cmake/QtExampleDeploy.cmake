# Copies the Qt runtime next to the executable so it can be started directly from the build directory.
function(qt_example_deploy target)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "QML_DIR" "")

    # One output directory per example keeps parallel deployments from writing into the same folder.
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

    if(NOT WIN32)
        return()
    endif()

    set(qml_args "")
    if(ARG_QML_DIR)
        set(qml_args --qmldir "${ARG_QML_DIR}")
    endif()

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND $<TARGET_FILE:Qt6::windeployqt>
                $<IF:$<CONFIG:Debug>,--debug,--release>
                --no-translations --no-compiler-runtime --no-system-d3d-compiler --no-opengl-sw
                ${qml_args}
                $<TARGET_FILE:${target}>
        COMMENT "Deploying Qt runtime for ${target}"
        VERBATIM)
endfunction()
