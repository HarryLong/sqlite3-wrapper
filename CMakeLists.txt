cmake_minimum_required(VERSION 2.8)

project(sqlite-wrapper)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
add_subdirectory("test")

find_package(sqlite3 REQUIRED)

set(INCLUDE_DIRECTORIES ${sqlite3_INCLUDE_DIRS} )

include_directories(${INCLUDE_DIRECTORIES})

set(LIBS ${LIBS} ${SQLITE3_LIBRARIES})

set(CMAKE_AUTOMOC ON)

SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g -DGL_DEBUG -std=c++11 -Wno-write-strings")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -std=c++11 -Wno-write-strings")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -std=c++11 -Wno-write-strings")

SET(CMAKE_BUILD_TYPE DEBUG)
#SET(CMAKE_BUILD_TYPE RELEASE)
#SET(CMAKE_BUILD_TYPE RELWITHDEBINFO)

set(SRC_FILES sqlite3_types sqlite3_column sqlite3_table sqlite3_db sqlite3_helpers query_data main)

set(API_HEADERS sqlite3_types.h sqlite3_column.h sqlite3_table.h sqlite3_db.h query_data.h)

# LIB
add_executable(sqlite3-wrapper-exec ${SRC_FILES})
target_link_libraries(sqlite3-wrapper-exec ${LIBS})

add_library(sqlite3-wrapper SHARED ${SRC_FILES})
target_link_libraries(sqlite3-wrapper ${LIBS})

#INSTALL LIBRARY
install(TARGETS sqlite3-wrapper
        LIBRARY DESTINATION lib
        CONFIGURATIONS RELWITHDEBINFO)

# INSTALL HEADER FILES
install(FILES ${API_HEADERS}
        DESTINATION include/sqlite3-wrapper)
