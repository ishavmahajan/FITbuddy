#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "calorie.h"

// In-memory storage
struct FoodEntry entries[MAX_ENTRIES];
int entryCount = 0;

const char* CALORIE_FILE = "calories.txt";
const char* GOAL_FILE = "goal.txt";

// ----------------------
// Utility: get today's date
// ----------------------
void getTodayDate(char* buffer)
{
    if (!buffer) return;
    time_t t = time(NULL);
    struct tm tm;
    localtime_s(&tm, &t);
    sprintf_s(buffer, 11, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

// ----------------------
// File I/O: load entries
// ----------------------
void loadCaloriesFromFile()
{
    FILE* file;
    errno_t err = fopen_s(&file, CALORIE_FILE, "r");
    if (err != 0 || !file) return; // File may not exist yet

    entryCount = 0;
    while (entryCount < MAX_ENTRIES)
    {
        char date[11], food[50];
        int cal;

        int result = fscanf_s(file, "%10[^,],%49[^,],%d\n", date, (unsigned)_countof(date), food, (unsigned)_countof(food), &cal);
        if (result != 3) break;

        strcpy_s(entries[entryCount].date, 11, date);
        strcpy_s(entries[entryCount].foodName, 50, food);
        entries[entryCount].calories = cal;

        entryCount++;
    }

    fclose(file);
}

// ----------------------
// File I/O: save entries
// ----------------------
void saveCaloriesToFile()
{
    FILE* file;
    errno_t err = fopen_s(&file, CALORIE_FILE, "w");
    if (err != 0 || !file)
    {
        printf("Error: Unable to save calories to file.\n");
        return;
    }

    for (int i = 0; i < entryCount; i++)
    {
        fprintf(file, "%s,%s,%d\n", entries[i].date, entries[i].foodName, entries[i].calories);
    }

    fclose(file);
}

// ----------------------
// Add food entry
// ----------------------
void addFoodEntry()
{
    if (entryCount >= MAX_ENTRIES)
    {
        printf("Error: Maximum entries reached!\n");
        return;
    }

    char date[11];
    getTodayDate(date);

    int continueAdding = 1;

    while (continueAdding)
    {
        printf("\nSelect food:\n1 Apple (80)\n2 Banana (100)\n3 Rice (200)\n4 Chicken Breast (250)\n5 Protein Shake (200)\n6 Custom Food\n");
        int choice;
        if (scanf_s("%d", &choice) != 1)
        {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }
        while (getchar() != '\n'); // clear leftover characters after reading the number

        char name[50];
        int cal = 0;

        switch (choice)
        {
        case 1: strcpy_s(name, 50, "Apple"); cal = 80; break;
        case 2: strcpy_s(name, 50, "Banana"); cal = 100; break;
        case 3: strcpy_s(name, 50, "Rice"); cal = 200; break;
        case 4: strcpy_s(name, 50, "Chicken Breast"); cal = 250; break;
        case 5: strcpy_s(name, 50, "Protein Shake"); cal = 200; break;
        case 6:
            printf("Enter food name: ");
            scanf_s(" %[^\n]", name, (unsigned)_countof(name));
            printf("Enter calories: ");
            if (scanf_s("%d", &cal) != 1)
            {
                printf("Invalid calorie input!\n");
                while (getchar() != '\n'); // clear input buffer
                continue;
            }
            break;
        default:
            printf("Invalid choice!\n");
            continue;
        }

        if (entryCount >= MAX_ENTRIES)
        {
            printf("Maximum entries reached!\n");
            break;
        }

        strcpy_s(entries[entryCount].foodName, 50, name);
        entries[entryCount].calories = cal;
        strcpy_s(entries[entryCount].date, 11, date);
        entryCount++;

        saveCaloriesToFile();
        printf("%s (%d calories) added for %s\n", name, cal, date);

        // Ask if user wants to add more
        char cont;
        printf("Do you want to add another food? (y/n): ");
        scanf_s(" %c", &cont, 1);
        while (getchar() != '\n'); // flush leftover input

        if (cont != 'y' && cont != 'Y')
            continueAdding = 0;
    }
}

// ----------------------
// Calorie menu
// ----------------------
void calorieMenu()
{
    int choice;
    while (1)
    {
        printf("\nCalorie Tracking Menu\n1 Add Food Entry\n2 View Today's Calories\n3 View Calories by Date\n4 Delete Food Entry\n5 Set Daily Calorie Goal\n6 View Calorie Goal Status\n7 Return to Main Menu\nEnter choice: ");
        if (scanf_s("%d", &choice) != 1)
        {
            printf("Invalid input!\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice)
        {
        case 1: addFoodEntry(); break;
        case 2: viewTodayCalories(); break;
        case 3: viewCaloriesByDate(); break;
        case 4: deleteFoodEntry(); break;
        case 5: setCalorieGoal(); break;
        case 6: viewCalorieGoalStatus(); break;
        case 7: return;
        default: printf("Invalid choice\n");
        }
    }
}

// ----------------------
// Placeholders for functions not implemented yet
// ----------------------
void viewTodayCalories() { printf("viewTodayCalories() not implemented yet\n"); }
void viewCaloriesByDate() { printf("viewCaloriesByDate() not implemented yet\n"); }
void deleteFoodEntry() { printf("deleteFoodEntry() not implemented yet\n"); }
void setCalorieGoal() { printf("setCalorieGoal() not implemented yet\n"); }
void viewCalorieGoalStatus() { printf("viewCalorieGoalStatus() not implemented yet\n"); }

int getTodayCalories() { return 0; }
int getCaloriesByDate(const char* date) { return 0; }
int getWeeklyCalories() { return 0; }
int getMonthlyCalories() { return 0; }