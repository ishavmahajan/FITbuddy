#define _CRT_SECURE_NO_WARNINGS
#include "workout.h"
#include <stdlib.h>
#include <string.h>

//Intensity 1 (Low) : MET 3.5 (Light calisthenics)

//Intensity 2 (Medium) : MET 5.0 (Moderate effort)

//Intensity 3 (High) : MET 8.0 (Vigorous / HIIT calisthenics)

// deafault weight for now to check the feature, will pull from BMI feature later
#define DEFAULT_WEIGHT_KG 70.0 


Workout* head = NULL; // head of the linked list

void addWorkout()
{
	Workout* new_workout = (Workout*)malloc(sizeof(Workout)); // using dynamic memory allocation to create a new workout instance
	if (new_workout == NULL) {   // Check if memory allocation was successful
		printf("Memory allocation failed!\n");
		return;
	}

	//User input
	//Name Input(Clear buffer first)
	while (getchar() != '\n' && getchar() != EOF);
	printf("Enter workout name: ");
	scanf(" %s", new_workout->exercise_name);

	// Duration Input with validation
	do {
		printf("Enter the Duration (mins): ");
		if (scanf("%d", &new_workout->duration_minutes) != 1 || new_workout->duration_minutes <= 0) {
			printf("Error: Duration must be a positive number.\n");
			while (getchar() != '\n');
		}
		else {
			break;
		}
	} while (1);

	// Intensity Input with validation
	int valid_intensity = 0;
	while (!valid_intensity) {
		printf("1=low\n2=medium\n3=high\n");
		printf("Enter the intensity (1-3): ");
		if (scanf("%d", &new_workout->intensity) == 1 &&
			new_workout->intensity >= 1 &&
			new_workout->intensity <= 3) {
			valid_intensity = 1; // Exit loop
		}
		else {
			printf("Error: Invalid input. Please enter 1, 2, or 3.\n");
			// Clear buffer in case they typed a letter
			while (getchar() != '\n');
		}
	}

	
	printf("Enter the date (YYYY-MM-DD): ");
	scanf("%10s", new_workout->date);

	new_workout->calories_burned = calculate_calories_burned(new_workout->intensity, new_workout->duration_minutes);
	printf("Success! You Burned approx:  %.2f\n", new_workout->calories_burned);

	new_workout->next = head;
	head = new_workout;

	printf("Workout added successfully!\n");

}

void view_workouts() {
	if (head == NULL) {
		printf("No workouts found.\n");
		return;
	}

	printf("\n==============================================================================\n");
	printf("                          FITBUDDY: WORKOUT HISTORY                             \n");
	printf("==============================================================================\n");
	printf("%-12s | %-15s | %-8s | %-10s | %-10s\n", "DATE", "EXERCISE", "MINS", "INTENSITY", "CALORIES");
	printf("-------------|-----------------|----------|------------|----------\n");

	Workout* current = head;
	while (current != NULL) {
		printf("%-12s | %-15s | %-8d | %-10d | %-10.2f\n",
			current->date,
			current->exercise_name,
			current->duration_minutes,
			current->intensity,
			current->calories_burned);
		current = current->next;
	}
	printf("==============================================================================\n");
}

float calculate_calories_burned(int intensity, int duration_minutes) {
	float met_value;

	if (intensity == 1) {
		met_value = 3.5; // Low intensity
	}
	else if (intensity == 2) {
		met_value = 5.0; // Medium intensity
	} else if (intensity == 3) {
		met_value = 8.0; // High intensity
	} else {
		return 0; // Invalid intensity
	}

	// Calculating calories burned using the MET formula	
	float kcal_per_minute = (met_value * 3.5 * DEFAULT_WEIGHT_KG) / 200; // Calories burned per minute
	return kcal_per_minute * duration_minutes;
	

}

void save_workouts_to_file() {
	FILE* file = fopen("Workout.txt", "w");
	if (file == NULL) {
		return;
	}
	Workout* current = head;
	while (current != NULL) {
		// Saving: Exercise Name, Duration, Intensity, Calories, Date
		fprintf(file, "Workout %s %d %d %.2f %s\n",
			current->exercise_name,
			current->duration_minutes,
			current->intensity,
			current->calories_burned,
			current->date);
		current = current->next;
	}

	fclose(file);
}


void load_workouts_from_file() {
	FILE* file = fopen("Workout.txt", "r");
	if (file == NULL) {
		return; // File doesn't exist yet
	}

	char line[256];
	char name[50];
	char date[11];
	int duration, intensity;
	float calories;

	// Use fgets to read the file line-by-line to prevent infinite loops
	while (fgets(line, sizeof(line), file)) {

		
		if (sscanf(line, "Workout %s %d %d %f %s", name, &duration, &intensity, &calories, date) == 5) {

			Workout* new_node = (Workout*)malloc(sizeof(Workout));
			if (new_node != NULL) {
				// Copy data to the new dynamic node
				strcpy(new_node->exercise_name, name);
				new_node->duration_minutes = duration;
				new_node->intensity = intensity;
				new_node->calories_burned = calories;
				strcpy(new_node->date, date);

				// Insert the new node at the beginning (Head)
				new_node->next = head;
				head = new_node;
			}
		}
	}

	fclose(file);
}

void free_memory() {
	Workout* current = head;
	Workout* next_node;

	while (current != NULL) {
		next_node = current->next; // save the next pointer before freeing
		free(current);           // free the current node
		current = next_node;     // move to the next node
	}
	head = NULL; // resetting head to NULL after freeing all nodes
	printf("Memory for workout list has been cleared.\n");
}

// Example helper for Ishav
float get_total_calories_burned() {
	float total = 0;
	Workout* current = head;
	while (current != NULL) {
		total += current->calories_burned;
		current = current->next;
	}
	return total;
}