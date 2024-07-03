cmake_minimum_required(VERSION 3.29)

include(CheckCXXCompilerFlag)

function(enable_cxx_compiler_flag_if_supported flag)
    message(--\ Trying\ to\ enable\ compiler\ flag:\ "${flag}")
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if(flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if(flag_supported)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
        endif()
        unset(flag_supported CACHE)
    endif()
endfunction()

set(PROJECT_NAME NixyPlayer)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND MINGW32)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -gdwarf-2")
endif()

enable_cxx_compiler_flag_if_supported("-Wall")
enable_cxx_compiler_flag_if_supported("-Wextra")
enable_cxx_compiler_flag_if_supported("-pedantic")
enable_cxx_compiler_flag_if_supported("-pedantic-errors")
enable_cxx_compiler_flag_if_supported("-Werror")
enable_cxx_compiler_flag_if_supported("-Wconversion")
enable_cxx_compiler_flag_if_supported("-Wunused")
enable_cxx_compiler_flag_if_supported("-Wshadow")
enable_cxx_compiler_flag_if_supported("-Wunused-parameter")
enable_cxx_compiler_flag_if_supported("-Wunreachable-code")
enable_cxx_compiler_flag_if_supported("-Wfloat-equal")
enable_cxx_compiler_flag_if_supported("-Wformat=2")

include_directories(. ../shared/thirdparty ../shared/include)

file(GLOB_RECURSE SOURCE_FILES 
    "*.cpp" 
    "../shared/thirdparty/*.cpp"
)

if(MINGW32)
SET(CMAKE_SYSTEM_NAME Windows)
SET(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
SET(CMAKE_RC_COMPILER i686-w64-mingw32-windres)
SET(CMAKE_RANLIB i686-w64-mingw32-ranlib)
else()
set(CMAKE_ENABLE_EXPORTS ON)
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if (MINGW32)
target_link_libraries(${PROJECT_NAME} 
#-static 
#-static-libgcc 
#-static-libstdc++ 
)
else()
target_link_libraries(${PROJECT_NAME})
endif()