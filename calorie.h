#ifndef CALORIE_H
#define CALORIE_H

#define MAX_ENTRIES 500

// Struct for one food entry
struct FoodEntry
{
    char foodName[50];  // Name of the food
    int calories;       // Calories of the food
    char date[11];      // Date in YYYY-MM-DD format
};

// Function declarations
void calorieMenu();
void addFoodEntry();  // allows to add the food 
void viewTodayCalories();  // intake calories for today 
void viewCaloriesByDate();  
void deleteFoodEntry();
void setCalorieGoal();
void viewCalorieGoalStatus();

// functions for the progress report integration 
int getTodayCalories();    // Return total calories for today
int getCaloriesByDate(const char* date);  // return total calories for a specific date YYYY-MM-DD
int getWeeklyCalories();   // returns total calories in the last 7 days
int getMonthlyCalories();   // returns total calories in the last 30 days

// fiel handling 
void loadCaloriesFromFile();
void saveCaloriesToFile();

// Utility
void getTodayDate(char* buffer);

#endif