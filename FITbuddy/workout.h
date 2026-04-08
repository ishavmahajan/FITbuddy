#ifndef WORKOUT_H
#define WORKOUT_H

typedef struct Workout {
    char exercise_name[50];
    int duration_minutes;
    int intensity;
    float calories_burned;
    char date[11];
    struct Workout* next;
} Workout;

extern Workout* head;

void addWorkout(void);
void view_workouts(void);
float calculate_calories_burned(int intensity, int duration_minutes);
void save_workouts_to_file(void);
void load_workouts_from_file(void);
void free_memory(void);
float get_total_calories_burned(void);

#endif