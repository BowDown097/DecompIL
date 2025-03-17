function(DotnetPublish target project)
    set(DOTNET_COMMAND "dotnet" CACHE STRING "Path to dotnet command")

    # arch for RID
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
        set(rid_arch "arm64")
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7")
        set(rid_arch "arm")
    else()
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(rid_arch "x64")
        elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
            set(rid_arch "x86")
        endif()
    endif()

    # platform for RID
    if(WIN32)
        set(rid_platform "win")
    elseif(LINUX)
        set(rid_platform "linux")
    elseif(IOS)
        set(rid_platform "ios")
    elseif(APPLE)
        set(rid_platform "osx")
    elseif(ANDROID)
        set(rid_platform "android")
    endif()

    # check if we have an arch and platform for RID
    if(NOT DEFINED rid_arch OR NOT DEFINED rid_platform)
        message(FATAL_ERROR "Unsupported configuration for building C# project. Cannot continue.")
    endif()

    # build config
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(build_config "Debug")
    else()
        set(build_config "Release")
    endif()

    # we're allowing for an optional comment, so get extra args and use the first extra arg if it's there
    set(extra_args ${ARGN})
    list(LENGTH extra_args extra_count)

    if(${extra_count} GREATER 0)
        list(GET extra_args 0 command_comment)
    endif()

    if(CMAKE_PROJECT_VERSION_TWEAK)
        set(assembly_version "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}")
    elseif(CMAKE_PROJECT_VERSION_PATCH)
        set(assembly_version "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.0")
    else()
        set(assembly_version "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.0.0")
    endif()

    if(${extra_count} GREATER 1)
        list(GET extra_args 1 out_dir)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMENT ${command_comment}
            COMMAND ${DOTNET_COMMAND} publish ${project} -c ${build_config} -r ${rid_platform}-${rid_arch} -o ${out_dir} -p:Version=${assembly_version}
            VERBATIM)
    else()
        add_custom_command(TARGET ${target} POST_BUILD
            COMMENT ${command_comment}
            COMMAND ${DOTNET_COMMAND} publish ${project} -c ${build_config} -r ${rid_platform}-${rid_arch} -o $<TARGET_FILE_DIR:${target}> -p:Version=${assembly_version}
            VERBATIM)
    endif()
endfunction()
