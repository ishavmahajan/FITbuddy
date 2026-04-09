#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "report.h"
#include "BMI.h"
#include "calorie.h"
#include "workout.h"
#include "console_color.h"

void generate_weekly_report(void) {
    int weeklyCalories = getWeeklyCalories();
    int monthlyCalories = getMonthlyCalories();

    float burnedTotal = get_total_calories_burned();

    float netWeekly = (float)weeklyCalories - burnedTotal;
    float netMonthly = (float)monthlyCalories - burnedTotal;

    set_color(COLOR_MENU);
    printf("\n===== WEEKLY SUMMARY =====\n");
    set_color(COLOR_DEFAULT);
    printf("Calories consumed this week     : %d\n", weeklyCalories);
    printf("Total calories burned workout   : %.2f\n", burnedTotal);
    set_color(COLOR_SUCCESS);
    printf("Net calories this week          : %.2f\n", netWeekly);
    set_color(COLOR_DEFAULT);

    set_color(COLOR_MENU);
    printf("\n===== MONTHLY SUMMARY =====\n");
    set_color(COLOR_DEFAULT);
    printf("Calories consumed this month    : %d\n", monthlyCalories);
    printf("Total calories burned workout   : %.2f\n", burnedTotal);
    set_color(COLOR_SUCCESS);
    printf("Net calories this month         : %.2f\n", netMonthly);
    set_color(COLOR_DEFAULT);

    set_color(COLOR_MENU);
    printf("\n--- Weight / BMI ---\n");
    set_color(COLOR_DEFAULT);
    if (entry_count > 0) {
        Weight_Entry latest = history[entry_count - 1];

        printf("Latest entry date: %s\n", latest.date);
        printf("Latest weight    : %.1f kg\n", latest.weight_kg);
        set_color(COLOR_SUCCESS);
        printf("Latest BMI       : %.2f (%s)\n",
            latest.bmi,
            get_bmi_category(latest.bmi));
        set_color(COLOR_DEFAULT);
    }
    else {
        set_color(COLOR_ERROR);
        printf("No weight data available.\n");
        set_color(COLOR_DEFAULT);
    }

    set_color(COLOR_MENU);
    printf("=========================\n");
    set_color(COLOR_DEFAULT);
}