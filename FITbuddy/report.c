#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "report.h"
#include "BMI.h"
#include "calorie.h"
#include "workout.h"

void generate_weekly_report(void) {
    int weeklyCalories = getWeeklyCalories();
    int monthlyCalories = getMonthlyCalories();

    float burnedTotal = get_total_calories_burned();

    float netWeekly = (float)weeklyCalories - burnedTotal;
    float netMonthly = (float)monthlyCalories - burnedTotal;

    printf("\n===== WEEKLY SUMMARY =====\n");
    printf("Calories consumed this week  : %d\n", weeklyCalories);
    printf("Calories burned (workouts)   : %.2f\n", burnedTotal);
    printf("Net calories this week       : %.2f\n", netWeekly);

    printf("\n===== MONTHLY SUMMARY =====\n");
    printf("Calories consumed this month : %d\n", monthlyCalories);
    printf("Calories burned (workouts)   : %.2f\n", burnedTotal);
    printf("Net calories this month      : %.2f\n", netMonthly);

    printf("\n--- Weight / BMI ---\n");
    if (entry_count > 0) {
        Weight_Entry latest = history[entry_count - 1];

        printf("Latest entry date: %s\n", latest.date);
        printf("Latest weight    : %.1f kg\n", latest.weight_kg);
        printf("Latest BMI       : %.2f (%s)\n",
            latest.bmi,
            get_bmi_category(latest.bmi));
    }
    else {
        printf("No weight data available.\n");
    }

    printf("=========================\n");
}

