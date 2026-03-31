#ifndef BMI_H
#define BMI_H

#define MAX_ENTRIES 500

typedef struct {
    float weight_kg;     // > 0
    float bmi;           // auto-calculated
    char  date[11];      // YYYY-MM-DD
} Weight_Entry;

extern Weight_Entry history[MAX_ENTRIES];
extern int entry_count;
extern float current_bmi;

// Function declarations
void log_weight(float height_cm);
float calculate_bmi(float weight_kg, float height_cm);
const char* get_bmi_category(float bmi);
void view_weight_history();
void load_weight_from_file();
void save_weight_to_file();
float add_weight(float weight, const char* date, float height_cm);
void delete_weight(int index);
float set_bmi_goal(float goal_weight, float height_cm);
void get_ideal_weight_range(float height_cm, float* min_weight, float* max_weight);

#endif
