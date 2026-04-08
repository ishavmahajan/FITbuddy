#include "pch.h"
#include "CppUnitTest.h"

extern "C" {
#include "../FITbuddy/calorie.h"
}

#include <ctime>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CalorieModuleTests
{
    TEST_CLASS(CalorieModuleTests)
    {
    public:

        // Helper to reset state BEFORE each test
        TEST_METHOD_INITIALIZE(Setup)
        {
            resetCalorieData();   // ⚠️ You MUST implement this in your .c file
        }

        TEST_METHOD(Test_AddAndGetCaloriesByDate)
        {
            addFoodEntryWithParams("apple", 95, "2026-04-08");
            addFoodEntryWithParams("banana", 105, "2026-04-08");

            int total = getCaloriesByDate("2026-04-08");

            Assert::AreEqual(200, total);
        }

        TEST_METHOD(Test_GetCaloriesByDate_NoEntries)
        {
            int total = getCaloriesByDate("2000-01-01");

            Assert::AreEqual(0, total);
        }

        TEST_METHOD(Test_GetTodayCalories)
        {
            // Get today's date in same format
            time_t t = time(NULL);
            struct tm tm;
            localtime_s(&tm, &t);

            char today[11];
            sprintf_s(today, "%04d-%02d-%02d",
                tm.tm_year + 1900,
                tm.tm_mon + 1,
                tm.tm_mday);

            addFoodEntryWithParams("rice", 200, today);

            int total = getTodayCalories();

            Assert::IsTrue(total >= 200);
        }

        TEST_METHOD(Test_MultipleDatesIsolation)
        {
            addFoodEntryWithParams("apple", 95, "2026-04-01");
            addFoodEntryWithParams("banana", 105, "2026-04-02");

            int total1 = getCaloriesByDate("2026-04-01");
            int total2 = getCaloriesByDate("2026-04-02");

            Assert::AreEqual(95, total1);
            Assert::AreEqual(105, total2);
        }

        TEST_METHOD(Test_WeeklyCalories)
        {
            // Use a recent date (adjust if needed)
            addFoodEntryWithParams("egg", 70, "2026-04-07");

            int total = getWeeklyCalories();

            Assert::IsTrue(total >= 70);
        }

        TEST_METHOD(Test_MonthlyCalories)
        {
            addFoodEntryWithParams("bread", 80, "2026-04-01");

            int total = getMonthlyCalories();

            Assert::IsTrue(total >= 80);
        }

        TEST_METHOD(Test_MultipleEntriesSameDay)
        {
            addFoodEntryWithParams("apple", 50, "2026-04-08");
            addFoodEntryWithParams("apple", 50, "2026-04-08");
            addFoodEntryWithParams("apple", 50, "2026-04-08");

            int total = getCaloriesByDate("2026-04-08");

            Assert::AreEqual(150, total);
        }

        TEST_METHOD(Test_LargeCalories)
        {
            addFoodEntryWithParams("pizza", 1000, "2026-04-08");

            int total = getCaloriesByDate("2026-04-08");

            Assert::AreEqual(1000, total);
        }
    };
}