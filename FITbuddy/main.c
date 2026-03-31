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

					printf("Enter height (cm): ");
					scanf_s("%f", &height_cm);

                    float bmi = calculate_bmi(w, height_cm);
                    printf("BMI = %.2f (%s)\n", bmi, get_bmi_category(bmi));
                    break;
                }

                case 3: {
                    char n_choice;
                    printf("\n--- Set BMI Goal ---\n");
                    printf("\n---Addtional functions---\n");
                    printf("A. Ideal BMI Recommendations\n");
					printf("B. Custom BMI Goal\n");
					printf("C. Compare with Current BMI\n");
                    printf("Please choose:");
					scanf_s("%c", &n_choice);
                    switch (n_choice)
                    case 'A': {
                    float h;
                    float bmi_min, bmi_max;

                    printf("\n--- Ideal BMI Recommendations ---\n");
                    printf("For adults, a healthy BMI is typically between 18.5 and 24.9.\n");
                    printf("BMI Categories:\n");
                    printf("Underweight: < 18.5\n");
                    printf("Normal weight: 18.5 - 24.9\n");
                    printf("Overweight: 25 - 29.9\n");
                    printf("Obesity: >= 30\n");

                    // Input height
                    printf("\nEnter your height (cm): ");
                    scanf_s("%f", &h);

                    if (h <= 0) {
                        printf("Error: Invalid height!\n");
                        break;
                    }

                    // Input BMI range
                    printf("Enter your BMI range (e.g., 18.5 24.9): ");
                    scanf_s("%f %f", &bmi_min, &bmi_max);

                    if (bmi_min <= 0 || bmi_max <= 0 || bmi_min > bmi_max) {
                        printf("Error: Invalid BMI range!\n");
                        break;
                    }

                    // Convert to meters
                    float h_m = h / 100.0f;

                    // Calculate weight range
                    float min_weight = bmi_min * h_m * h_m;
                    float max_weight = bmi_max * h_m * h_m;

                    // Output
                    printf("\n--- Result ---\n");
                    printf("Height: %.2f cm\n", h);
                    printf("BMI range: %.1f - %.1f\n", bmi_min, bmi_max);
                    printf("Ideal weight range: %.2f kg - %.2f kg\n", min_weight, max_weight);

                    break;
                }
				}
                    case 'B': {
                    float goal;
                    printf("Enter goal weight (kg): ");
                    scanf_s("%f", &goal);

                    printf("Enter height (cm): ");
                    scanf_s("%f", &height_cm);

                    float bmi_goal = set_bmi_goal(goal, height_cm);
                    printf("Goal BMI = %.2f (%s)\n", bmi_goal, get_bmi_category(bmi_goal));


                    break;
                    }
                    
                    case 'C': {
                        float h;
                        float h_m;
                        float ideal_bmi = 22.0f;
                        float ideal_weight;
                        float current_weight;
                        float current_bmi;
                        float diff;

						printf("\n--- Compare with Current BMI ---\n");
                        if (entry_count == 0) {
							printf("No weight history found. Please log your weight first.\n");
                            break;
                        }

						printf("Enter your height (cm): ");
						scanf_s("%f", &h);

                        if (h <= 0) {
                            printf("Error: Invalid height!\n");
                            break;
                        }

						h_m = h / 100.0f;

                        current_weight = history[entry_count - 1].weight_kg;
                        current_bmi = history[entry_count - 1].bmi;

                        ideal_weight = ideal_bmi * h_m * h_m;
                        diff = current_weight - ideal_weight;

                        printf("\n--- Latest Data ---\n");
                        printf("Date: %s\n", history[entry_count - 1].date);
                        printf("Current weight: %.2f kg\n", current_weight);
                        printf("Current BMI: %.2f (%s)\n", current_bmi, get_bmi_category(current_bmi));

                        printf("\n--- Ideal Data ---\n");
                        printf("Ideal BMI: %.1f\n", ideal_bmi);
                        printf("Ideal weight: %.2f kg\n", ideal_weight);

                        printf("\n--- Comparison ---\n");
                        if (diff > 0) {
                            printf("You need to LOSE %.2f kg to reach the ideal BMI.\n", diff);
                        }
                        else if (diff < 0) {
                            printf("You need to GAIN %.2f kg to reach the ideal BMI.\n", -diff);
                        }
                        else {
                            printf("You are already at the ideal weight.\n");
                        }

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