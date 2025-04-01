############################################################
Tutorials: Good tutorials: http://www.tutorialspoint.com/cprogramming/c_constants.htm
############################################################
Online C compiler: http://www.tutorialspoint.com/compile_c_online.php
no login needed. cannot save. Try this: >> gcc main.c -o demo -->a new demo file is generated. Just type >>demo to execute the program
############################################################
# LIST OF EXAMPLES: 
/*** BASIC HELLO WORLD***/
/*** Data Types ***/
/*** ARRAYS ***/
/*** ARRAYS AND STRINGS */
/*** ARRAYS AND STRINGS...MORE */
/*** NUMBER FORMAT */
/*** MULTI DIMENTSIONAL ARRAY*/
/*** USING FUNCTION*/
/*** USING FUNCTION AND SWAP*/
/*** LOOP RUNNING FOR EVER! ***/
/*** IF-STATEMENT ***/
/*** WHILE LOOP ***/
/*** NESTED LOOP ***/
/*** USING IF STATEMENT ***/
/*** Get an INPUT Character ***/
/*** Learn about different operators ***/
/*** Check to see if the user entered a DIGIT ***/
/*** USING SWITCH STATEMENT ***/
/*** USING SWITCH STATEMENT AND RANDOM NUMBERS***/
/*** Pointer with Argument***/
/*** Pointer Example***/



/*** BASIC HELLO WORLD***/
############################################################
#include <stdio.h>
int main()
{
printf("Hello, World!\n");

return 0;
}

/*** Data Types ***/
############################################################
#include <stdio.h>

int myinteger; /*just define an integer*/
float myfloat; /*just define a float*/
char mychar; /*Just define a character*/

int main()
{
myfloat = 1.0 / 2;
myinteger = 6.0/3;
mychar = 'Y';

printf("The value for my Integet is %d \n", myinteger);
printf("The value for my Float is %f \n", myfloat);

printf("The value for my Character is %c \n", mychar);

return 0;
}

/*** ARRAYS ***/
############################################################
#include <stdio.h>

float average; /*define a float*/
float data[5]; /*define an array:0,1,2,3,4*/
float total; /*define a float*/

main()
{

data[0] = 34.2;
data[1] = 23.1;
data[2] = 43.2;
data[3] = 1.2;
data[4] = 3.2;

/* We can also use this: float data[ ]={34.2, 23, 43, 12, 3};*/

total = data[0]+data[1]+data[2]+data[3]+data[4];
average = total / 5;
printf("The SUM is %f \n", total);
printf("The AVERAGE is %f \n", average);
return 0;
}

/*** ARRAYS AND STRINGS */
############################################################
#include <stdio.h>
#include <string.h> // note the header file

char name[4]; /*define an array*/
int size;
main()
{

name[0] = 'P';
name[1] = 'I';
name[2] = 'C';
name[3] = '\0'; // end of string (NUL)

size = strlen(name);
printf("The name is %s \n", name);
strcpy(name,"Sam");
printf("The name is %s \n", name);
printf("The size of name is %d \n", size);

return 0;
}

/*** ARRAYS AND STRINGS...MORE */
############################################################
#include <stdio.h>
#include <string.h> // note the header file

char name[4]; /*define an array*/
int disp [2][4]= {
{10, 11, 12, 13},
{14, 15, 16, 17}
};
int size;
main()
{

name[0] = 'P';
name[1] = 'I';
name[2] = 'C';
name[3] = '\0'; // end of string (NUL)

size = strlen(name); // string length
printf("The name is %s \n", name);
strcpy(name,"Sam");
printf("The name is %s \n", name);
printf("The size of name is %d \n", size);
strcat(name," & ARM"); // concatenate two strings
printf("The name is %s \n", name);

// Defining the two-dimenional array
// read more here: http://beginnersbook.com/2014/01/2d-arrays-in-c-example/

printf("The cell content %d \n", disp[0][1]);

return 0;
}

/*** NUMBER FORMAT */
############################################################
#include<stdio.h>
int x = 0x12;

main()
{
printf("The color: %s\n", "blue");
printf("First number: %d\n", 12345);
printf("Second number: %04d\n", 25);
printf("Third number: %i\n", 1234);
printf("Float number: %3.2f\n", 3.14159);
printf("Hexadecimal: %x\n", 255);
printf("Hexadecimal-----: %d\n", x);
printf("Octal: %o\n", 255);
printf("Unsigned value: %u\n", 150);
printf("Just print the percentage sign %%\n", 10);
}
 
