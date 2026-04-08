#include <stdio.h>
#ifndef WORKOUT_H
#define WORKOUT_H


typedef struct {

	char exercise_name[100];
	int duration_minutes; // in minutes
	float calories_burned;
	int intensity;  // 1=low, 2=medium, 3=high
	char date[11]; // Format: YYYY-MM-DD
	struct Workout* next;   // Pointer to the next workout in the list

} Workout;


void addWorkout();
void view_workouts();                  
float calculate_calories_burned(int intensity, int duration); 
void load_workouts_from_file();        // Loads from Workouts.txt 
void save_workouts_to_file();          // Saves to Workouts.txt 

//Memory Management
void free_memory();                    // frees the memory, avoid memroy leaks


#endif