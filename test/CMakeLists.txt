project(sqlite-wrapper-tests)
# QT Imports
find_package(CppUnit REQUIRED)
find_package(sqlite3 REQUIRED)

set(INCLUDE_DIRECTORIES ${CppUnit_INCLUDE_DIRS} ${sqlite3_INCLUDE_DIRS})
include_directories(${INCLUDE_DIRECTORIES})

set(LIBS ${CPPUNIT_LIBRARIES} ${SQLITE3_LIBRARIES})

SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g -std=c++11")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -std=c++11")
SET(CMAKE_BUILD_TYPE DEBUG)
#SET(CMAKE_BUILD_TYPE RELEASE)
#SET(CMAKE_BUILD_TYPE RELWITHDEBINFO)

set(SRC_FILES ../sqlite3_types ../sqlite3_column ../sqlite3_table ../sqlite3_db ../sqlite3_helpers ../query_data main)

set(TEST_SRC_FILES db_test mock_db mock_table)

add_executable(sqlite-wrapper-tests
${SRC_FILES}
${TEST_SRC_FILES})

target_link_libraries(sqlite-wrapper-tests ${LIBS})

# Use the Widgets module from Qt 5.
