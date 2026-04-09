#ifndef BMI_H
#define BMI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#define MAX_ENTRIES 500

    typedef struct {
        float weight_kg;
        float bmi;
        char date[11]; /* YYYY-MM-DD, 10 chars + null */
    } Weight_Entry;

    extern Weight_Entry history[MAX_ENTRIES];
    extern int entry_count;
    extern float current_bmi;

    /* Public API */
    void run_bmi_menu(float* height_cm);
    void get_current_date(char* date, size_t size);
    float calculate_bmi(float weight_kg, float height_cm);
    const char* get_bmi_category(float bmi);
    void view_weight_history(void);
    void load_weight_from_file(void);
    void save_weight_to_file(void);
    float add_weight(float weight, const char* date, float height_cm);
    float set_bmi_goal(float goal_weight, float height_cm);
    void get_ideal_weight_range(float height_cm, float* min_weight, float* max_weight);
    float get_latest_weight_kg();

#ifdef __cplusplus
}
#endif

#endif /* BMI_H */