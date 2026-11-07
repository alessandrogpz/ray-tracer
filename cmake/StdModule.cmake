# StdModule.cmake
#
# Builds the C++23 standard library module (`import std;`) as a regular CMake target, `std_module`.
#
# CMake's built-in support (CMAKE_CXX_MODULE_STD) is still experimental and needs a UUID that
# changes with every CMake release. Instead, this file compiles the standard library's own module
# source, located through the "modules manifest" JSON file each toolchain ships, using only stable
# CMake module support (3.28+). Link `std_module` to any target that wants to `import std;`.
#
# Supported standard libraries:
#   - libstdc++ (GCC 15+, or Clang using libstdc++) -> libstdc++.modules.json
#   - libc++    (Clang 18.1.2+, e.g. macOS or -stdlib=libc++) -> libc++.modules.json
#   - Microsoft STL (MSVC, Visual Studio 2022 17.6+) -> modules.json next to the toolset
#     (Clang on Windows uses the Microsoft STL but cannot build its std module, so it is rejected)
#
# Reference: documentation/references/C++Architecture.md (Build & Compilation Architecture)

include(CheckCXXSourceCompiles)

# Reads a manifest in the standard format ({"modules": [{"source-path", "local-arguments"}]})
# and returns the absolute path of the `std` module source plus any include directories it needs.
function(rt_read_std_manifest manifest out_source out_include_dirs)
    file(READ "${manifest}" json)
    cmake_path(GET manifest PARENT_PATH manifest_dir)

    string(JSON module_count LENGTH "${json}" modules)
    math(EXPR last_module "${module_count} - 1")
    foreach(m RANGE ${last_module})
        string(JSON name GET "${json}" modules ${m} logical-name)
        if(NOT name STREQUAL "std")
            continue()
        endif()

        string(JSON source GET "${json}" modules ${m} source-path)
        cmake_path(ABSOLUTE_PATH source BASE_DIRECTORY "${manifest_dir}" NORMALIZE)

        set(include_dirs "")
        string(JSON include_count ERROR_VARIABLE no_includes
               LENGTH "${json}" modules ${m} local-arguments system-include-directories)
        if(NOT no_includes AND include_count GREATER 0)
            math(EXPR last_include "${include_count} - 1")
            foreach(i RANGE ${last_include})
                string(JSON dir GET "${json}" modules ${m} local-arguments system-include-directories ${i})
                cmake_path(ABSOLUTE_PATH dir BASE_DIRECTORY "${manifest_dir}" NORMALIZE)
                list(APPEND include_dirs "${dir}")
            endforeach()
        endif()

        set(${out_source} "${source}" PARENT_SCOPE)
        set(${out_include_dirs} "${include_dirs}" PARENT_SCOPE)
        return()
    endforeach()

    message(FATAL_ERROR "${manifest} does not describe a `std` module.")
endfunction()

