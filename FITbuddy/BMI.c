#define _CRT_SECURE_NO_WARNINGS   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include "BMI.h"

#ifdef __cplusplus
extern "C" {
#endif

    Weight_Entry history[MAX_ENTRIES];
    int entry_count = 0;
    float current_bmi = 0.0f;

    float calculate_bmi(float weight_kg, float height_cm) {
        float height_m = height_cm / 100.0f;
        return weight_kg / (height_m * height_m);
    }

    const char* get_bmi_category(float bmi) {
        if (bmi < 18.5f) return "Underweight";
        if (bmi < 25.0f) return "Normal weight";
        if (bmi < 30.0f) return "Overweight";
        return "Obese";
    }

    float add_weight(float weight, const char* date, float height_cm) {
        if (entry_count >= MAX_ENTRIES) {
            printf("History full!\n");
            return -1;
        }

        Weight_Entry e;
        e.weight_kg = weight;
        e.bmi = calculate_bmi(weight, height_cm);

        strcpy_s(e.date, sizeof(e.date), date);

        /* Compare with previous entry if exists (before adding current entry) */
        if (entry_count > 0) {
            float prev_bmi = history[entry_count - 1].bmi;
            float diff = e.bmi - prev_bmi;

            if (diff > 0) {
                printf("You have GAINED %.2f BMI points since the last entry.\n", diff);
            }
            else if (diff < 0) {
                printf("You have LOST %.2f BMI points since the last entry.\n", -diff);
            }
            else {
                printf("Your BMI is unchanged since the last entry.\n");
            }
        }
        else {
            printf("This is your first entry. BMI recorded\n");
        }

        /* Store entry and update current BMI */
        history[entry_count++] = e;
        current_bmi = e.bmi;

        printf("Weight added successfully. Current BMI: %.2f (%s)\n", e.bmi, get_bmi_category(e.bmi));
        return e.bmi;
    }

    void view_weight_history(void) {
        printf("\n=== Weight History ===\n");
        for (int i = 0; i < entry_count; i++) {
            printf("%d) %s | %.1f kg | BMI: %.2f (%s)\n",
                i + 1,
                history[i].date,
                history[i].weight_kg,
                history[i].bmi,
                get_bmi_category(history[i].bmi));
        }
    }

    void load_weight_from_file(void) {
        FILE* f = fopen("Progress.txt", "r");
        if (!f) {
            printf("No file found");
            return;
        }

        entry_count = 0;

        while (entry_count < MAX_ENTRIES) {
            Weight_Entry e;

            if (fscanf_s(f, "%f %f %10s",
                &e.weight_kg,
                &e.bmi,
                e.date, (unsigned)_countof(e.date)) == 3)
            {
                history[entry_count++] = e;
            }
            else { break; }
        }

        fclose(f);

        if (entry_count > 0) {
            current_bmi = history[entry_count - 1].bmi;
        }
        else {
            current_bmi = 0.0f;
        }

        printf("Data loaded successfully. \n");
        printf("Current BMI: %.2f\n", current_bmi);
    }

    void save_weight_to_file(void) {
        FILE* f = fopen("Progress.txt", "w");
        if (!f) return;

        for (int i = 0; i < entry_count; i++) {
            fprintf(f, "%.1f %.2f %s\n",
                history[i].weight_kg,
                history[i].bmi,
                history[i].date);
        }

        fclose(f);
    }

    float set_bmi_goal(float goal_weight, float height_cm) {
        return calculate_bmi(goal_weight, height_cm);
    }

    void get_ideal_weight_range(float height_cm, float* min_weight, float* max_weight) {
        float height_m = height_cm / 100.0f;
        *min_weight = 18.5f * (height_m * height_m);
        *max_weight = 24.9f * (height_m * height_m);
    }

    void get_current_date(char* date, size_t size) {
        time_t now;
        struct tm local_time;

        time(&now);
        localtime_s(&local_time, &now);

        snprintf(date, size, "%04d-%02d-%02d",
            local_time.tm_year + 1900,
            local_time.tm_mon + 1,
            local_time.tm_mday);

    }

    static void run_bmi_goals_menu(float* height_cm) {
        char line[128];
        char choice = '\0';

        printf("\n--- Set BMI Goals ---\n");
        printf("A. Ideal BMI Recommendations\n");
        printf("B. Custom BMI Goal\n");
        printf("C. Compare with Current BMI\n");
        printf("Please choose: ");

        /* Đọc một dòng hợp lệ, bỏ qua các dòng rỗng do newline còn sót */
        while (1) {
            if (fgets(line, sizeof(line), stdin) == NULL) {
                printf("Invalid input.\n");
                return;
            }
            /* tìm ký tự đầu tiên không phải khoảng trắng */
            int idx = 0;
            while (line[idx] != '\0' && (line[idx] == ' ' || line[idx] == '\t' || line[idx] == '\n')) idx++;
            if (line[idx] == '\0') {
                printf("Please enter A, B or C: ");
                continue;
            }
            choice = line[idx];
            break;
        }

        /* chuẩn hoá về chữ thường */
        if (choice >= 'A' && choice <= 'Z') choice = (char)(choice - 'A' + 'a');

        switch (choice) {
        case 'a': {
            float h;
            float bmi_min = 18.5f, bmi_max = 24.9f;

            printf("\n--- Ideal BMI Recommendations ---\n");
            printf("Enter your height (cm): ");
            if (fgets(line, sizeof(line), stdin) == NULL || sscanf(line, "%f", &h) != 1 || h <= 0.0f) {
                printf("Error: Invalid height!\n");
                return;
            }

            printf("Enter your BMI range (e.g., 18.5 24.9) or press Enter to use default (18.5 24.9): ");
            if (fgets(line, sizeof(line), stdin) != NULL) {
                float a, b;
                if (sscanf(line, "%f %f", &a, &b) == 2) {
                    bmi_min = a; bmi_max = b;
                }
            }

            if (bmi_min <= 0.0f || bmi_max <= 0.0f || bmi_min > bmi_max) {
                printf("Error: Invalid BMI range!\n");
                return;
            }

            {
                float h_m = h / 100.0f;
                float min_weight = bmi_min * h_m * h_m;
                float max_weight = bmi_max * h_m * h_m;
                printf("\n--- Result ---\n");
                printf("Height: %.2f cm\n", h);
                printf("BMI range: %.1f - %.1f\n", bmi_min, bmi_max);
                printf("Ideal weight range: %.2f kg - %.2f kg\n", min_weight, max_weight);
            }
            break;
        }

        case 'b': {
            float goal;

            printf("\n--- Custom BMI Goal ---\n");
            printf("Enter goal weight (kg): ");
            if (fgets(line, sizeof(line), stdin) == NULL || sscanf(line, "%f", &goal) != 1 || goal <= 0.0f) {
                printf("Error: Invalid goal weight!\n");
                return;
            }

            printf("Enter height (cm): ");
            if (fgets(line, sizeof(line), stdin) == NULL || sscanf(line, "%f", height_cm) != 1 || *height_cm <= 0.0f) {
                printf("Error: Invalid height!\n");
                return;
            }

            {
                float bmi_goal = set_bmi_goal(goal, *height_cm);
                printf("Goal BMI = %.2f (%s)\n", bmi_goal, get_bmi_category(bmi_goal));
            }
            break;
        }

        case 'c': {
            float h;
            const float ideal_bmi = 22.0f;

            printf("\n--- Compare with Current BMI ---\n");

            if (entry_count == 0) {
                printf("No weight history found. Please log your weight first.\n");
                return;
            }

            printf("Enter your height (cm): ");
            if (fgets(line, sizeof(line), stdin) == NULL || sscanf(line, "%f", &h) != 1 || h <= 0.0f) {
                printf("Error: Invalid height!\n");
                return;
            }

            {
                float h_m = h / 100.0f;
                float current_weight = history[entry_count - 1].weight_kg;
                float current_bmi_local = history[entry_count - 1].bmi;
                float ideal_weight = ideal_bmi * h_m * h_m;
                float diff = current_weight - ideal_weight;

                printf("\n--- Latest Data ---\n");
                printf("Date: %s\n", history[entry_count - 1].date);
                printf("Current weight: %.2f kg\n", current_weight);
                printf("Current BMI: %.2f (%s)\n", current_bmi_local, get_bmi_category(current_bmi_local));

                printf("\n--- Ideal Data ---\n");
                printf("Ideal BMI: %.1f\n", ideal_bmi);
                printf("Ideal weight: %.2f kg\n", ideal_weight);

                printf("\n--- Comparison ---\n");
                if (diff > 0.0f) {
                    printf("You need to LOSE %.2f kg to reach the ideal BMI.\n", diff);
                }
                else if (diff < 0.0f) {
                    printf("You need to GAIN %.2f kg to reach the ideal BMI.\n", -diff);
                }
                else {
                    printf("You are already at the ideal weight.\n");
                }
            }
            break;
        }

        default:
            printf("Invalid choice. Try again.\n");
            break;
        }
    }

    void run_bmi_menu(float* height_cm) {
        int bmi_choice = 0;

        do {
            printf("\n===== BMI & Weight Module =====\n");
            printf("1. Update weight\n");
            printf("2. BMI Calculator\n");
            printf("3. Set BMI Goals\n");
            printf("4. View weight history\n");
            printf("7. Exit\n");
            printf("Choose: ");
            scanf_s("%d", &bmi_choice);

            switch (bmi_choice) {
            case 1:
            {
                float w;
                char date[11];

                printf("\n--- Update Weight ---\n");
                printf("Enter weight (kg): ");
                scanf_s("%f", &w);

                get_current_date(date, sizeof(date));
                printf("Current date: %s\n", date);

                add_weight(w, date, *height_cm);
                break;
            }

            case 2:
            {
                float w;
                float bmi;

                printf("\n--- BMI Calculator ---\n");
                printf("Enter weight (kg): ");
                scanf_s("%f", &w);

                printf("Enter height (cm): ");
                scanf_s("%f", height_cm);

                bmi = calculate_bmi(w, *height_cm);
                printf("BMI = %.2f (%s)\n", bmi, get_bmi_category(bmi));
                break;
            }

            case 3:
                run_bmi_goals_menu(height_cm);
                break;

            case 4:
                printf("\n--- Weight History ---\n");
                view_weight_history();
                break;

            case 7:
                printf("\nSaving data...\n");
                save_weight_to_file();
                printf("Saved. Exiting module.\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
                break;
            }

        } while (bmi_choice != 7);
    }

#ifdef __cplusplus
}
#endif