/*** MULTI DIMENTSIONAL ARRAY*/
​############################################################
#include <stdio.h>
// Read this for more information: http://www.tutorialspoint.com/cprogramming/c_multi_dimensional_arrays.htm
// Read this also: https://www.eskimo.com/~scs/cclass/int/sx9.html
int main ()
{
/* an array with 5 rows and 2 columns*/
int a[5][2] = { {0,0}, {1,2}, {2,4}, {3,6},{4,8}};
int i, j;
int b[2][5] = {
{0, 1, 2, 3, 4} , /* initializers for row indexed by 0 */
{5, 6, 7, 8, 9} /* initializers for row indexed by 1 */
};

/* output each array element's value */
for ( i = 0; i < 5; i++ )
{
for ( j = 0; j < 2; j++ )
{
printf("a[%d][%d] = %d\n", i,j, a[i][j] );
}
}
return 0;
}

/*** USING FUNCTION*/
############################################################
#include<stdio.h>
// function prototype, also called function declaration
float square ( float x );

// main function, program starts from here
int main( )
{

float m, n ;
printf ( "\nEnter some number for finding square \n");
scanf ( "%f", &m ) ;
// function call
n = square ( m ) ;
printf ( "\nSquare of the given number %f is %f\n",m,n );

}

float square ( float x ) // function definition
{
float p ;
p = x * x ;
return ( p ) ;
}

/*** USING FUNCTION AND SWAP*/
############################################################
#include<stdio.h>
// function prototype, also called function declaration

//return_type function_name( parameter list );
// note *a refers to the address of a
void swap( int *a, int *b); 
int main()
{

int m = 22, n = 44;
// calling swap function by reference
printf("values before swap m = %d and n = %d \n",m,n);

/* calling a function to swap the values. (1) &a indicates pointer to m ie. address of variable m and (2) &b indicates pointer to n ie. address of variable n. */
swap(&m, &n);

}

void swap(int *a, int *b)
{
/* local variable declaration */
int tmp;
tmp = *a; /* save the value at address a */
*a = *b; /* put a into b */
*b = tmp; /* put temp into b */

printf("\n values after swap a = %d and b = %d \n", *a, *b);

}

/*** LOOP RUNNING FOR EVER! ***/
############################################################
#include <stdio.h>   
int main () 
{     
for( ; ; ) 
{       
printf("This loop will run forever.\n");    
}     
return 0; 
}
/*** FOR LOOP ***/
#include <stdio.h>
int main()
{
int num, count, sum = 0;

printf("Enter a positive integer: ");
scanf("%d", &num); // get an input

// for loop terminates when n is less than count
for(count = 1; count <= num; ++count)
{
sum += count;
}

printf("Sum = %d", sum);

return 0;
}

/*** IF-STATEMENT ***/
############################################################
#include <stdio.h>
int main()
{
int number=-2;

// Test expression is true if number is less than 0
if (number < 0)
{
printf("You entered %d.\n", number);
}

printf("The if statement is easy.");

return 0;
}


/*** WHILE LOOP ***/
############################################################
#include <stdio.h>   
int main () 
{     /* local variable definition */    
int a = 10;     
/* while loop execution */    
while( a < 20 ) 
{       
printf("value of a: %d\n", a);       
a++;    
}      
return 0; 
}

/*** NESTED LOOP ***/
############################################################
#include <stdio.h>   
int main () 
{     /* local variable definition */    
int i, j;        
for(i = 2; i<100; i++) {           
	for(j = 2; j <= (i/j); j++)           
		if(!(i%j)) break; 
		// if factor found, not prime          
		if(j > (i/j)) 
			printf("%d is prime\n", i);    		
}      
return 0; 
}
 
/*** USING IF STATEMENT ***/
############################################################
#include <stdio.h>
int iTemperature = 88;
int main()
{
if (iTemperature >= 80)
{ //Turn AC on
printf("\nThe AC is on\n");
}
else
{ //Turn AC off
printf("\nThe AC is off\n");
}
return 0;
}
/*** Get an INPUT ***/

#include <stdio.h>
int iTemperature = 88;
int main()
{
scanf("%d", &iTemperature);
//if (iTemperature == 22)
if (iTemperature >= 80)
{ //Turn AC on
printf("\nThe AC is on\n");
}
else
{ //Turn AC off
printf("\nThe AC is off\n");
}
return 0;
}

