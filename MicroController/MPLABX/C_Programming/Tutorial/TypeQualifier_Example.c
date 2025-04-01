#############  VOLATILE - TYPE QUALIFIER ############# 
#############  Used for variables modified by ISRs or hardware.
######################################################
volatile unsigned char timer_flag = 0;

void interrupt ISR(void) {
    if (TMR0IF) {
        TMR0IF = 0;
        timer_flag = 1;  // Modified by ISR
    }
}
#############  CONSTANT - TYPE QUALIFIER #############
#############  Used as a read-only variable.
######################################################
const int MAX_STUDENTS = 50;

void printMax(void) {
    printf("Maximum Students Allowed: %d\n", MAX_STUDENTS);
}

int main() {
    printMax(); // Output: Maximum Students Allowed: 50
}


#############  STATIC - STORAGE CLASS SPECIFIER ############# 
#############  Retains value between function calls.
######################################################
void myFunction(void) {
    static int count = 0; // Retains value between function calls
    count++;
    printf("Count: %d\n", count);
}

int main() {
    myFunction(); // Output: Count: 1
    myFunction(); // Output: Count: 2
    myFunction(); // Output: Count: 3
}

#############  VOLATILE STATIC  ############# 
#############  Retains value but can be modified asynchronously.
######################################################
volatile static unsigned int counter = 0;

void interrupt ISR(void) {
    if (TMR0IF) {
        TMR0IF = 0;
        counter++;  // This value is persistent and can be modified asynchronously
    }
}
