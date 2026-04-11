#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "calorie.h"

#define FILE_NAME "calories.txt"
#define GOAL_FILE "goal.txt"

static FoodEntry* entries = NULL;
static int entryCount = 0;
static int capacity = 0;
static int calorieGoal = 0;

// ===== Utility =====
static void ensureCapacity() {
    if (entryCount >= capacity) {
        capacity = (capacity == 0) ? 10 : capacity * 2;
        entries = (FoodEntry*)realloc(entries, capacity * sizeof(FoodEntry));
    }
}

static int inputInt(const char* prompt) {
    int value;
    char term;
    while (1) {
        printf("%s", prompt);
        if (scanf_s("%d%c", &value, &term, 1) != 2 || term != '\n') {
            printf(RED "Invalid input. Enter a number.\n" RESET);
            while (getchar() != '\n');
        }
        else {
            return value;
        }
    }
}

// ===== Food Lookup =====
static int lookupCalories(const char* foodName) {
    if (_stricmp(foodName, "apple") == 0) return 95;
    if (_stricmp(foodName, "banana") == 0) return 105;
    if (_stricmp(foodName, "bread") == 0) return 80;
    if (_stricmp(foodName, "rice") == 0) return 200;
    if (_stricmp(foodName, "egg") == 0) return 70;
    return inputInt("Calories not found. Enter manually: ");
}

// ===== File Handling =====
void loadCaloriesFromFile() {
    FILE* file = NULL;
    fopen_s(&file, FILE_NAME, "r");
    if (!file) return;

    FoodEntry temp;
    while (fscanf_s(file, "%10s %49s %d",
        temp.date, (unsigned)_countof(temp.date),
        temp.foodName, (unsigned)_countof(temp.foodName),
        &temp.calories) == 3) {
        ensureCapacity();
        entries[entryCount++] = temp;
    }
    fclose(file);
}

void saveCaloriesToFile() {
    FILE* file = NULL;
    fopen_s(&file, FILE_NAME, "w");
    if (!file) return;

    for (int i = 0; i < entryCount; i++) {
        fprintf(file, "%s %s %d\n",
            entries[i].date,
            entries[i].foodName,
            entries[i].calories);
    }
    fclose(file);
}

// ===== Goal =====
void loadGoalFromFile() {
    FILE* file = NULL;
    fopen_s(&file, GOAL_FILE, "r");
    if (!file) return;

    fscanf_s(file, "%d", &calorieGoal);
    fclose(file);
}

static void saveGoalToFile() {
    FILE* file = NULL;
    fopen_s(&file, GOAL_FILE, "w");
    if (!file) return;

    fprintf(file, "%d", calorieGoal);
    fclose(file);
}

