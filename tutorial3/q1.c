#include <stdio.h>

int main() {
	// setting up the variables needed
	char first_name[256];
	int age;
	float height;

	// Getting names, age, and height in cm
	printf("Enter your first name: ");
	scanf("%s", first_name);
	printf("Enter your age: ");
	scanf("%d", &age);
	printf("Enter your height in cm: ");
	scanf("%f", &height);

	// Printing the information
	printf("Your name is %s, you are %d years old, and your height is %.2f cm.\n", first_name, age, height);

	return 0;
}