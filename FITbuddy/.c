#define _CRT_SECURE_NO_WARNINGS   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmi.h"

Weight_Entry history[MAX_ENTRIES];
int entry_count = 0;

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

int is_valid_date(const char* date) {
    int y, m, d;

    // Cheking format YYYY-MM-DD
    if (scanf_s(date, "%4d-%2d-%2d", &y, &m, &d) != 3)
        return 0;

    // =Checking valid range of years
    if (y < 1900 || y > 2100) return 0;
    if (m < 1 || m > 12) return 0;
    if (d < 1 || d > 31) return 0;

    // Days in a month maxium
    int days_in_month[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

	// Checking leap year for February
    if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))
        days_in_month[1] = 29;

    if (d > days_in_month[m - 1])
        return 0;

    return 1;
}


void log_weight(float height_cm) {
    float weight;
    char date[11];

    printf("Enter weight (kg): ");
    scanf_s("%f", &weight);
    if (weight <= 0) {
		printf("Error: Invalid weight!\n");
		printf("Weight must be greater than 0.\n");
		printf("Please try again.\n");
        return;
    }
    do {
        printf("Enter date (YYYY-MM-DD): ");
        scanf_s("%10s", date, (unsigned)_countof(date));

        if (!is_valid_date(date)) {
            printf("❌ Invalid date format or value. Try again.\n");
        }
	} while (!is_valid_date(date));
    add_weight(weight, date, height_cm);
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

    history[entry_count++] = e;

    printf("Added: %.1f kg on %s | BMI: %.2f (%s)\n",
        e.weight_kg, e.date, e.bmi, get_bmi_category(e.bmi));

    return e.bmi;
}

void delete_weight(int index) {
    if (index < 0 || index >= entry_count) {
        printf("Invalid index!\n");
        return;
    }

    for (int i = index; i < entry_count - 1; i++) {
        history[i] = history[i + 1];
    }

    entry_count--;
    printf("Entry deleted.\n");
}

void view_weight_history() {
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

void load_weight_from_file() {
    FILE* f = fopen("Progress.txt", "r");
    if (!f) return;

    entry_count = 0;

    while (!feof(f)) {
        Weight_Entry e;

        if (fscanf_s(f, "%f %f %10s",
            &e.weight_kg,
            &e.bmi,
            e.date, (unsigned)_countof(e.date)) == 3)
        {
            history[entry_count++] = e;
        }
    }

    fclose(f);
}

void save_weight_to_file() {
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
