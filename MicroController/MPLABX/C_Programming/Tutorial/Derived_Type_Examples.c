#############    ARRAY   ###############
#include <stdio.h>

int main() {
    int numbers[5] = {10, 20, 30, 40, 50};  // Array declaration and initialization
    int i;

    // Accessing array elements
    for (i = 0; i < 5; i++) {
        printf("Element %d: %d\n", i, numbers[i]);
    }
    return 0;
}
#############    POINTER   ###############
#include <stdio.h>

int main() {
    int x = 10;
    int *ptr = &x;  // Pointer declaration and initialization

    printf("Value of x: %d\n", x);
    printf("Address of x: %p\n", ptr);    // Prints the address of x
    printf("Value at address: %d\n", *ptr); // Dereferencing the pointer

    return 0;
}
#############    STRUCTURE   ###############

#include <stdio.h>
#include <string.h>

struct Student {
    char name[50];
    int age;
    float grade;
};

int main() {
    struct Student s1;  // Declare a structure variable

    // Initializing structure members
    strcpy(s1.name, "Alice");
    s1.age = 20;
    s1.grade = 89.5;

    // Accessing structure members
    printf("Name: %s\n", s1.name);
    printf("Age: %d\n", s1.age);
    printf("Grade: %.2f\n", s1.grade);

    return 0;
}
#############    UNIONS   ###############

#include <stdio.h>
#include <string.h>

union Data {
    int i;
    float f;
    char str[20];
};

int main() {
    union Data data;  // Declare a union variable

    data.i = 10;       // Store an integer
    printf("Integer: %d\n", data.i);

    data.f = 220.5;    // Store a float (overwrites the integer)
    printf("Float: %f\n", data.f);

    strcpy(data.str, "C Programming"); // Store a string (overwrites the float)
    printf("String: %s\n", data.str);

    return 0;
}
#############    FUNCTIONS   ###############

#include <stdio.h>

// Function declaration
int add(int a, int b);

int main() {
    int result = add(5, 7);  // Function call
    printf("Sum: %d\n", result);
    return 0;
}

// Function definition
int add(int a, int b) {
    return a + b;
}
