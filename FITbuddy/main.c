#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include "tips.h"
#include "BMI.h"

int main(int argc, char* argv[]) {
    int choice = 0;
    float height_cm = 170.0f;   // hard code height here

    /* Check command line argument */
    if (argc < 2) {
        printf("Usage: FITbuddy.exe tips.txt\n");
        return 1;
    }

    /* Seed random number generator */
    srand((unsigned int)time(NULL));

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
        scanf_s("%d", &choice);

        switch (choice) {
        case 1:
            printf("Calorie Tracker - Coming Soon\n");
            break;

        case 2:
            printf("Workout Tracker - Coming Soon\n");
            break;

        case 3:
            run_bmi_menu(&height_cm);
            break;

        case 4:
            printf("Weekly Report - Coming Soon\n");
            break;

        case 5:
            printf("Goodbye!\n");
            break;

        default:
            printf("Invalid choice. Try again.\n");
            break;
        }
    }

    return 0;
}