cmake_minimum_required (VERSION 3.16)

# This is the "Boss" file that connects everything
project ("IntegrationTesting" C)

enable_testing()

# 1. Include the subdirectory where the actual test logic lives
add_subdirectory ("IntegrationTesting")