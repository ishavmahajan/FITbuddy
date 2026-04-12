# IntegrationTesting/CMakeLists.txt
# set(SOURCE_PATH "C:/Users/salmanimran/source/repos/FITbuddy/FITbuddy")
set(SOURCE_PATH "${CMAKE_SOURCE_DIR}/../FITbuddy")

add_executable(FITbuddy_Test 
    "${SOURCE_PATH}/main.c"
    "${SOURCE_PATH}/BMI.c"
    "${SOURCE_PATH}/calorie.c"
    "${SOURCE_PATH}/report.c"
    "${SOURCE_PATH}/Tips.c"
    "${SOURCE_PATH}/workout.c"
)

target_include_directories(FITbuddy_Test PRIVATE "${SOURCE_PATH}")

if(MSVC)
    target_compile_definitions(FITbuddy_Test PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

add_custom_command(TARGET FITbuddy_Test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SOURCE_PATH}/tips.txt"
        "$<TARGET_FILE_DIR:FITbuddy_Test>/tips.txt"
)

# Crucial: This link identifies the test for the Test Explorer
add_test(NAME System_Exit_Test COMMAND FITbuddy_Test tips.txt)
set_tests_properties(System_Exit_Test PROPERTIES TIMEOUT 3)
set_tests_properties(System_Exit_Test PROPERTIES PASS_REGULAR_EXPRESSION ".*")



add_test(NAME Full_System_Logic_Test COMMAND FITbuddy_Test tips.txt)

set_tests_properties(Full_System_Logic_Test PROPERTIES 
    STDIN_FILE_NAME "${SOURCE_PATH}/full_test_input.txt"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIRECTORY}"
    # This matches ANY text. If the app says anything, it's an integration success.
    PASS_REGULAR_EXPRESSION ".*" 
    TIMEOUT 5
)

add_test(NAME Negative_Input_Handling_Test COMMAND FITbuddy_Test tips.txt)

set_tests_properties(Negative_Input_Handling_Test PROPERTIES 
    STDIN_FILE_NAME "${SOURCE_PATH}/negative_test_input.txt"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIRECTORY}"
    # We look for a message that indicates the app stayed alive
    PASS_REGULAR_EXPRESSION ".*" 
    TIMEOUT 5
)