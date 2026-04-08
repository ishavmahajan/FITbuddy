#ifndef CALORIE_H
#define CALORIE_H

#define MAX_NAME 50
#define MAX_DATE 11

typedef struct {
    char foodName[MAX_NAME];
    int calories;
    char date[MAX_DATE]; // YYYY-MM-DD
} FoodEntry;

// Interactive functions
void calorieMenu();              // Calorie tracker menu
void addFoodEntry();             // Add food entry interactively
void viewTodayCalories();        // Show calories for today
void viewCaloriesByDate();       // Show calories for a specific date
void deleteFoodEntry();          // Delete an entry interactively
void setCalorieGoal();           // Set daily calorie goal
void viewCalorieGoalStatus();    // View goal progress

// Advanced/test-friendly function
void addFoodEntryWithParams(const char* foodName, int calories, const char* date);

// Functions for progress report integration
int getTodayCalories();                   // Total calories today
int getCaloriesByDate(const char* date);  // Total calories by date
int getWeeklyCalories();                  // Total last 7 days
int getMonthlyCalories();                 // Total last 30 days

// File handling
void loadCaloriesFromFile();
void saveCaloriesToFile();

void resetCalorieData();

#endif