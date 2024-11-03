# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

include_guard(GLOBAL)

find_program(
	SHUFFLY_CLANG_FORMAT_BINARY
	NAMES "clang-format"
	DOC "Clang-format binary"
)

if (SHUFFLY_CLANG_FORMAT_BINARY)
	set(SHUFFLY_CLANG_FORMAT_OPTIONS
		${SHUFFLY_CLANG_FORMAT_BINARY}
		--dry-run
	)

	if (CMAKE_COMPILE_WARNING_AS_ERROR)
		list(APPEND SHUFFLY_CLANG_FORMAT_OPTIONS -Werror)
	endif ()

	file(GLOB_RECURSE SHUFFLY_CLANG_FORMAT_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.hpp ${PROJECT_SOURCE_DIR}/tests/*.cpp ${PROJECT_SOURCE_DIR}/tests/*.hpp)
	list(APPEND SHUFFLY_CLANG_FORMAT_OPTIONS ${SHUFFLY_CLANG_FORMAT_FILES})

	set(SHUFFLY_CLANG_FORMAT_TIMESTAMP_FILE ${CMAKE_CURRENT_BINARY_DIR}/clang_format_timestamp.txt)

	add_custom_command(OUTPUT ${SHUFFLY_CLANG_FORMAT_TIMESTAMP_FILE}
		DEPENDS ${SHUFFLY_CLANG_FORMAT_FILES}
		COMMAND ${SHUFFLY_CLANG_FORMAT_OPTIONS}
		COMMAND ${CMAKE_COMMAND} -E touch ${SHUFFLY_CLANG_FORMAT_TIMESTAMP_FILE}
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		COMMENT "Checking code with clang-format")

	add_custom_target(shuffly_clang_format ALL DEPENDS ${SHUFFLY_CLANG_FORMAT_TIMESTAMP_FILE})

endif ()


