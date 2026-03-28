#include "workout.h"

void addWorkout(Workout* w)
{
	printf("Enter workout name: ");
	scanf("%s", w->name);

	printf("Enter the Duration: ");
	scanf("%d", &w->duration);

	printf("1=low\n 2=medium\n 3=high\n ");
	printf("Enter the intensity: ");
	scanf("%d", &w->intensity);

}