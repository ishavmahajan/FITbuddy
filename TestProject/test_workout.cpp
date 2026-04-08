#include "pch.h"
#include "CppUnitTest.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WorkoutModuleTests
{
    TEST_CLASS(WorkoutModuleTests)
    {
    public:

        // GOOD CASE: Verify standard calorie math
        TEST_METHOD(Test_CalculateCalories_Standard)
        {
            // default weight is 70kg
            // (8.0 MET * 3.5 * 70kg) / 200 * 10 mins = 98.0
            float result = calculate_calories_burned(3, 10);

            // Used a tolerance of 0.01 because of float precision
            Assert::AreEqual(98.00f, result, 0.01f);
        }

        // BOUNDARY CASE: Minimum intensity
        TEST_METHOD(Test_CalculateCalories_LowIntensity)
        {
            // (3.5 MET * 3.5 * 70kg) / 200 * 20 mins = 85.75
            float result = calculate_calories_burned(1, 20);

            Assert::AreEqual(85.75f, result, 0.01f);
        }

        // EDGE CASE: Invalid Intensity
        TEST_METHOD(Test_InvalidIntensity_ReturnsZero)
        {
            // Your logic returns 0 for invalid intensity
            float result = calculate_calories_burned(5, 10);

            Assert::AreEqual(0.0f, result);
        }

        // EDGE CASE: Zero Duration
        TEST_METHOD(Test_ZeroDuration_ReturnsZero)
        {
            float result = calculate_calories_burned(2, 0);

            Assert::AreEqual(0.0f, result);
        }

        // INTEGRATION CASE: Logic checking for weight change in BMI weight update feature
        TEST_METHOD(Test_CalorieChangeWithWeight)
        {
            // Manual check: If weight was 100kg
            // (8.0 * 3.5 * 100) / 200 * 10 = 140.0
            add_weight(100.0f, "2026-04-08", 180.0f);

            float result = calculate_calories_burned(3, 10);
            Assert::AreEqual(140.00f, result, 0.01f);
        }
    };
}