/*** Get an INPUT Character ***/
############################################################
#include <stdio.h>
int main()
{
char cResponse = '\0';
printf("\n\tAC Control Unit\n");
printf("\na\tTurn the AC on\n");
printf("b\tTurn the AC off\n");
printf("\nEnter your selection: ");
scanf("%c", &cResponse);
if (cResponse == 'a')
printf("\nAC is now on\n");
if (cResponse == 'b')

return 0;
}

/*** Learn about different operators ***/
############################################################
#include <stdio.h>
int main()
{
if ( 3 > 1 && 5 < 5 ) // and operator
printf("The entire expression is true\n");
else
printf("The entire expression is False \n");
if ( 3 > 5 || 6 < 5 )
printf("***The entire expression is true\n");
else
printf("***The entire expression is False \n");

//if ( cResponse == 'A' || 'a' ) // NOT CORRECT
//if ( cResponse == 'A' || == 'a' ) // NOT CORRECT

// Check for range of numbers
int iResponse = 0;
printf("Enter a number from 1 to 10: ");
scanf("%d", &iResponse);
if ( iResponse < 1 || iResponse > 10 )
printf("\nNumber not in range\n");
else
printf("\nThank you\n");

return 0;
}
/*** Check to see if the user entered a DIGIT ***/
############################################################
#include <stdio.h>
#include <ctype.h>
int main()
{
char cResponse = '\0';
printf("\nPlease enter a digit: ");
scanf("%c", &cResponse);
if isdigit(cResponse)
printf("\nThank you\n");
else
printf("\nYou did not enter a digit\n");

return 0;
}

/*** USING SWITCH STATEMENT ***/
############################################################
#include <stdio.h>
#include <ctype.h>
char cResponse = '\0';
int main()
{
scanf("%c", &cResponse);
switch (cResponse)
{ case 'a': case 'A':
printf("\nYou selected the character a or A\n");
break;
case 'b': case 'B':
printf("You selected the character b or B\n");
break;
case 'c': case 'C':
printf("You selected the character c or C\n");
break;

} //end switch

return 0;
}

/*** USING SWITCH STATEMENT AND RANDOM NUMBERS***/
############################################################
#include <stdio.h>
#include <ctype.h>
#include<time.h>

int main()
{
int iRandomNum = 0;

srand(time(NULL));
printf("\n Your REAL Random Number is %i", rand();
iRandomNum = (rand() % 4) + 1; // random number betwee 4 and 1
printf("\n Your Random Number is %i", iRandomNum);
printf("\nFortune Cookie - Chapter 3\n");
switch (iRandomNum)
{
case 1:
printf("\nYou will meet a new friend today.\n");
break;
case 2:
printf("\nYou will enjoy a long and happy life.\n");
break;
case 3:
printf("\nOpportunity knocks softly. Can you hear it?\n");
break;
case 4:
printf("\nYou'll be break; financially rewarded for your good deeds.\n");
}
printf("\nLucky lotto numbers: ");
printf("%d ", (rand() % 49) + 1); // number between 1 to 49
printf("%d ", (rand() % 49) + 1);
printf("%d ", (rand() % 49) + 1);
printf("%d ", (rand() % 49) + 1);
printf("%d\n", (rand() % 49) + 1);

return 0;
}

/*** Pointer with Argument***/
############################################################
// pointers as arguments:
#include <stdio.h>
void increment_all (int* start, int* stop)
{
int * current = start;
printf("Start & Stop Address: %X, %X , \n",start, stop);
while (current != stop) {
++(*current); // increment value pointed
++current; // increment pointer
}
}

void print_all (const int* start, const int* stop)
{
const int * current = start;
while (current != stop) {
printf("%d , \n",*current);
++current; // increment pointer
}
}

int main ()
{
int numbers[ ] = {10,20,30};
printf("Addresses %X, %X, %X , \n",numbers, numbers+1, numbers+2);
increment_all (numbers,numbers+3);
print_all (numbers,numbers+3);
return 0;
}

/*** Pointer Example***/
############################################################
// more pointers
#include <stdio.h>
int main ()
{
int n;
int numbers[5];
int * p;
p = numbers; *p = 10;
p++; *p = 20;
p = &numbers[2]; *p = 30;
p = numbers + 3; *p = 40;
p = numbers; *(p+4) = 50;
for (n=0; n<5; n++){
printf("%d , ", numbers[n]);}
return 0;
}

 
