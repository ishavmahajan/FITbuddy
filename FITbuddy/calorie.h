#ifndef CALORIE_H
#define CALORIE_H

#define MAX_NAME 50
#define MAX_DATE 11

// 🎨 Colors
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

typedef struct {
    char foodName[MAX_NAME];
    int calories;
    char date[MAX_DATE];
} FoodEntry;

// Core functions
void calorieMenu();
void addFoodEntry();
void viewTodayCalories();
void viewCaloriesByDate();
void deleteFoodEntry();
void setCalorieGoal();
void viewCalorieGoalStatus();

// Extra
void addFoodEntryWithParams(const char* foodName, int calories, const char* date);

// Stats
int getTodayCalories();
int getCaloriesByDate(const char* date);
int getWeeklyCalories();
int getMonthlyCalories();

// File handling
void loadCaloriesFromFile();
void saveCaloriesToFile();
void loadGoalFromFile();

void resetCalorieData();

#endif