# IntegrationTesting/CMakeLists.txt
set(SOURCE_PATH "${CMAKE_SOURCE_DIR}/../FITbuddy")
set(RUNNER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/run_with_input.cmake")


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

enable_testing()

function(add_fitbuddy_input_test test_name input_file timeout_seconds)
    add_test(
        NAME ${test_name}
        COMMAND ${CMAKE_COMMAND}
            -DTEST_EXE=$<TARGET_FILE:FITbuddy_Test>
            -DINPUT_FILE=${SOURCE_PATH}/${input_file}
            -DWORKING_DIR=${SOURCE_PATH}
            -DTIPS_FILE=${SOURCE_PATH}/tips.txt
            -P ${RUNNER_SCRIPT}
    )

    set_tests_properties(${test_name} PROPERTIES
        TIMEOUT ${timeout_seconds}
        WORKING_DIRECTORY "${SOURCE_PATH}"
        REQUIRED_FILES "${SOURCE_PATH}/${input_file};${SOURCE_PATH}/tips.txt"
    )
endfunction()

add_fitbuddy_input_test(System_Exit_Test        "exit_input.txt"          5)
add_fitbuddy_input_test(Full_System_Logic_Test  "full_test_input.txt"    10)
add_fitbuddy_input_test(Workout_Delete_Test     "delete_test_input.txt"   5)
add_fitbuddy_input_test(BMI_Update_Test         "bmi_test_input.txt"      5)
add_fitbuddy_input_test(Calorie_Food_Test       "calorie_test_input.txt"  5)
add_fitbuddy_input_test(Workout_Validation_Test "workout_val_input.txt"   5)
add_fitbuddy_input_test(BMI_Validation_Test     "bmi_val_input.txt"       5)
add_fitbuddy_input_test(Calorie_Validation_Test "calorie_val_input.txt"   5)
add_fitbuddy_input_test(Empty_Delete_Test       "empty_delete_input.txt"  5)
add_fitbuddy_input_test(Main_Menu_Boundary_Test "main_boundary_input.txt" 5)