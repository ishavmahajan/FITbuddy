#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tips.h"

int main(int argc, char* argv[]) {

    /* Check command line argument */
    if (argc < 2) {
        printf("Usage: FITbuddy.exe tips.txt\n");
        return 1;
    }

    /* Seed random number generator */
    srand((unsigned int)time(NULL));

    /* Display random tip */
    display_random_tip(argv[1]);

    int choice = 0;
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
            printf("BMI & Weight Feature\n");
			printf("1. Calculate BMI & Catergories your bodyweight:\n");
			printf("2. Log Weight\n");
			printf("3. Set your goal weight\n");
			printf("4. View Weight History\n");
			scanf_s("%d", &choice);
            switch (choice)
            {
            case 1:
            default:
                break;
            }
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