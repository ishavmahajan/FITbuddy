#define _CRT_SECURE_NO_WARNINGS
#include "workout.h"
#include "BMI.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "console_color.h"
#include <time.h>

Workout* head = NULL;

// created to clear buffer safely for both user inputs and automated tests
static void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// new feature added: delete workout from history
void deleteWorkout() {
    if (head == NULL) {
        set_color(COLOR_ERROR);
        printf("No workouts to delete.\n");
        set_color(COLOR_DEFAULT);
        return;
    }

    char target_name[50];
    set_color(COLOR_MENU);
    printf("Enter the name of the exercise to delete: ");
    set_color(COLOR_DEFAULT);
    if (scanf_s("%49s", target_name, (unsigned)_countof(target_name)) != 1) {
        if (feof(stdin)) return;
    }
    

    Workout* current = head, * prev = NULL;

    while (current != NULL) {
        if (strcmp(current->exercise_name, target_name) == 0) {
            if (prev == NULL) {
                head = current->next;
            }
            else {
                prev->next = current->next;
            }
            free(current);
            set_color(COLOR_SUCCESS);
            printf("Workout '%s' deleted successfully!\n", target_name);
            set_color(COLOR_DEFAULT);
            save_workouts_to_file(); // Updates the file immediately
            return;
        }
        prev = current;
        current = current->next;
    }

    set_color(COLOR_ERROR);
    printf("Workout '%s' not found.\n", target_name);
    set_color(COLOR_DEFAULT);
}

// new feature added: workout sub menu
void workoutMenu() {
    int subChoice = 0;
    while (subChoice != 4) {
        set_color(COLOR_TITLE);
        printf("\n** WORKOUT MANAGEMENT **\n");
        set_color(COLOR_MENU);
        printf("[1] Log New Workout\n");
        printf("[2] View Workout History\n");
        printf("[3] Delete a Workout\n");
        printf("[4] Back to Main Menu\n");
        printf("Selection: ");
        set_color(COLOR_DEFAULT);

        if (scanf_s("%d", &subChoice) != 1) {
            if (feof(stdin)) return; //  for automated testing
            set_color(COLOR_ERROR);
            printf("Invalid input. Use numbers 1-4.\n");
            set_color(COLOR_DEFAULT);
            clear_input_buffer();
            continue;
        }

        switch (subChoice) {
        case 1:
            addWorkout();
            save_workouts_to_file();
            break;
        case 2:
            view_workouts();
            break;
        case 3:
            deleteWorkout();
            break;
        case 4:
            return;
        default:
            set_color(COLOR_ERROR);
            printf("Invalid choice.\n");
            set_color(COLOR_DEFAULT);
        }
    }
}

void addWorkout() {
    Workout* new_workout = (Workout*)malloc(sizeof(Workout));
    if (new_workout == NULL) {
        set_color(COLOR_ERROR);
        printf("Memory allocation failed!\n");
        set_color(COLOR_DEFAULT);
        return;
    }

    

    set_color(COLOR_MENU);
    printf("Enter workout name: ");
    set_color(COLOR_DEFAULT);
    scanf_s("%49s", new_workout->exercise_name, (unsigned)_countof(new_workout->exercise_name));
    clear_input_buffer();

	// 1. Duration Input Validation
    do {
        set_color(COLOR_MENU);
        printf("Enter the Duration (mins): ");
        set_color(COLOR_DEFAULT);
        if (scanf_s("%d", &new_workout->duration_minutes) != 1 || new_workout->duration_minutes <= 0) {
            if (feof(stdin)) { free(new_workout); return; }
            set_color(COLOR_ERROR);
            printf("Error: Positive number required.\n");
            set_color(COLOR_DEFAULT);
            clear_input_buffer();
        }
        else {
            break;
        }
    } while (1);

    // 2. Intensity Input Validation
    int valid_intensity = 0;
    while (!valid_intensity) {
        set_color(COLOR_MENU);
        printf("1=low, 2=medium, 3=high\nEnter intensity (1-3): ");
        set_color(COLOR_DEFAULT);
        if (scanf_s("%d", &new_workout->intensity) == 1 &&
            new_workout->intensity >= 1 && new_workout->intensity <= 3) {
            valid_intensity = 1;
        }
        else {
            if (feof(stdin)) { free(new_workout); return; }
            set_color(COLOR_ERROR);
            printf("Error: Invalid intensity.\n");
            set_color(COLOR_DEFAULT);
            clear_input_buffer();
        }
    }

    // 3.Ttakes automatic dates
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(new_workout->date, sizeof(new_workout->date), "%Y-%m-%d", tm_info);

    // 4. Calculations and Linking
    new_workout->calories_burned = calculate_calories_burned(new_workout->intensity, new_workout->duration_minutes);

    new_workout->next = head;
    head = new_workout;

    set_color(COLOR_SUCCESS);
    printf("Workout added successfully! (%.2f kcal)\n", new_workout->calories_burned);
    set_color(COLOR_DEFAULT);
}


void view_workouts() {
    if (head == NULL) {
        set_color(COLOR_ERROR);
        printf("No workouts found.\n");
        set_color(COLOR_DEFAULT);
        return;
    }

    set_color(COLOR_MENU);
    printf("\n==============================================================================\n");
    printf("                          FITBUDDY: WORKOUT HISTORY                             \n");
    printf("==============================================================================\n");
    printf("%-12s | %-15s | %-8s | %-10s | %-10s\n", "DATE", "EXERCISE", "MINS", "INTENSITY", "CALORIES");
    printf("-------------|-----------------|----------|------------|----------\n");
    set_color(COLOR_DEFAULT);

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
    set_color(COLOR_MENU);
    printf("==============================================================================\n");
    set_color(COLOR_DEFAULT);
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
    set_color(COLOR_SUCCESS);
    printf("Memory for workout list has been cleared.\n");
    set_color(COLOR_DEFAULT);
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