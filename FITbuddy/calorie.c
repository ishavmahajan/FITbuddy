#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "calorie.h"
#include "console_color.h"

#define FILE_NAME "calories.txt"

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
            set_color(COLOR_ERROR);
            printf("Invalid input. Enter a number.\n");
            set_color(COLOR_DEFAULT);
            while (getchar() != '\n');
        }
        else {
            return value;
        }
    }
}

// ===== Simple food database =====
static int lookupCalories(const char* foodName) {
    if (_stricmp(foodName, "apple") == 0) return 95;
    if (_stricmp(foodName, "banana") == 0) return 105;
    if (_stricmp(foodName, "bread") == 0) return 80;
    if (_stricmp(foodName, "rice") == 0) return 200;
    if (_stricmp(foodName, "egg") == 0) return 70;
    return inputInt("Calories for this food not found. Enter manually: ");
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

// ===== Add Entries =====
void addFoodEntry() {
    FoodEntry newEntry;
    set_color(COLOR_MENU);
    printf("Enter food name: ");
    set_color(COLOR_DEFAULT);
    scanf_s("%49s", newEntry.foodName, (unsigned)_countof(newEntry.foodName));
    newEntry.calories = lookupCalories(newEntry.foodName);

    // Set today's date
    time_t t = time(NULL);
    struct tm tm;
    localtime_s(&tm, &t);
    sprintf_s(newEntry.date, MAX_DATE, "%04d-%02d-%02d",
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    ensureCapacity();
    entries[entryCount++] = newEntry;
    saveCaloriesToFile();

    set_color(COLOR_SUCCESS);
    printf("Food entry added: %s - %d calories on %s\n",
        newEntry.foodName, newEntry.calories, newEntry.date);
    set_color(COLOR_DEFAULT);
}

// Test-friendly addition
void addFoodEntryWithParams(const char* foodName, int calories, const char* date) {
    FoodEntry newEntry;
    strncpy_s(newEntry.foodName, MAX_NAME, foodName, _TRUNCATE);
    newEntry.calories = calories;
    strncpy_s(newEntry.date, MAX_DATE, date, _TRUNCATE);

    ensureCapacity();
    entries[entryCount++] = newEntry;
}

// ===== View Functions =====
void viewCaloriesByDate() {
    char date[MAX_DATE];
    set_color(COLOR_MENU);
    printf("Enter date (YYYY-MM-DD): ");
    set_color(COLOR_DEFAULT);
    scanf_s("%10s", date, (unsigned)_countof(date));

    int found = 0;
    for (int i = 0; i < entryCount; i++) {
        if (strcmp(entries[i].date, date) == 0) {
            printf("%d. %s - %d calories\n", i + 1,
                entries[i].foodName, entries[i].calories);
            found = 1;
        }
    }
    if (!found) {
        set_color(COLOR_ERROR);
        printf("No entries found for this date.\n");
        set_color(COLOR_DEFAULT);
    }
}

void viewTodayCalories() {
    char today[MAX_DATE];
    time_t t = time(NULL);
    struct tm tm;
    localtime_s(&tm, &t);
    sprintf_s(today, MAX_DATE, "%04d-%02d-%02d",
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    int total = 0;
    set_color(COLOR_MENU);
    printf("Today's Entries:\n");
    set_color(COLOR_DEFAULT);
    for (int i = 0; i < entryCount; i++) {
        if (strcmp(entries[i].date, today) == 0) {
            printf("%s - %d calories\n", entries[i].foodName, entries[i].calories);
            total += entries[i].calories;
        }
    }
    set_color(COLOR_MENU);
    printf("Total Calories Today: %d\n", total);
    set_color(COLOR_DEFAULT);
}

// ===== Delete Entry =====
void deleteFoodEntry() {
    int index = inputInt("Enter entry index to delete: ");
    if (index < 1 || index > entryCount) {
        set_color(COLOR_ERROR);
        printf("Invalid index.\n");
        set_color(COLOR_DEFAULT);
        return;
    }

    for (int i = index - 1; i < entryCount - 1; i++) {
        entries[i] = entries[i + 1];
    }
    entryCount--;
    saveCaloriesToFile();
    set_color(COLOR_SUCCESS);
    printf("Entry deleted successfully.\n");
    set_color(COLOR_DEFAULT);
}

// ===== Goal Functions =====
void setCalorieGoal() {
    calorieGoal = inputInt("Enter daily calorie goal: ");
    set_color(COLOR_SUCCESS);
    printf("Goal set to %d calories.\n", calorieGoal);
    set_color(COLOR_DEFAULT);
}

int getCaloriesByDate(const char* date) {
    int total = 0;
    for (int i = 0; i < entryCount; i++) {
        if (strcmp(entries[i].date, date) == 0)
            total += entries[i].calories;
    }
    return total;
}

int getTodayCalories() {
    char today[MAX_DATE];
    time_t t = time(NULL);
    struct tm tm;
    localtime_s(&tm, &t);
    sprintf_s(today, MAX_DATE, "%04d-%02d-%02d",
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    return getCaloriesByDate(today);
}

void viewCalorieGoalStatus() {
    if (calorieGoal == 0) {
        set_color(COLOR_ERROR);
        printf("No goal set.\n");
        set_color(COLOR_DEFAULT);
        return;
    }
    int todayCalories = getTodayCalories();
    set_color(COLOR_MENU);
    printf("Goal: %d | Consumed: %d\n", calorieGoal, todayCalories);
    set_color(COLOR_DEFAULT);
    if (todayCalories > calorieGoal) {
        set_color(COLOR_ERROR);
        printf("You exceeded your goal.\n");
        set_color(COLOR_DEFAULT);
    }
    else {
        set_color(COLOR_SUCCESS);
        printf("You are within your goal.\n");
        set_color(COLOR_DEFAULT);
    }
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

        double diffDays = difftime(now, entryTime) / (60 * 60 * 24);

        if (diffDays >= 0 && diffDays <= 7)
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

        double diffDays = difftime(now, entryTime) / (60 * 60 * 24);

        if (diffDays >= 0 && diffDays <= 30)
            total += entries[i].calories;
    }

    return total;
}

void resetCalorieData() {
    free(entries);
    entries = NULL;
    entryCount = 0;
    capacity = 0;
}

// ===== Calorie Tracker Menu =====
void calorieMenu() {
    int choice;
    do {
        set_color(COLOR_MENU);
        printf("\n===== Calorie Tracker =====\n");
        printf("1. Add Food Entry\n");
        printf("2. View Today's Calories\n");
        printf("3. View Calories by Date\n");
        printf("4. Delete Food Entry\n");
        printf("5. Set Calorie Goal\n");
        printf("6. View Goal Status\n");
        printf("7. Back to Main Menu\n");
        printf("Enter choice: ");
        set_color(COLOR_DEFAULT);
        choice = inputInt("");

        switch (choice) {
        case 1:
            addFoodEntry();
            break;
        case 2:
            viewTodayCalories();
            break;
        case 3:
            viewCaloriesByDate();
            break;
        case 4:
            deleteFoodEntry();
            break;
        case 5:
            setCalorieGoal();
            break;
        case 6:
            viewCalorieGoalStatus();
            break;
        case 7:
            set_color(COLOR_SUCCESS);
            printf("Returning to main menu...\n");
            set_color(COLOR_DEFAULT);
            break;
        default:
            set_color(COLOR_ERROR);
            printf("Invalid choice.\n");
            set_color(COLOR_DEFAULT);
            break;
        }
    } while (choice != 7);
}