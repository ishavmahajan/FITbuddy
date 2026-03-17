#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "calorie.h"


// Array to store entries in memory
struct FoodEntry entries[MAX_ENTRIES];
int entryCount = 0;

// File names
const char* CALORIE_FILE = "calories.txt";
const char* GOAL_FILE = "goal.txt";

// load entries from calories.txt
void loadCaloriesFromFile()
{
    FILE* file = fopen(CALORIE_FILE, "r");
    if (!file) return;  // File might not exist yet

    entryCount = 0;
    while (fscanf_s(file, "%10[^,],%49[^,],%d\n",
        entries[entryCount].date,
        entries[entryCount].foodName,
        &entries[entryCount].calories) == 3)
    {
        entryCount++;
        if (entryCount >= MAX_ENTRIES) break; // prevent overflow
    }
    fclose(file);
}

// save entries to file
void saveCaloriesToFile()
{
    FILE* file = fopen(CALORIE_FILE, "w");
    if (!file) return;

    for (int i = 0; i < entryCount; i++)
    {
        fprintf(file, "%s,%s,%d\n",
            entries[i].date,
            entries[i].foodName,
            entries[i].calories);
    }
    fclose(file);
}