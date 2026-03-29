#include "BMI.h"
#include <stdio.h>
#include <stdlib.h>
void  log_weight() {
	float height_cm;
	float weight_kg;
	char date[BMI_DATE];

	//checking height for error handling
	if (height_cm <= 0) {
		printf("Error: Please enter your vaild height in cm");
		return; 
	}

	//checking space for array
	if()
}