if(NOT DEFINED TEST_EXE)
    message(FATAL_ERROR "TEST_EXE was not provided")
endif()

if(NOT DEFINED INPUT_FILE)
    message(FATAL_ERROR "INPUT_FILE was not provided")
endif()

if(NOT DEFINED WORKING_DIR)
    message(FATAL_ERROR "WORKING_DIR was not provided")
endif()

if(NOT DEFINED TIPS_FILE)
    message(FATAL_ERROR "TIPS_FILE was not provided")
endif()

if(NOT EXISTS "${TEST_EXE}")
    message(FATAL_ERROR "Test executable not found: ${TEST_EXE}")
endif()

if(NOT EXISTS "${INPUT_FILE}")
    message(FATAL_ERROR "Input file not found: ${INPUT_FILE}")
endif()

if(NOT EXISTS "${TIPS_FILE}")
    message(FATAL_ERROR "Tips file not found: ${TIPS_FILE}")
endif()

file(REMOVE
    "${WORKING_DIR}/Progress.txt"
    "${WORKING_DIR}/Workout.txt"
    "${WORKING_DIR}/calories.txt"
)

execute_process(
    COMMAND "${TEST_EXE}" "${TIPS_FILE}"
    INPUT_FILE "${INPUT_FILE}"
    WORKING_DIRECTORY "${WORKING_DIR}"
    RESULT_VARIABLE test_result
    OUTPUT_VARIABLE test_output
    ERROR_VARIABLE test_error
)

message(STATUS "=== TEST STDOUT ===")
message(STATUS "${test_output}")

if(NOT "${test_error}" STREQUAL "")
    message(STATUS "=== TEST STDERR ===")
    message(STATUS "${test_error}")
endif()

if(NOT test_result EQUAL 0)
    message(FATAL_ERROR "Test failed with exit code ${test_result}")
endif()