
cmake_minimum_required(VERSION 3.9)
project(VolsungWebFrontend)

include_directories("include")
include_directories("Volsung/include")
set(CMAKE_CXX_STANDARD 17)

file(GLOB code 
    "src/*.cc"
)

add_subdirectory(Volsung Volsung/build)
				 
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wfatal-errors -O3")
add_executable( WebVolsung
                ${code} )

target_include_directories( WebVolsung PUBLIC
                            Volsung/include )

target_link_libraries( WebVolsung
                       Volsung/lib/Volsung )
                       

