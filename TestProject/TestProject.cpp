#include "pch.h"
#include "CppUnitTest.h"

extern "C" {
#include "../FITbuddy/BMI.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestProject
{
    TEST_CLASS(BMITests)
    {
    public:

        TEST_METHOD_INITIALIZE(ResetData)
        {
            entry_count = 0;
            current_bmi = 0.0f;

            for (int i = 0; i < MAX_ENTRIES; i++) {
                history[i].weight_kg = 0.0f;
                history[i].bmi = 0.0f;
                history[i].date[0] = '\0';
            }
        }

        TEST_METHOD(Test_CalculateBMI_ValidInput)
        {
            float bmi = calculate_bmi(70.0f, 170.0f);
            Assert::AreEqual(24.22f, bmi, 0.05f);
        }

        TEST_METHOD(Test_CalculateBMI_SecondCase)
        {
            float bmi = calculate_bmi(50.0f, 160.0f);
            Assert::AreEqual(19.53f, bmi, 0.05f);
        }

        TEST_METHOD(Test_GetBMICategory_Underweight)
        {
            const char* category = get_bmi_category(17.5f);
            Assert::AreEqual("Underweight", category);
        }

        TEST_METHOD(Test_GetBMICategory_NormalWeight)
        {
            const char* category = get_bmi_category(22.0f);
            Assert::AreEqual("Normal weight", category);
        }

        TEST_METHOD(Test_GetBMICategory_Overweight)
        {
            const char* category = get_bmi_category(27.0f);
            Assert::AreEqual("Overweight", category);
        }

        TEST_METHOD(Test_GetBMICategory_Obese)
        {
            const char* category = get_bmi_category(32.0f);
            Assert::AreEqual("Obese", category);
        }

        TEST_METHOD(Test_AddWeight_FirstEntry)
        {
            float bmi = add_weight(70.0f, "2026-04-01", 170.0f);

            Assert::AreEqual(1, entry_count);
            Assert::AreEqual(70.0f, history[0].weight_kg, 0.01f);
            Assert::AreEqual(bmi, history[0].bmi, 0.01f);
            Assert::AreEqual(bmi, current_bmi, 0.01f);
            Assert::AreEqual("2026-04-01", history[0].date);
        }

        TEST_METHOD(Test_AddWeight_SecondEntry)
        {
            add_weight(70.0f, "2026-04-01", 170.0f);
            float bmi2 = add_weight(72.0f, "2026-04-02", 170.0f);

            Assert::AreEqual(2, entry_count);
            Assert::AreEqual(72.0f, history[1].weight_kg, 0.01f);
            Assert::AreEqual(bmi2, history[1].bmi, 0.01f);
            Assert::AreEqual("2026-04-02", history[1].date);
            Assert::AreEqual(bmi2, current_bmi, 0.01f);
        }

        TEST_METHOD(Test_AddWeight_HistoryFull)
        {
            entry_count = MAX_ENTRIES;

            float result = add_weight(70.0f, "2026-04-01", 170.0f);

            Assert::AreEqual(-1.0f, result, 0.01f);
            Assert::AreEqual(MAX_ENTRIES, entry_count);
        }

        TEST_METHOD(Test_SetBMIGoal)
        {
            float goal_bmi = set_bmi_goal(65.0f, 170.0f);
            Assert::AreEqual(22.49f, goal_bmi, 0.05f);
        }

        TEST_METHOD(Test_GetIdealWeightRange)
        {
            float min_weight = 0.0f;
            float max_weight = 0.0f;

            get_ideal_weight_range(170.0f, &min_weight, &max_weight);

            Assert::AreEqual(53.46f, min_weight, 0.05f);
            Assert::AreEqual(71.96f, max_weight, 0.05f);
        }

        TEST_METHOD(Test_GetCurrentDate_Format)
        {
            char date[11];
            get_current_date(date, sizeof(date));

            Assert::AreEqual((size_t)10, strlen(date));
            Assert::AreEqual('-', date[4]);
            Assert::AreEqual('-', date[7]);
        }
    };
}