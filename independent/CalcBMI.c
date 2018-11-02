/*
Calculates BMI

*/

#include "stdio.h"
#include "stdlib.h"

int main(void){
	
	// Defining the bool type
	typedef int bool;
	#define true 1;
	#define false 0;

	// If imperial or metric
	bool imperial = false;

	// Prompts user for units type
	char asw;
	printf("Imperial or metric? i/m ");
	scanf("%c", &asw);
	
	// Set imperial bool
	if(asw == 'i'){
		imperial = true;
		printf("Use inches and pounds.\n");
	} else {
		printf("Use centimeters and kilograms\n");
	}
	
	// Initialising users stats
	char name[20];
	int age;
	int height;
	int weight;

	// Prompting user for values
	printf("What is your name, age, height and weight?\n");
	scanf("%s", &name);
	scanf("%d", &age);
	scanf("%d", &height);
	scanf("%d", &weight);
	
	printf("\n%7s %s\n%7s %d\n%7s %d\n%7s %d\n", "Name:", name, "Age:", age, "Heigh:", height, "Weight:", weight);
	
	float meters;
	float kgs;
	
	// Converting units
	if(imperial){
		meters = 0.0254 * height;
		kgs = 0.453592 * weight;
	} else {
		meters = height/100.0;
		kgs = weight;
	}
	
	float bmi = kgs / meters / meters;
	
	printf ("\n%7s %.1f\n","BMI:", bmi);
	
	return 0;
}
