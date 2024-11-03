# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.25)

project(shuffly
	VERSION 0.1.0
	DESCRIPTION "Shuffles MP3 file names, because my MP3 player lacks this feature."
	HOMEPAGE_URL "https://github.com/joseasoler/shuffly"
	LANGUAGES CXX
)

# The project uses C++23 without any non-standard compiler extensions.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set common target compile options and definitions.
set(SHUFFLY_CXX_COMPILE_OPTIONS)
set(SHUFFLY_CXX_COMPILE_DEFINITIONS)
include(cmake/compiler_warnings.cmake)

# Tooling.
include(cmake/clang_format.cmake)
include(cmake/clang_tidy.cmake)

add_executable(shuffly
	src/shuffly.cpp
)

target_compile_definitions(shuffly PRIVATE ${SHUFFLY_CXX_COMPILE_DEFINITIONS})
target_compile_options(shuffly PRIVATE ${SHUFFLY_CXX_COMPILE_OPTIONS})

if (SHUFFLY_CLANG_FORMAT_BINARY)
	add_dependencies(shuffly shuffly_clang_format)
endif ()