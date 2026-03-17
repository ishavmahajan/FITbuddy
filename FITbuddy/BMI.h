#pragma once
#define BMI_DATE 11
typedef struct Weight_Entry {
	float weight;
	float bmi;
	char date[BMI_DATE];
}; Weight_Entry;

// Basic functional prototypes for BMI calculator
void  bmi_set_height_cm(float height_cm);
float bmi_get_height_cm(void);
void  bmi_set_data_file(const char* path);
float calculate_bmi(float weight_kg, float height_cm);
const char* get_bmi_category(float bmi);
float Set_BMT_goal(void);

// Calculate BMI based on weight and height
void  log_weight(void);
void  view_weight_history(void);
void  load_weight_from_file(void);
void  save_weight_to_file(void);
float Add_weight(void);
void  Delete_weight(void);




