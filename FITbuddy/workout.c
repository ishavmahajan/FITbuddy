#define _CRT_SECURE_NO_WARNINGS
#include "workout.h"
#include "BMI.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Intensity 1 (Low)    : MET 3.5
// Intensity 2 (Medium) : MET 5.0
// Intensity 3 (High)   : MET 8.0

#define DEFAULT_WEIGHT_KG 70.0

Workout* head = NULL;

void addWorkout()
{
    Workout* new_workout = (Workout*)malloc(sizeof(Workout));
    if (new_workout == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    while (getchar() != '\n' && getchar() != EOF);
    printf("Enter workout name: ");
    scanf("%s", new_workout->exercise_name);

    do {
        printf("Enter the Duration (mins): ");
        if (scanf("%d", &new_workout->duration_minutes) != 1 || new_workout->duration_minutes <= 0) {
            printf("Error: Duration must be a positive number.\n");
            while (getchar() != '\n');
        }
        else {
            break;
        }
    } while (1);

    int valid_intensity = 0;
    while (!valid_intensity) {
        printf("1=low\n2=medium\n3=high\n");
        printf("Enter the intensity (1-3): ");
        if (scanf("%d", &new_workout->intensity) == 1 &&
            new_workout->intensity >= 1 &&
            new_workout->intensity <= 3) {
            valid_intensity = 1;
        }
        else {
            printf("Error: Invalid input. Please enter 1, 2, or 3.\n");
            while (getchar() != '\n');
        }
    }

    printf("Enter the date (YYYY-MM-DD): ");
    scanf("%10s", new_workout->date);

    new_workout->calories_burned = calculate_calories_burned(new_workout->intensity, new_workout->duration_minutes);
    printf("Success! You Burned approx: %.2f\n", new_workout->calories_burned);

    new_workout->next = head;
    head = new_workout;

    printf("Workout added successfully!\n");
}

void view_workouts() {
    if (head == NULL) {
        printf("No workouts found.\n");
        return;
    }

    printf("\n==============================================================================\n");
    printf("                          FITBUDDY: WORKOUT HISTORY                             \n");
    printf("==============================================================================\n");
    printf("%-12s | %-15s | %-8s | %-10s | %-10s\n", "DATE", "EXERCISE", "MINS", "INTENSITY", "CALORIES");
    printf("-------------|-----------------|----------|------------|----------\n");

    Workout* current = head;
    while (current != NULL) {
        printf("%-12s | %-15s | %-8d | %-10d | %-10.2f\n",
            current->date,
            current->exercise_name,
            current->duration_minutes,
            current->intensity,
            current->calories_burned);
        current = current->next;
    }
    printf("==============================================================================\n");
}

float calculate_calories_burned(int intensity, int duration_minutes) {
    float met_value;

    if (intensity == 1) {
        met_value = 3.5;
    }
    else if (intensity == 2) {
        met_value = 5.0;
    }
    else if (intensity == 3) {
        met_value = 8.0;
    }
    else {
        return 0;
    }

    float weight_kg = get_latest_weight_kg();
    float kcal_per_minute = (met_value * 3.5f * weight_kg) / 200.0f;
    return kcal_per_minute * duration_minutes;
}

void save_workouts_to_file() {
    FILE* file = fopen("Workout.txt", "w");
    if (file == NULL) {
        return;
    }

    Workout* current = head;
    while (current != NULL) {
        fprintf(file, "Workout %s %d %d %.2f %s\n",
            current->exercise_name,
            current->duration_minutes,
            current->intensity,
            current->calories_burned,
            current->date);
        current = current->next;
    }

    fclose(file);
}

void load_workouts_from_file() {
    FILE* file = fopen("Workout.txt", "r");
    if (file == NULL) {
        return;
    }

    char line[256];
    char name[50];
    char date[11];
    int duration, intensity;
    float calories;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "Workout %49s %d %d %f %10s", name, &duration, &intensity, &calories, date) == 5) {
            Workout* new_node = (Workout*)malloc(sizeof(Workout));
            if (new_node != NULL) {
                strcpy(new_node->exercise_name, name);
                new_node->duration_minutes = duration;
                new_node->intensity = intensity;
                new_node->calories_burned = calories;
                strcpy(new_node->date, date);

                new_node->next = head;
                head = new_node;
            }
        }
    }

    fclose(file);
}

void free_memory() {
    Workout* current = head;
    Workout* next_node;

    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    head = NULL;
    printf("Memory for workout list has been cleared.\n");
}

float get_total_calories_burned() {
    float total = 0.0f;
    Workout* current = head;

    while (current != NULL) {
        total += current->calories_burned;
        current = current->next;
    }

    return total;
}