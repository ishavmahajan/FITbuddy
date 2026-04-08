#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include "tips.h"
#include "workout.h"
#include "BMI.h"
#include "calorie.h"
#include "report.h"

int main(int argc, char* argv[]) {
    int choice = 0;
    float height_cm = 170.0f;   // default height

    /* Check command line argument */
    if (argc < 2) {
        printf("Usage: FITbuddy.exe tips.txt\n");
        return 1;
    }

    /* Seed random number generator */
    srand((unsigned int)time(NULL));

    /* Load saved data */
    load_workouts_from_file();
    load_weight_from_file();
    loadCaloriesFromFile();

    /* Display random tip */
    display_random_tip(argv[1]);

    printf("Welcome to FITbuddy!\n");

    while (choice != 5) {
        printf("\n--- MAIN MENU ---\n");
        printf("[1] Log Calorie Intake\n");
        printf("[2] Log Workout\n");
        printf("[3] BMI & Weight Tracking\n");
        printf("[4] View Weekly Report\n");
        printf("[5] Exit\n");
        printf("Enter choice: ");
        if (scanf_s("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            choice = 0;
            continue;
        }

        switch (choice) {
        case 1:
            // Use your existing calorie module menu
            calorieMenu();
            break;

        case 2:
            printf("** Workout Tracker **\n");
            addWorkout();
            view_workouts();
            save_workouts_to_file();
            break;

        case 3:
            run_bmi_menu(&height_cm);
            break;

        case 4:
            // New report feature
            generate_weekly_report();
            break;

        case 5:
            // Save everything before exit

            save_weight_to_file();
            saveCaloriesToFile();
            free_memory();
            printf("Goodbye!\n");
            break;

        default:
            printf("Invalid choice. Try again.\n");
            break;
        }
    }

    return 0;
}