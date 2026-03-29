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

void log_weight(float height_cm) {
    float weight;
    char date[11];

    printf("Enter weight (kg): ");
    scanf_s("%f", &weight);

    printf("Enter date (YYYY-MM-DD): ");
    scanf_s("%10s", date, (unsigned)_countof(date));

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
