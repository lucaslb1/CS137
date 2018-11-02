//Lucas Battelle
#include <stdio.h>
#include <stdlib.h>

int main(void){
	setvbuf(stdout, NULL, _IONBF, 0); // Turn off output buffering. Required for automated testing.
	
	char buffer[BUFSIZ+1]; // Space for the user to type in their age as a string of characters
	int ageInYears;        // Variable that will hold the user's age in numeric (integer) form
	int ageInDays;         // Will hold the user's age in days
	
	printf("Enter your age:\n");	// NOTE: The \n is required for automated testing.
	ageInYears = atoi(fgets(buffer, BUFSIZ, stdin));
	ageInDays = 365 * ageInYears;
	printf("%d years old is about %d days old.\n", ageInYears, ageInDays);
	
	return 0;
}