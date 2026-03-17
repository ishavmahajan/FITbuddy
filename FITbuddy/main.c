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
            printf("\n===== BMI & Weight Module =====\n");
            printf("1. Log weight\n");
			printf("2. BMI Calculator\n");
            printf("3. Set BMI goal\n");
            printf("4. View weight history\n");
            printf("5. Delete weight \n");
			printf("6. Add weight\n");
            printf("7. Save and exit\n");
            printf("Choose: ");
			scanf_s("%d", &choice);
            switch (choice)
            {
            case 1:
                printf("1. Log weight ");
                break;
            case 2:
                printf("2. BMI Calculator\n");
				break;
            case 3:
                printf("3. Set BMI goal\n");
				break;
            case 4:
                printf("4. View weight history\n");
				break;
            case 5:
                printf("5. Delete weight \n");
                break;
            case 6:
                printf("6. Add weight\n");
                break;
            case 7:
                printf("7. Save and exit\n");
                break;
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