if(MSVC)
    # Visual Studio keeps modules.json in <toolset>/modules, next to bin/ (same search as CMake itself)
    find_file(STD_MODULES_MANIFEST
        NAMES modules.json
        HINTS "$ENV{VCToolsInstallDir}/modules"
        PATHS "${CMAKE_CXX_COMPILER}/../../.." "${CMAKE_CXX_COMPILER}/../.."
        PATH_SUFFIXES ../modules
        NO_CACHE)
    if(NOT STD_MODULES_MANIFEST)
        message(FATAL_ERROR "Could not find modules.json for MSVC; Visual Studio 2022 17.6+ with the C++ modules component is required.")
    endif()

    file(READ "${STD_MODULES_MANIFEST}" msvc_json)
    string(JSON msvc_modules ERROR_VARIABLE msvc_not_standard GET "${msvc_json}" modules)
    if(NOT msvc_not_standard)
        rt_read_std_manifest("${STD_MODULES_MANIFEST}" STD_MODULE_SOURCE STD_MODULE_INCLUDE_DIRS)
    else()
        # Microsoft STL format: {"library": "microsoft/STL", "module-sources": ["std.ixx", ...]}
        cmake_path(GET STD_MODULES_MANIFEST PARENT_PATH msvc_dir)
        string(JSON msvc_source_count LENGTH "${msvc_json}" module-sources)
        math(EXPR msvc_last_source "${msvc_source_count} - 1")
        foreach(i RANGE ${msvc_last_source})
            string(JSON msvc_source GET "${msvc_json}" module-sources ${i})
            if(msvc_source MATCHES "(^|[/\\\\])std\\.ixx$")
                cmake_path(ABSOLUTE_PATH msvc_source BASE_DIRECTORY "${msvc_dir}" NORMALIZE)
                set(STD_MODULE_SOURCE "${msvc_source}")
            endif()
        endforeach()
        set(STD_MODULE_INCLUDE_DIRS "")
        if(NOT STD_MODULE_SOURCE)
            message(FATAL_ERROR "${STD_MODULES_MANIFEST} does not list std.ixx.")
        endif()
    endif()
else()
    # GCC and Clang can use either libstdc++ or libc++; ask the compiler which one is active
    # (macOS uses libc++ by default, Linux usually libstdc++ unless -stdlib=libc++ is passed)
    unset(RT_USING_LIBCXX CACHE)
    check_cxx_source_compiles("
        #include <version>
        #ifndef _LIBCPP_VERSION
        #error not libc++
        #endif
        int main() { return 0; }" RT_USING_LIBCXX)

    if(RT_USING_LIBCXX)
        set(STD_MODULES_MANIFEST_NAME "libc++.modules.json")
    elseif(WIN32)
        # Clang on Windows defaults to the Microsoft STL, whose std module only builds with MSVC
        message(FATAL_ERROR "On Windows, build with MSVC (Visual Studio or the Build Tools); "
                            "Clang cannot build the Microsoft STL's std module.")
    else()
        set(STD_MODULES_MANIFEST_NAME "libstdc++.modules.json")
    endif()

    separate_arguments(rt_cxx_flags UNIX_COMMAND "${CMAKE_CXX_FLAGS}")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} ${rt_cxx_flags} -print-file-name=${STD_MODULES_MANIFEST_NAME}
        OUTPUT_VARIABLE STD_MODULES_MANIFEST
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(NOT IS_ABSOLUTE "${STD_MODULES_MANIFEST}" OR NOT EXISTS "${STD_MODULES_MANIFEST}")
        message(FATAL_ERROR "Could not find ${STD_MODULES_MANIFEST_NAME}; this toolchain does not ship a std module "
                            "(GCC 15+ or Clang 18.1.2+ with libc++ is required).")
    endif()

    rt_read_std_manifest("${STD_MODULES_MANIFEST}" STD_MODULE_SOURCE STD_MODULE_INCLUDE_DIRS)
endif()

message(STATUS "Standard library module: ${STD_MODULE_SOURCE}")

cmake_path(GET STD_MODULE_SOURCE PARENT_PATH STD_MODULE_SOURCE_DIR)
add_library(std_module STATIC)
target_sources(std_module
  PUBLIC
    FILE_SET CXX_MODULES BASE_DIRS ${STD_MODULE_SOURCE_DIR} FILES ${STD_MODULE_SOURCE}
)
# `import std;` is a C++23 feature; this also propagates to every target linking std_module
target_compile_features(std_module PUBLIC cxx_std_23)
if(STD_MODULE_INCLUDE_DIRS)
    target_include_directories(std_module SYSTEM PRIVATE ${STD_MODULE_INCLUDE_DIRS})
endif()

# Clang warns that the library's own `export module std;` uses a reserved name; harmless, but
# -Werror makes it fatal. PUBLIC because Clang rebuilds the std module with each importer's flags.
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(std_module PUBLIC -Wno-reserved-module-identifier)
endif()
