#include "pch.h"
#include "CppUnitTest.h"
#include <ctime>

extern "C" {
#include "../FITbuddy/report.h"
#include "../FITbuddy/calorie.h"
#include "../FITbuddy/BMI.h"
#include "../FITbuddy/workout.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ReportModuleTests
{
    TEST_CLASS(ReportModuleTests)
    {
    public:

        static void formatDateOffset(char* buffer, size_t size, int daysOffset)
        {
            time_t t = time(NULL) + (daysOffset * 24 * 60 * 60);
            struct tm tmLocal;
            localtime_s(&tmLocal, &t);
            sprintf_s(buffer, size, "%04d-%02d-%02d",
                tmLocal.tm_year + 1900,
                tmLocal.tm_mon + 1,
                tmLocal.tm_mday);
        }

        TEST_METHOD_INITIALIZE(Setup)
        {
            resetCalorieData();

            entry_count = 0;
            current_bmi = 0.0f;

            for (int i = 0; i < MAX_ENTRIES; i++) {
                history[i].weight_kg = 0.0f;
                history[i].bmi = 0.0f;
                history[i].date[0] = '\0';
            }
        }

        TEST_METHOD(Test_ReportWeeklyCalories_NoEntries)
        {
            int total = getWeeklyCalories();
            Assert::AreEqual(0, total);
        }

        TEST_METHOD(Test_ReportMonthlyCalories_NoEntries)
        {
            int total = getMonthlyCalories();
            Assert::AreEqual(0, total);
        }

        TEST_METHOD(Test_ReportWeeklyCalories_WithSingleEntry)
        {
            char recentDate[11];
            formatDateOffset(recentDate, sizeof(recentDate), -1);

            addFoodEntryWithParams("apple", 95, recentDate);

            int total = getWeeklyCalories();

            Assert::IsTrue(total >= 95);
        }

        TEST_METHOD(Test_ReportMonthlyCalories_WithSingleEntry)
        {
            addFoodEntryWithParams("banana", 105, "2026-04-08");

            int total = getMonthlyCalories();

            Assert::IsTrue(total >= 105);
        }

        TEST_METHOD(Test_ReportMonthlyCalories_MultipleEntries)
        {
            char date1[11];
            char date2[11];
            formatDateOffset(date1, sizeof(date1), -20);
            formatDateOffset(date2, sizeof(date2), -3);

            addFoodEntryWithParams("rice", 200, date1);
            addFoodEntryWithParams("egg", 70, date2);

            int total = getMonthlyCalories();

            Assert::IsTrue(total >= 270);
        }

        TEST_METHOD(Test_ReportBMIData_AfterAddWeight)
        {
            float bmi = add_weight(70.0f, "2026-04-08", 170.0f);

            Assert::AreEqual(1, entry_count);
            Assert::AreEqual(70.0f, history[0].weight_kg, 0.01f);
            Assert::AreEqual(bmi, history[0].bmi, 0.01f);
            Assert::AreEqual(bmi, current_bmi, 0.01f);
            Assert::AreEqual("2026-04-08", history[0].date);
        }

        TEST_METHOD(Test_ReportBMIUsesLatestEntry)
        {
            add_weight(70.0f, "2026-04-01", 170.0f);
            float latestBmi = add_weight(75.0f, "2026-04-08", 170.0f);

            Assert::AreEqual(2, entry_count);
            Assert::AreEqual(75.0f, history[1].weight_kg, 0.01f);
            Assert::AreEqual(latestBmi, history[1].bmi, 0.01f);
            Assert::AreEqual(latestBmi, current_bmi, 0.01f);
            Assert::AreEqual("2026-04-08", history[1].date);
        }

        TEST_METHOD(Test_ReportBMIStatus_OverweightCategory)
        {
            float bmi = add_weight(80.0f, "2026-04-08", 170.0f);
            const char* category = get_bmi_category(bmi);

            Assert::AreEqual("Overweight", category);
        }

        TEST_METHOD(Test_ReportBMIStatus_NormalCategory)
        {
            float bmi = add_weight(63.0f, "2026-04-08", 170.0f);
            const char* category = get_bmi_category(bmi);

            Assert::AreEqual("Normal weight", category);
        }

        TEST_METHOD(Test_ReportGenerateFunction_CanBeCalled)
        {
            addFoodEntryWithParams("apple", 95, "2026-04-08");
            add_weight(70.0f, "2026-04-08", 170.0f);

            generate_weekly_report();

            Assert::IsTrue(true);
        }

        TEST_METHOD(Test_ReportWithCaloriesAndBMIData)
        {
            time_t t = time(NULL);
            struct tm tmLocal;
            localtime_s(&tmLocal, &t);

            char today[11];
            sprintf_s(today, "%04d-%02d-%02d",
                tmLocal.tm_year + 1900,
                tmLocal.tm_mon + 1,
                tmLocal.tm_mday);

            addFoodEntryWithParams("rice", 200, today);
            addFoodEntryWithParams("egg", 70, today);
            float bmi = add_weight(72.0f, today, 170.0f);

            int weeklyCalories = getWeeklyCalories();
            int monthlyCalories = getMonthlyCalories();

            Assert::IsTrue(weeklyCalories >= 270);
            Assert::IsTrue(monthlyCalories >= 270);
            Assert::AreEqual(bmi, current_bmi, 0.01f);
        }

        TEST_METHOD(Test_ReportMonthlyAtLeastWeekly)
        {
            addFoodEntryWithParams("apple", 95, "2026-04-08");
            addFoodEntryWithParams("banana", 105, "2026-04-07");

            int weeklyCalories = getWeeklyCalories();
            int monthlyCalories = getMonthlyCalories();

            Assert::IsTrue(monthlyCalories >= weeklyCalories);
        }
    };
}
