#define _CRT_SECURE_NO_WARNINGS   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmi.h"

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

int valid_date(const char* date) {
    if (date == NULL) {
        printf("Error: Date cannot be null!\n");
        return 0;
    }

    
    if (strlen(date) != 10) {
        printf("Error: Invalid date format! Use YYYY-MM-DD.\n");
        return 0;
    }

    
    if (date[4] != '-' || date[7] != '-') {
        printf("Error: Invalid date format! Use YYYY-MM-DD.\n");
        return 0;
    }

    int y, m, d;
    if (sscanf(date, "%4d-%2d-%2d", &y, &m, &d) != 3) {
        printf("Error: Invalid date format! Use YYYY-MM-DD.\n");
        return 0;
    }

    
    if (y < 1900 || y > 2100) {
        printf("Error: Year must be between 1900 and 2100.\n");
        return 0;
    }

    
    if (m < 1 || m > 12) {
        printf("Error: Month must be between 1 and 12.\n");
        return 0;
    }

    
    int max_day;

    switch (m) {
    case 2: {
        int leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
        max_day = leap ? 29 : 28;
        break;
    }
    case 4: case 6: case 9: case 11:
        max_day = 30;
        break;
    default:
        max_day = 31;
    }

    if (d < 1 || d > max_day) {
        printf("Error: Invalid day for this month.\n");
        return 0;
    }

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
    while (1) {
        printf("Enter date (YYYY-MM-DD): ");
        scanf_s("%10s", date, (unsigned)_countof(date));

        if (valid_date(date)) {
            break;  
        }

        printf("Please enter a valid date again.\n");
    }

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
    current_bmi = e.bmi;
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
    if (!f || f == NULL) {
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

    if(entry_count > 0) {
        current_bmi = history[entry_count - 1].bmi;
    }
    else {
        current_bmi = 0.0f;
    }

    printf("Data loaded successfully. \n");
    printf("Current BMI: %.2f\n", current_bmi);
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

void get_ideal_weight_range(float height_cm, float* min_weight, float* max_weight) {
    float height_m = height_cm / 100.0f;
    *min_weight = 18.5f * (height_m * height_m);
    *max_weight = 24.9f * (height_m * height_m);
}
