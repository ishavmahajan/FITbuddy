#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tips.h"
#include "BMI.h"

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
    float height_cm;
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
            do {
                printf("\n===== BMI & Weight Module =====\n");
                printf("1. Log weight\n");
                printf("2. BMI Calculator\n");
                printf("3. Set BMI goal\n");
                printf("4. View weight history\n");
                printf("5. Delete weight\n");
                printf("6. Add weight\n");
                printf("7. Save and exit\n");
                printf("Choose: ");
                scanf_s("%d", &choice);

                switch (choice)
                {
                case 1:
                    printf("\n--- Log weight ---\n");
                    printf("Enter your height (cm): ");
                    scanf_s("%f", &height_cm);
                    log_weight(height_cm);
                    break;

                case 2: {
                    printf("\n--- BMI Calculator ---\n");
                    float w;
                    printf("Enter weight (kg): ");
                    scanf_s("%f", &w);

                    float bmi = calculate_bmi(w, height_cm);
                    printf("BMI = %.2f (%s)\n", bmi, get_bmi_category(bmi));
                    break;
                }

                case 3: {
                    printf("\n--- Set BMI Goal ---\n");
                    float goal;
                    printf("Enter goal weight (kg): ");
                    scanf_s("%f", &goal);

                    float bmi_goal = set_bmi_goal(goal, height_cm);
                    printf("Goal BMI = %.2f (%s)\n", bmi_goal, get_bmi_category(bmi_goal));
                    break;
                }

                case 4:
                    printf("\n--- Weight History ---\n");
                    view_weight_history();
                    break;

                case 5: {
                    printf("\n--- Delete Weight Entry ---\n");
                    int index;
                    view_weight_history();
                    printf("Enter entry number to delete: ");
                    scanf_s("%d", &index);
                    delete_weight(index - 1);
                    break;
                }

                case 6: {
                    printf("\n--- Add Weight Manually ---\n");
                    float w;
                    char date[11];

                    printf("Enter weight (kg): ");
                    scanf_s("%f", &w);

                    printf("Enter date (YYYY-MM-DD): ");
                    scanf_s("%10s", date, (unsigned)_countof(date));

                    add_weight(w, date, height_cm);
                    break;
                }

                case 7:
                    printf("\nSaving data...\n");
                    save_weight_to_file();
                    printf("Saved. Exiting module.\n");
                    break;

                default:
                    printf("Invalid choice. Try again.\n");
                    break;
                }

            } while (choice != 7);

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