// ===== Add =====
void addFoodEntry() {
    FoodEntry newEntry;

    printf("Enter food name: ");
    scanf_s("%49s", newEntry.foodName, (unsigned)_countof(newEntry.foodName));

    newEntry.calories = lookupCalories(newEntry.foodName);

    time_t t = time(NULL);
    struct tm tm;
    localtime_s(&tm, &t);

    sprintf_s(newEntry.date, MAX_DATE, "%04d-%02d-%02d",
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    ensureCapacity();
    entries[entryCount++] = newEntry;
    saveCaloriesToFile();

    printf(GREEN "Added: %s - %d calories\n" RESET,
        newEntry.foodName, newEntry.calories);

    int today = getTodayCalories();

    if (calorieGoal != 0 && today > calorieGoal) {
        printf(RED BOLD "\n⚠ WARNING: Goal exceeded!\n" RESET);
    }
}

// ===== View =====
void viewTodayCalories() {
    char today[MAX_DATE];
    time_t t = time(NULL);
    struct tm tm;
    localtime_s(&tm, &t);

    sprintf_s(today, MAX_DATE, "%04d-%02d-%02d",
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    int total = 0;

    printf(BLUE "\nToday's Entries:\n" RESET);

    for (int i = 0; i < entryCount; i++) {
        if (strcmp(entries[i].date, today) == 0) {
            printf("%d. %s - %d calories\n",
                i + 1,
                entries[i].foodName,
                entries[i].calories);
            total += entries[i].calories;
        }
    }

    printf("Total: %d calories\n", total);
}

// ===== Delete =====
void deleteFoodEntry() {
    int index = inputInt("Enter entry number to delete: ");

    if (index < 1 || index > entryCount) {
        printf(RED "Invalid index.\n" RESET);
        return;
    }

    for (int i = index - 1; i < entryCount - 1; i++) {
        entries[i] = entries[i + 1];
    }

    entryCount--;
    saveCaloriesToFile();

    printf(GREEN "Entry deleted.\n" RESET);
}

// ===== Stats =====
int getTodayCalories() {
    char today[MAX_DATE];
    time_t t = time(NULL);
    struct tm tm;
    localtime_s(&tm, &t);

    sprintf_s(today, MAX_DATE, "%04d-%02d-%02d",
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    int total = 0;

    for (int i = 0; i < entryCount; i++) {
        if (strcmp(entries[i].date, today) == 0)
            total += entries[i].calories;
    }

    return total;
}

int getWeeklyCalories() {
    int total = 0;
    time_t now = time(NULL);

    for (int i = 0; i < entryCount; i++) {
        struct tm tmEntry = { 0 };

        sscanf_s(entries[i].date, "%d-%d-%d",
            &tmEntry.tm_year, &tmEntry.tm_mon, &tmEntry.tm_mday);

        tmEntry.tm_year -= 1900;
        tmEntry.tm_mon -= 1;

        time_t entryTime = mktime(&tmEntry);

        double diff = difftime(now, entryTime) / (60 * 60 * 24);

        if (diff >= 0 && diff <= 7)
            total += entries[i].calories;
    }

    return total;
}

int getMonthlyCalories() {
    int total = 0;
    time_t now = time(NULL);

    for (int i = 0; i < entryCount; i++) {
        struct tm tmEntry = { 0 };

        sscanf_s(entries[i].date, "%d-%d-%d",
            &tmEntry.tm_year, &tmEntry.tm_mon, &tmEntry.tm_mday);

        tmEntry.tm_year -= 1900;
        tmEntry.tm_mon -= 1;

        time_t entryTime = mktime(&tmEntry);

        double diff = difftime(now, entryTime) / (60 * 60 * 24);

        if (diff >= 0 && diff <= 30)
            total += entries[i].calories;
    }

    return total;
}

// ===== Goal Status =====
void setCalorieGoal() {
    calorieGoal = inputInt("Enter daily calorie goal: ");
    saveGoalToFile();
    printf(GREEN "Goal set!\n" RESET);
}

void viewCalorieGoalStatus() {
    if (calorieGoal == 0) {
        printf(YELLOW "No goal set.\n" RESET);
        return;
    }

    int today = getTodayCalories();

    printf("\nGoal: %d | Consumed: %d\n", calorieGoal, today);

    if (today > calorieGoal)
        printf(RED "Exceeded goal!\n" RESET);
    else
        printf(GREEN "Within goal.\n" RESET);
}

// ===== Menu =====
void calorieMenu() {
    int choice;

    do {
        printf(BLUE BOLD "\n===== CALORIE TRACKER =====\n" RESET);

        printf("1. Add Food\n");
        printf("2. View Today\n");
        printf("3. Delete Food Entry\n");
        printf("4. Set Goal\n");
        printf("5. View Goal\n");
        printf("6. Exit\n");

        choice = inputInt("Enter choice: ");

        switch (choice) {
        case 1: addFoodEntry(); break;
        case 2: viewTodayCalories(); break;
        case 3: deleteFoodEntry(); break;
        case 4: setCalorieGoal(); break;
        case 5: viewCalorieGoalStatus(); break;
        case 6: printf("Exiting...\n"); break;
        default: printf(RED "Invalid choice\n" RESET);
        }

    } while (choice != 6);
}