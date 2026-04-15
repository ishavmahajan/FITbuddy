#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <windows.h>

#include "tips.h"
#include "workout.h"
#include "BMI.h"
#include "calorie.h"
#include "report.h"
#include "console_color.h"

int main(int argc, char* argv[]) {
    int choice = 0;
    float height_cm = 170.0f;   // default height

    srand((unsigned int)time(NULL));

    load_workouts_from_file();
    load_weight_from_file();
    loadCaloriesFromFile();

   
    if (argc >= 2) {
        display_random_tip(argv[1]);
    }

    set_color(COLOR_TITLE);
    printf("Welcome to FITbuddy!\n");
    set_color(COLOR_DEFAULT);

    while (choice != 5) {
        set_color(COLOR_MAIN_MENU);
        printf("\n--- MAIN MENU ---\n");
        printf("[1] Log Calorie Intake\n");
        printf("[2] Log Workout\n");
        printf("[3] BMI & Weight Tracking\n");
        printf("[4] View Weekly Report\n");
        printf("[5] Exit\n");
        printf("Enter choice: ");
        set_color(COLOR_DEFAULT);
        if (scanf_s("%d", &choice) != 1) {
            if (feof(stdin)) {
                // if the test file ends, exit the loop naturally
                choice = 5;
            }
            else {
                set_color(COLOR_ERROR);
                printf("Invalid input. Enter a number.\n");
                set_color(COLOR_DEFAULT);

                // Clear buffer properly for both humans and automated tests
                int c;
                while ((c = getchar()) != '\n' && c != EOF);

                choice = 0;
                continue;
            }
        }

        switch (choice) {
        case 1:
            calorieMenu();
            break;

        case 2:
            set_color(COLOR_MENU);
            printf("** Workout Tracker **\n");
            set_color(COLOR_DEFAULT);
            workoutMenu();
            break;

        case 3:
            run_bmi_menu(&height_cm);
            break;

        case 4:
            generate_weekly_report();
            break;

        case 5:
            save_weight_to_file();
            saveCaloriesToFile();
            free_memory();
            set_color(COLOR_SUCCESS);
            printf("Goodbye!\n");
            set_color(COLOR_DEFAULT);
            break;

        default:
            set_color(COLOR_ERROR);
            printf("Invalid choice. Try again.\n");
            set_color(COLOR_DEFAULT);
            break;
        }
    }

    return 0;
}