cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project (file_scanner VERSION ${PROJECT_VERSION})
option(WITH_BOOST_TEST "Whether to build Boost test" ON)

enable_language(C)
enable_language(CXX)

set(Boost_USE_RELEASE_LIBS ON)

set(Boost_USE_MULTITHREAD ON)

set(BOOST_ROOT "/usr")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")

find_package(Boost 1.5.8 REQUIRED COMPONENTS program_options system filesystem)
include_directories(${Boost_INCLUDE_DIRS})

add_executable (file_scanner
	main.cpp
    fileScanner/FileScanner.cpp 
    utils/utils.cpp 
)

target_link_libraries (file_scanner 
	${Boost_LIBRARIES}
)

set_target_properties(file_scanner PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(file_scanner
    PRIVATE "${CMAKE_BINARY_DIR}"
)

if (MSVC)
    target_compile_options(file_scanner PRIVATE
        /W4
    )
    target_compile_options(file_scanner PRIVATE
        /W4
    )
else ()
    target_compile_options(file_scanner PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS file_scanner RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)


