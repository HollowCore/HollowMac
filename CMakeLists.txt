cmake_minimum_required (VERSION 3.5)
project (HollowMac)

# Add dependency directories
add_subdirectory(HollowCore)

# Configure Environment
if (COVERAGE)
    find_program(GCOV_PATH gcov)
    if(NOT GCOV_PATH)
        message(FATAL_ERROR "gcov not found! Aborting...")
    endif()

    find_program(LCOV_PATH  NAMES lcov lcov.bat lcov.exe lcov.perl)
    if(NOT LCOV_PATH)
        message(FATAL_ERROR "lcov not found! Aborting...")
    endif()

    find_program(GENHTML_PATH NAMES genhtml genhtml.perl genhtml.bat)
    if(NOT GENHTML_PATH)
        message(FATAL_ERROR "genhtml not found! Aborting...")
    endif()

    set(CMAKE_C_FLAGS "-g -O0 -Wall -W -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
    set(CMAKE_LD_FLAGS "-fprofile-arcs -ftest-coverage")
    set(CMAKE_C_OUTPUT_EXTENSION_REPLACE ON)
    set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
endif ()

# HollowMac Library
set(HOLLOWMAC_LIBRARY_NAME hollowmac)
set(SOURCES ${SOURCES} Source/UI/HCApplication.c)
set(SOURCES ${SOURCES} Source/UI/HCMenu.c)
set(SOURCES ${SOURCES} Source/UI/HCWindow.c)
set(SOURCES ${SOURCES} Source/UI/HCView.c)
set(SOURCES ${SOURCES} Source/UI/HCButton.c)
set(SOURCES ${SOURCES} Source/UI/HCRasterView.c)
add_library(${HOLLOWMAC_LIBRARY_NAME} ${SOURCES})

# HollowMac Links
target_link_libraries(${HOLLOWMAC_LIBRARY_NAME} m pthread "-framework AppKit")
add_dependencies(${HOLLOWMAC_LIBRARY_NAME} hollowcore)
target_link_libraries(${HOLLOWMAC_LIBRARY_NAME} hollowcore)

# HollowMac Test Suite
set(HOLLOWMAC_TEST_NAME hollowmactest)
set(TEST_SOURCES ${TEST_SOURCES} Test/main.c)
set(TEST_SOURCES ${TEST_SOURCES} Test/HCApplication.c)
set(TEST_SOURCES ${TEST_SOURCES} Test/HCMenu.c)
set(TEST_SOURCES ${TEST_SOURCES} Test/HCWindow.c)
set(TEST_SOURCES ${TEST_SOURCES} Test/HCView.c)
set(TEST_SOURCES ${TEST_SOURCES} Test/HCButton.c)
set(TEST_SOURCES ${TEST_SOURCES} Test/HCRasterView.c)
add_executable(${HOLLOWMAC_TEST_NAME} ${TEST_SOURCES})
add_dependencies(${HOLLOWMAC_TEST_NAME} hollowcore)
target_link_libraries(${HOLLOWMAC_TEST_NAME} hollowmac)
enable_testing()
add_test(tests ${HOLLOWMAC_TEST_NAME})

# HollowMac Test Suite Code Coverage
if (COVERAGE)
    set(HOLLOWMAC_COVERAGE hollowmaccoverage)
    set(OBJECT_DIR ${CMAKE_BINARY_DIR}/CMakeFiles/${HOLLOWMAC_LIBRARY_NAME}.dir/Source)
    add_custom_target(${HOLLOWMAC_COVERAGE}
        COMMAND mkdir -p ${HOLLOWMAC_COVERAGE}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    add_custom_command(TARGET ${HOLLOWMAC_COVERAGE}
        COMMAND echo "-- LCOV running tests..."
        COMMAND ${CMAKE_BINARY_DIR}/${HOLLOWMAC_TEST_NAME}
        COMMAND echo "-- LCOV tests complete"
        COMMAND echo "-- LCOV generating coverage..."
        COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} -d ${OBJECT_DIR} -c -o coverage.info
        COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} -a coverage.info -o coverage.total
        COMMAND ${GENHTML_PATH} coverage.total --output-directory html
        COMMAND echo "-- LCOV coverage generation complete"
        COMMAND echo "-- GCOV analysis"
        COMMAND ${GCOV_PATH} ${SOURCES} -o ${OBJECT_DIR} > summary.txt
        COMMAND cat summary.txt
        COMMAND echo "-- Coverage files have been output to ${CMAKE_BINARY_DIR}/${HOLLOWMAC_COVERAGE}"
        COMMAND open ${CMAKE_BINARY_DIR}/${HOLLOWMAC_COVERAGE}/html/index.html
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${HOLLOWMAC_COVERAGE}
    )
    add_custom_target(cumulative_${HOLLOWMAC_COVERAGE}
        COMMAND mkdir -p ${HOLLOWMAC_COVERAGE}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    add_custom_command(TARGET cumulative_${HOLLOWMAC_COVERAGE}
        COMMAND echo "-- LCOV establishing baseline..."
        COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} -d ${OBJECT_DIR} -z
        COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} -d ${OBJECT_DIR} -c -i -o coverage.base
        COMMAND echo "-- LCOV baseline established"
        COMMAND echo "-- LCOV running tests..."
        COMMAND ${CMAKE_BINARY_DIR}/${HOLLOWMAC_TEST_NAME}
        COMMAND echo "-- LCOV tests complete"
        COMMAND echo "-- LCOV generating coverage..."
        COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} -d ${OBJECT_DIR} -c -o coverage.info
        COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} -a coverage.base -a coverage.info -o coverage.total
        COMMAND ${GENHTML_PATH} coverage.total --output-directory html
        COMMAND echo "-- LCOV coverage generation complete"
        COMMAND echo "-- GCOV analysis"
        COMMAND ${GCOV_PATH} ${SOURCES} -o ${OBJECT_DIR} > summary.txt
        COMMAND cat summary.txt
        COMMAND echo "-- Coverage files have been output to ${CMAKE_BINARY_DIR}/${HOLLOWMAC_COVERAGE}"
        COMMAND open ${CMAKE_BINARY_DIR}/${HOLLOWMAC_COVERAGE}/html/index.html
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${HOLLOWMAC_COVERAGE}
    )
    add_dependencies(${HOLLOWMAC_COVERAGE} ${HOLLOWMAC_TEST_NAME})
    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${HOLLOWMAC_COVERAGE})
endif ()

# HollowMac Sample App
set(HOLLOWMAC_APP_NAME controls)
set(APP_SOURCES ${APP_SOURCES} App/main.c)
add_executable(${HOLLOWMAC_APP_NAME} ${APP_SOURCES})
add_dependencies(${HOLLOWMAC_APP_NAME} hollowcore)
target_link_libraries(${HOLLOWMAC_APP_NAME} hollowmac)
