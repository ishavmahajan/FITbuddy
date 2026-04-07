#include <stdio.h>
#ifndef WORKOUT_H
#define WORKOUT_H


typedef struct {

	char name[100];
	int duration; // in minutes
	int caloriesBurned;
	int intensity;  // 1=low, 2=medium, 3=high

} Workout;

void addWorkout(Workout* w);
void displayWorkout(Workout w);
void caloriesBurned(Workout w);

#endif