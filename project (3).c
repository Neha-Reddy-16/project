#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#define N 9
#define UNASSIGNED 0
#include <string.h>
#include<conio.h>
#include <math.h>
#define PI  3.14159265358979323846
#define RED 0
#define BLACK 1
#define GREEN 2

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define EVEN 0
#define ODD 1

#define MIN_BET 1
#define MAX_BET 1000

bool isValidMove(int grid[N][N], int row, int col, int num);
bool findUnassignedLocation(int grid[N][N], int *row, int *col);
bool solveSudoku(int grid[N][N]);
void printGrid(int grid[N][N]);
void generateGrid(int grid[N][N]);
bool isValidSudoku(int grid[N][N]);
bool checkRow(int grid[N][N], int row, int num);
bool checkCol(int grid[N][N], int col, int num);
bool checkBox(int grid[N][N], int boxStartRow, int boxStartCol, int num);
void clearBuffer();
void printErrorMessage(char *message);

int sudokuGame() {
int grid[N][N];
generateGrid(grid);
printf("Sudoku Puzzle:\n");
printGrid(grid);

printf("\nTimer:\n");
clock_t startTime = clock();
while (true) 
{
    clock_t currentTime = clock();
    double elapsedTime = (double)(currentTime - startTime) / CLOCKS_PER_SEC;
    printf("Time elapsed: %.0f seconds\n", elapsedTime);
    //usleep(1000000); // Sleep for 1 second

    if (elapsedTime >= 1800) 
    { // Time limit: 30 minutes
        printf("Time's up! Game aborted.\n");
        break;
    }

    int row, col, num;
    printf("Enter row, column, and number (1-9) separated by spaces (or 0 0 0 to quit): ");
    if (scanf("%d %d %d", &row, &col, &num) != 3) 
    {
        printf("Invalid input. Please enter again.\n");
        clearBuffer();
        continue;
    }

    if (row == 0 || col == 0 || num == 0)
    {
        printf("Game aborted.\n");
        break;
    }

    if (row < 1 || row > N || col < 1 || col > N || num < 1 || num > 9) 
    {
        printf("Invalid input. Row, column, and number must be between 1 and 9.\n");
        continue;
    }

    if (!isValidMove(grid, row - 1, col - 1, num)) 
    {
        printErrorMessage("Invalid move. Please try again.");
        continue;
    }

    grid[row - 1][col - 1] = num;
    printGrid(grid);

    if (isValidSudoku(grid)) 
    {
        printf("\nCongratulations! You solved the puzzle.\n");
        break;
    }
}

return 0;
}

void generateGrid(int grid[N][N]) 
{
srand(time(NULL)); // Seed for random number generation

// Start with an empty grid
for (int i = 0; i < N; i++) 
{
    for (int j = 0; j < N; j++) 
    {
        grid[i][j] = UNASSIGNED;
    }
}

// Generate a random Sudoku puzzle
solveSudoku(grid);

// Remove some numbers to create the puzzle
int removeCount = N * N / 2; // Adjust the difficulty by changing the number of cells removed
while (removeCount > 0) 
{
    int row = rand() % N;
    int col = rand() % N;
    if (grid[row][col] != UNASSIGNED) 
    {
        grid[row][col] = UNASSIGNED;
        removeCount--;
    }
}
}

void printGrid(int grid[N][N]) 
{
printf("\n");
for (int row = 0; row < N; row++) 
{
    if (row % 3 == 0 && row != 0) 
    {
        printf("-------------------------\n");
    }
    for (int col = 0; col < N; col++) 
    {
        if (col % 3 == 0 && col != 0) 
        {
            printf("| ");
        }
        if (grid[row][col] == UNASSIGNED) {
            printf(". ");
        } else {
            printf("%d ", grid[row][col]);
        }
    }
    printf("\n");
}
}

bool solveSudoku(int grid[N][N]) {
int row, col;

// If there is no unassigned location, we are done
if (!findUnassignedLocation(grid, &row, &col))
    return true; // success!

// Consider digits 1 to 9
for (int num = 1; num <= 9; num++) {
    // If placing the current number in the current location is safe
    if (isValidMove(grid, row, col, num)) {
        // Make tentative assignment
        grid[row][col] = num;

        // Return if success, otherwise undo the assignment
        if (solveSudoku(grid))
            return true;

        grid[row][col] = UNASSIGNED; // undo the assignment
    }
}
return false; // This triggers backtracking
}

bool findUnassignedLocation(int grid[N][N], int *row, int *col) {
for (*row = 0; *row < N; (*row)++) {
    for (*col = 0; *col < N; (*col)++) {
        if (grid[*row][*col] == UNASSIGNED) {
            return true;
        }
    }
}
return false;
}

bool isValidMove(int grid[N][N], int row, int col, int num) {
return !checkRow(grid, row, num) &&
        !checkCol(grid, col, num) &&
        !checkBox(grid, row - row % 3, col - col % 3, num);
}

bool isValidSudoku(int grid[N][N]) 
{
// Check rows, columns, and 3x3 boxes
for (int i = 0; i < N; i++) 
{
    if (!checkRow(grid, i, 1) || !checkRow(grid, i, 2) || !checkRow(grid, i, 3) ||
        !checkRow(grid, i, 4) || !checkRow(grid, i, 5) || !checkRow(grid, i, 6) ||
        !checkRow(grid, i, 7) || !checkRow(grid, i, 8) || !checkRow(grid, i, 9) ||
        !checkCol(grid, i, 1) || !checkCol(grid, i, 2) || !checkCol(grid, i, 3) ||
        !checkCol(grid, i, 4) || !checkCol(grid, i, 5) || !checkCol(grid, i, 6) ||
        !checkCol(grid, i, 7) || !checkCol(grid, i, 8) || !checkCol(grid, i, 9) ||
        !checkBox(grid, i - i % 3, (i % 3) * 3, 1) || !checkBox(grid, i - i % 3, (i % 3) * 3, 2) ||
        !checkBox(grid, i - i % 3, (i % 3) * 3, 3) || !checkBox(grid, i - i % 3, (i % 3) * 3, 4) ||
        !checkBox(grid, i - i % 3, (i % 3) * 3, 5) || !checkBox(grid, i - i % 3, (i % 3) * 3, 6) ||
        !checkBox(grid, i - i % 3, (i % 3) * 3, 7) || !checkBox(grid, i - i % 3, (i % 3) * 3, 8) ||
        !checkBox(grid, i - i % 3, (i % 3) * 3, 9)) 
    {
        return false;
    }
}
return true;
}

bool checkRow(int grid[N][N], int row, int num) 
{
for (int col = 0; col < N; col++) 
{
    if (grid[row][col] == num) 
    {
        return true;
    }
}
return false;
}

bool checkCol(int grid[N][N], int col, int num) 
{
for (int row = 0; row < N; row++) 
{
    if (grid[row][col] == num) 
    {
        return true;
    }
}
return false;
}

bool checkBox(int grid[N][N], int boxStartRow, int boxStartCol, int num) 
{
for (int row = 0; row < 3; row++) 
{
    for (int col = 0; col < 3; col++) 
    {
        if (grid[row + boxStartRow][col + boxStartCol] == num) 
        {
            return true;
        }
    }
}
return false;
}

void clearBuffer() {
while (getchar() != '\n');
}

void printErrorMessage(char *message) {
printf("Error: %s\n", message);
}





int casinoGame()
{
int balance = 1000;
int betAmount;
int betType;
int betNumber;
char playAgain = 'y';

srand(time(NULL));

printf("Welcome to Roulette!\n");
printf("You start with a balance of $1000.\n");

while (playAgain == 'y' || playAgain == 'Y')
{
    printf("\nYour current balance is: $%d\n", balance);

    // Asking player to place a bet
    printf("Place your bet (minimum $%d, maximum $%d): ", MIN_BET, MAX_BET);
    scanf("%d", &betAmount);

    // Check if the bet is valid
    if (betAmount < MIN_BET || betAmount > MAX_BET || betAmount > balance)
    {
        printf("Invalid bet amount.\n");
        continue;
    }

    // Asking player for the bet type
    printf("Choose your bet type:\n");
    printf("1. Single Number(payout 35:1)\n");
    printf("2. Even/Odd(payout 1:1)\n");
    printf("3. Red/Black(payout 1:1)\n");
    printf("4. Low/High (1-18)/(19-36)(payout 2:1)\n");
    printf("Enter your choice: ");
    scanf("%d", &betType);

    // chose bet type
    switch (betType)
    {
    case 1:
        printf("Enter the number you want to bet on (0-36): ");
        scanf("%d", &betNumber);
        if (betNumber < 0 || betNumber > 36)
        {
            printf("Invalid number.\n");
            continue;
        }
        break;
    case 2:
        printf("Choose even (0) or odd (1): ");
        scanf("%d", &betNumber);
        if (betNumber != EVEN && betNumber != ODD)
        {
            printf("Invalid choice.\n");
            continue;
        }
        break;
    case 3:
        printf("Choose red (0) or black (1): ");
        scanf("%d", &betNumber);
        if (betNumber != RED && betNumber != BLACK)
        {
            printf("Invalid choice.\n");
            continue;
        }
        break;
    case 4:
        printf("Choose low (1-18) (0) or high (19-36) (1): ");
        scanf("%d", &betNumber);
        if (betNumber != 0 && betNumber != 1)
        {
            printf("Invalid choice.\n");
            continue;
        }
        break;

    default:
        printf("Invalid choice.\n");
        continue;
    }

    // the roulette spin
    int winningNumber = rand() % 37; // Numbers 0 to 36
    int winningColor;
    if (winningNumber == 0)
    {
        winningColor = GREEN;
    }
    else if (winningNumber % 2 == 0)
    {
        winningColor = RED;
    }
    else
    {
        winningColor = BLACK;
    }

    //  winning number and color
    printf("\nThe winning number is: %d ", winningNumber);
    if (winningColor == RED)
    {
        printf("(Red)\n");
    }
    else if (winningColor == BLACK)
    {
        printf("(Black)\n");
    }
    else
    {
        printf("(Green)\n");
    }
    int Payout = 0;

    if (betType == 1 && betNumber == winningNumber)
    {
        printf("Congratulations! You guessed the number correctly.\n");
        Payout = 35; // Payout is 35:1
    }
    else if (betType == 2 && betNumber == winningColor)
    {
        printf("Congratulations! You guessed the even/odd correctly.\n");
        Payout = 1;
    }
    else if (betType == 3 && betNumber == winningColor)
    {
        printf("Congratulations! You guessed the color correctly.\n");
        Payout = 1;
    }
    else if ((betType == 4 && betNumber == 0 && winningNumber >= 1 && winningNumber <= 18) || (betType == 4 && betNumber == 1 && winningNumber >= 19 && winningNumber <= 36))
    {
        printf("Congratulations! You guessed the number correctly.\n");
        Payout = 2; // Payout is 2:1
    }

    else
    {
        printf("Sorry, you lost.\n\n");
        Payout = -1;
    }
    balance += betAmount * Payout;
    printf("Your current balance is : $%d\n", balance);
    // Check if the player has run out of money
    if (balance <= 0)
    {
        printf("\nYou are out of money. Game over!\n");
        break;
    }
    if (Payout > 0)
    {
        printf("Congratulations! You won with a payout of %d:1\n", Payout);
    }
    // Ask if the player wants to play again
    printf("\nDo you want to bet again? (y/n): ");
    scanf(" %c", &playAgain);
}

printf("\nThank you for playing!\n");
return 0;
}




//function for adding n numbers
int addition(){
int n;
float sum = 0.0;

// asking the user to enter the number of elements
printf("Enter the number of elements you want to add: ");
if (scanf("%d", &n) != 1 || n <= 0) {
    printf("Invalid input. Please enter a positive integer.\n");
    return 1;    // Exiting with error code 1
}

// putting numbers from the user and calculating its sum
printf("Enter %d numbers:\n", n);
for (int i = 0; i< n; i=i+1) {
    float num;
    printf("Enter number %d: ", i+1);
    if (scanf("%f", &num) != 1) { 
        printf("Invalid input. Please enter a valid number.\n");
        return 1; // Exiting with error code 1
    }
    sum= sum + num;
}

// Displaying the result
printf("The sum of the %d numbers is: %.2lf\n", n, sum);

return 0; // Exiting normally
}

//function for subtacting two numbers
void subtraction(){
//Input both numbers and calculating its difference
printf("Enter the numbers you want to subtract: ");
float a,b;
scanf("%f%f",&a,&b);
float sub= a-b;
// Displaying the result
printf(" subtraction= %f\n",sub);
}

// function for computing multipliction of two numbers
void multiplication(){
//Input both numbers and calculating its product
printf("Enter the numbers you want to multiply: ");
float a,b;
scanf("%f%f",&a,&b);
// Displaying the result
printf(" multiplication = %f\n",a*b);

}
// function for computing division of two numbers
void division(){
//Input both numbers and calculating its division  
printf("Enter the numbers you want to divide: ");
float a,b;
scanf("%f%f",&a,&b);
float div=a/b;
// Displaying the result
printf(" division = %f\n",div);
}

// function for computing modulus of two numbers
void modulus(){
//Input both numbers and calculating its division  
printf("Enter the numbers you want to find modulus of: ");
int a,b;
scanf("%d%d",&a,&b);
int mod=(a%b);
// Displaying the result
printf("The modulus of a and b is %d\n",mod);
}

// function for computing factorial of a number
void factorial(){
int n,factorial;
//Input your number for calculating its factorial 
printf("Enter the number you want the factorial of: ");
scanf("%d",&n);
factorial=1;
for(int i=1;i<=n;i++){
factorial=factorial*i;
}
    // Displaying the result  
printf("Factorial of %d is %d\n",n,factorial);
}

// function for computing factorial of a number
void power(){
float b;
float p;
//Input your base and power
printf("Enter the base and the power: ");
scanf("%f%f",&b,&p);
float e=pow(b,p);
// Displaying the result
printf("The power is %f\n",e);
}

// function for computing square of a number
void square(){
float b;
//Input your number for calculating its factorial 
printf("Enter the number you want the square of: ");
scanf("%f",&b);
float p=(b*b);
// Displaying the result
printf("The square of %f is %f",b,p);
}

// function for computing cube of a number
void cube(){
float b;
//Input your number for calculating its cube 
printf("Enter the number you want the cube of: ");
scanf("%f",&b);
float p=(b*b*b);
// Displaying the result
printf("The cube of %f is %f",b,p);
}

// function for computing square root of a number
void squareroot(){
float b;
//Input your number for calculating its square root
printf("Enter the number you want the square root of : ");
scanf("%f",&b);
float s = sqrt(b);
// Displaying the result
printf("The square root of %f is %f",b,s);
}

void ln(){
float x, log_x;
// Taking input from the user
printf("Enter a number you want ln of : ");
scanf("%f", &x);
// Calculating logarithm ln()
log_x = log(x);
// Displaying the result
printf("Natural logarithm of %f = %f\n", x, log_x);


}

// function for computing sin value of a number
void SIN(){
float angle_degrees;
//input your angle in degrees
printf("enter angle:");
scanf("%f",&angle_degrees);
// Convert angle from degrees to radians
float angle_radians = angle_degrees*(PI / 180.0);

// Calculate sine value
float sin_value = sin(angle_radians);
// Displaying the result
printf("Sine of %f degrees is: %f\n", angle_degrees, sin_value);


}


// function for computing cos value of a number
void COS(){
float angle_degrees;
//input your angle in degrees
printf("enter angle:");
scanf("%f",&angle_degrees);
// Convert angle from degrees to radians
float angle_radians = angle_degrees*(PI / 180.0);

// Calculate cosine value
float cos_value = cos(angle_radians);
// Displaying the result
printf("cosine of %f degrees is: %f\n", angle_degrees, cos_value);


}

// function for computing tan value of a number
void TAN(){
float angle_degrees;
//input your angle in degrees
printf("enter angle:");
scanf("%f",&angle_degrees);
// Convert angle from degrees to radians
float angle_radians = angle_degrees*(PI / 180.0);
// Displaying the result
// Calculate tan value
float tan_value = tan(angle_radians);

printf("tan of %f degrees is: %f\n", angle_degrees, tan_value);

}

int CalculatorSc(){

printf("\t\t scientific calculator!!\n\n");
//providing choices to user to perform from given operations
int choice;
printf(" 0 : Exit \n");
printf(" 1 : Addition \n");
printf(" 2 : Subtraction \n");
printf(" 3 : Multiplication \n");
printf(" 4 : Division \n");
printf(" 5 : Modulus\n");
printf(" 6 : Power \n");
printf(" 7 : Factorial \n");
printf(" 8 : square \n");
printf(" 9 : cube \n");
printf(" 10 : squareroot\n");
printf(" 11 : natural log(ln)\n");
printf(" 12 : sin()\n");
printf(" 13 : cos()\n");
printf(" 14 : tan()\n");

while(1){
//asking user to choose an operation
printf("\n Choose operator: ");
scanf("%d",&choice);

switch(choice)
{
case 1:
//addition operation
addition();      //calling addition function
break;

case 2:
//subtraction operation
subtraction();           //calling subtraction function
break;

case 3:
//multiplication operation
multiplication();       //calling multiplication function
break;

case 4:
//division operation
division();             //calling division function
break;

case 5:
//modulus operation
modulus();              //calling modulus function
break;

case 6:
//power operation
power();                //calling power function
break;

case 7:
//factorial operation
factorial();            //calling factorial function
break;

case 8:
//square operation
square();
break;                  //calling square function

case 9:
//cube operation
cube();                 //calling cube function
break;

case 10:
//squareroot operation
squareroot();           //calling squareroot function
break;

case 11:
//ln operation
ln();           //calling ln function
break;

case 12: 
//sine operation
SIN();                  //calling sin function
break;

case 13:
//cosine operation
COS();                  //calling cos function
break;

case 14:
//tan operation
TAN();                  //calling tan function
break;

case 0:
exit(0);                //exit 

default:
printf("!! please enter valid operation");

}
}
return 0;
}




// Structure to store element information
struct Element {
char name[20];
double mass;
int atomic_number;
char configuration[20];
};

// Function to display element information
void displayElement(struct Element elements[], int num_elements, int atomic_number) {
for (int i = 0; i < num_elements; i++) {
    if (elements[i].atomic_number == atomic_number) {
        printf("Name: %s\n", elements[i].name);
        printf("Mass: %.2f\n", elements[i].mass);
        printf("Atomic Number: %d\n", elements[i].atomic_number);
        printf("Electronic Configuration: %s\n", elements[i].configuration);
        return;
    }
}
printf("Element with atomic number %d not found.\n", atomic_number);
}

int PeriodicTable() {
// Array of elements
struct Element elements[] = {
    {"Hydrogen", 1.008, 1, "1s1"},
    {"Helium", 4.0026, 2, "1s2"},
    {"Lithium", 6.94, 3, "[He] 2s1"},
    {"Beryllium", 9.0122, 4, "[He] 2s2"},
    {"Boron", 10.81, 5, "[He] 2s2 2p1"},
    {"Carbon", 12.011, 6, "[He] 2s2 2p2"},
    {"Nitrogen", 14.007, 7, "[He] 2s2 2p3"},
    {"Oxygen", 15.999, 8, "[He] 2s2 2p4"},
    {"Fluorine", 18.998, 9, "[He] 2s2 2p5"},
    {"Neon", 20.180, 10, "[He] 2s2 2p6"},
    {"Sodium", 22.990, 11, "[Ne] 3s1"},
    {"Magnesium", 24.305, 12, "[Ne] 3s2"},
    {"Aluminum", 26.982, 13, "[Ne] 3s2 3p1"},
    {"Silicon", 28.085, 14, "[Ne] 3s2 3p2"},
    {"Phosphorus", 30.974, 15, "[Ne] 3s2 3p3"},
    {"Sulfur", 32.06, 16, "[Ne] 3s2 3p4"},
    {"Chlorine", 35.45, 17, "[Ne] 3s2 3p5"},
    {"Argon", 39.948, 18, "[Ne] 3s2 3p6"},
    {"Potassium", 39.098, 19, "[Ar] 4s1"},
    {"Calcium", 40.078, 20, "[Ar] 4s2"},
    {"Scandium", 44.956, 21, "[Ar] 3d1 4s2"},
    {"Titanium", 47.867, 22, "[Ar] 3d2 4s2"},
    {"Vanadium", 50.942, 23, "[Ar] 3d3 4s2"},
    {"Chromium", 51.996, 24, "[Ar] 3d5 4s1"},
    {"Manganese", 54.938, 25, "[Ar] 3d5 4s2"},
    {"Iron", 55.845, 26, "[Ar] 3d6 4s2"},
    {"Cobalt", 58.933, 27, "[Ar] 3d7 4s2"},
    {"Nickel", 58.693, 28, "[Ar] 3d8 4s2"},
    {"Copper", 63.546, 29, "[Ar] 3d10 4s1"},
    {"Zinc", 65.38, 30, "[Ar] 3d10 4s2"},
    {"Gallium", 69.723, 31, "[Ar] 3d10 4s2 4p1"},
    {"Germanium", 72.63, 32, "[Ar] 3d10 4s2 4p2"},
    {"Arsenic", 74.922, 33, "[Ar] 3d10 4s2 4p3"},
    {"Selenium", 78.971, 34, "[Ar] 3d10 4s2 4p4"},
    {"Bromine", 79.904, 35, "[Ar] 3d10 4s2 4p5"},
    {"Krypton", 83.798, 36, "[Ar] 3d10 4s2 4p6"},
    {"Rubidium", 85.468, 37, "[Kr] 5s1"},
    {"Strontium", 87.62, 38, "[Kr] 5s2"},
    {"Yttrium", 88.906, 39, "[Kr] 4d1 5s2"},
    {"Zirconium", 91.224, 40, "[Kr] 4d2 5s2"},
    {"Niobium", 92.906, 41, "[Kr] 4d4 5s1"},
    {"Molybdenum", 95.95, 42, "[Kr] 4d5 5s1"},
    {"Technetium", 98, 43, "[Kr] 4d5 5s2"},
    {"Ruthenium", 101.07, 44, "[Kr] 4d7 5s1"},
    {"Rhodium", 102.91, 45, "[Kr] 4d8 5s1"},
    {"Palladium", 106.42, 46, "[Kr] 4d10"},
    {"Silver", 107.87, 47, "[Kr] 4d10 5s1"},
    {"Cadmium", 112.41, 48, "[Kr] 4d10 5s2"},
    {"Indium", 114.82, 49, "[Kr] 4d10 5s2 5p1"},
    {"Tin", 118.71, 50, "[Kr] 4d10 5s2 5p2"},
    {"Gallium", 69.723, 31, "[Ar] 3d10 4s2 4p1"},
    {"Germanium", 72.63, 32, "[Ar] 3d10 4s2 4p2"},
    {"Arsenic", 74.922, 33, "[Ar] 3d10 4s2 4p3"},
    {"Selenium", 78.971, 34, "[Ar] 3d10 4s2 4p4"},
    {"Bromine", 79.904, 35, "[Ar] 3d10 4s2 4p5"},
    {"Krypton", 83.798, 36, "[Ar] 3d10 4s2 4p6"},
    {"Rubidium", 85.468, 37, "[Kr] 5s1"},
    {"Strontium", 87.62, 38, "[Kr] 5s2"},
    {"Yttrium", 88.906, 39, "[Kr] 4d1 5s2"},
    {"Zirconium", 91.224, 40, "[Kr] 4d2 5s2"},
    {"Niobium", 92.906, 41, "[Kr] 4d4 5s1"},
    {"Molybdenum", 95.95, 42, "[Kr] 4d5 5s1"},
    {"Technetium", 98, 43, "[Kr] 4d5 5s2"},
    {"Ruthenium", 101.07, 44, "[Kr] 4d7 5s1"},
    {"Rhodium", 102.91, 45, "[Kr] 4d8 5s1"},
    {"Palladium", 106.42, 46, "[Kr] 4d10"},
    {"Silver", 107.87, 47, "[Kr] 4d10 5s1"},
    {"Cadmium", 112.41, 48, "[Kr] 4d10 5s2"},
    {"Indium", 114.82, 49, "[Kr] 4d10 5s2 5p1"},
    {"Tin", 118.71, 50, "[Kr] 4d10 5s2 5p2"},
    
    

};

int num_elements = sizeof(elements) / sizeof(elements[0]);
int atomic_number;
char choice;

do {
    printf("Enter the atomic number of the element: ");
    scanf("%d", &atomic_number);

    // Display element information
    displayElement(elements, num_elements, atomic_number);

    printf("Do you want to continue (Y/N)? ");
    scanf(" %c", &choice); // Note the space before %c to consume the newline character

} while (choice == 'Y' || choice == 'y');

return 0;
}

int Same_Scale(float TEMPEATURE,int a)
{
switch(a)
{
    case 1:printf("the temperature is %fdegree in Celsius",TEMPEATURE);
    break;
    case 2:printf("the temperature is %fdegree in Kelvin",TEMPEATURE);
    break;
    case 3:printf("the temperature is %f degree in Reaumur",TEMPEATURE);
    break;
    case 4:printf("the temperature is %f degree in Fahrenheit",TEMPEATURE);
    break;
    default:printf("\n");
}
}
void Celsius_to_Kelvin(float TEMPEATURE)
{
float C,K;
C=TEMPEATURE;
K=C+273.15;
//Conversion of tempearture from Celsius to Kelvin
printf(" %f degree in Celsius= %f degree in Kelvin",C,K);
}
void Celsius_to_Reaumur(float TEMPEATURE)
{
float C,R;
C=TEMPEATURE;
R=(4*C)/5;
//Conversion of tempearture from Celsius to Reaumur
printf("%f degree in Celsius= %f degree in Reaumur",C,R);
}
void Celsius_to_Fahrenheit(float TEMPEATURE)
{
float C,F;
C=TEMPEATURE;
F=(9*C)/5+32;
//Conversion of tempearture from Celsius to Fahrenheit 
printf("%f degree in celsius= %f degree in Fahrenheit",C,F);
}
void Kelvin_to_Celsius(float TEMPEATURE)
{
float K,C;
K=TEMPEATURE;
C=K-273.15;
//Conversion of tempearture from Kelvin to Celsius
printf("%f degree in Kelvin= %f degree in Celsius",K,C);
}
void Kelvin_to_Reaumur(float TEMPEATURE)
{
float K,R;
K=TEMPEATURE;
R=(4*(K-273.15))/5;
//Conversion of tempearture from Kelvin to Reaumur
printf("%f degree in kelvin= %f degree in Reaumur",K,R);
}
void Kelvin_to_Fahrenheit( float TEMPEATURE)
{
float K,F;
K=TEMPEATURE;
F=(9*(K-273.15))/5+32;
//Conversion of tempearture from Kelvin to Fahrenheit 
printf("%f degree in Kelvin= %f degree in Fahrenheit",K,F);
}
void Reaumur_to_Celsius(float TEMPEATURE)
{
float R,C;
R=TEMPEATURE;
C=(5*R)/4;
//Conversion of tempearture from Reaumur to Celsius
printf("%f degree in Reaumur= %f degree in Celsius",R,C);
}
void Reaumur_to_kelvin(float TEMPEATURE)
{
float R,K;
R=TEMPEATURE;
K=(5*R)/4+273.15;
//Conversion of tempearture from Reaumur to Kelvin
printf("%f degree in Reaumer= %f degree in kelvin",R,K);
}
void Reaumur_to_Fahrenheit(float TEMPEATURE)
{
float R,F;
R=TEMPEATURE;
F=(9*R)/4+32;
//Conversion of tempearture from Reaumur to Fahrenheit
printf("%f degree in Reaumur= %f degree in Fahrenheit",R,F);
}
void Fahrenheit_to_Celsius(float TEMPEATURE)
{
float F,C;
F=TEMPEATURE;
C=(5*(F-32))/9;
//Conversion of tempearture from Fahrenheit to Celsius
printf("%f degree in Fahrenheit= %f degree in celsius",F,C);
}
void Fahrenheit_to_kelvin(float TEMPEATURE)
{
float F,K;
F=TEMPEATURE;
K=(5*(F-32))/9+273.15;
//Conversion of tempearture from Fahrenheit to Kelvin
printf("%f degree in Fahrenheit= %f degree in Kelvin",F,K);
}
void Fahrenheit_to_Reaumur(float TEMPEATURE)
{
float F,R;
F=TEMPEATURE;
R=(4*(F-32))/9; 
//Conversion of tempearture from Fahrenheit to Reaumur
printf("%f degree in Fahrenheit= %f degree in Reaumur",F,R);
}
int TempConvertor()
{   int COMMAND;
int I_TEMP_SCALE,F_TEMP_SCALE;
float TEMP_READING;
printf("\nTemperature scales conversion");
printf("\nTemperature scale notations are:\n1:Celsius\n2:Kelvin\n3:Reaumur\n4:Fahrenheit");
while(1)
{   printf("\nEnter '1' to continue and '0' to stop");
//asking user to whether he wants to continue or not for more changes
    scanf("%d",&COMMAND);
    while(COMMAND)
    {   printf("\nselect the temperature scale from:");
        scanf("%d",&I_TEMP_SCALE);
        printf("\nenter the temperature");
        scanf("%f",&TEMP_READING);
        printf("\nselect the temperature scale in which it needs to be converted:");
        scanf("%d",&F_TEMP_SCALE);
        if(I_TEMP_SCALE==F_TEMP_SCALE){
            Same_Scale(TEMP_READING,I_TEMP_SCALE);
            //calling Same_Scale function
            break;
        }
        if(I_TEMP_SCALE==1&&F_TEMP_SCALE==2){
            Celsius_to_Kelvin(TEMP_READING);
            //calling Celsius_to_Kelvin function
            break;
        }
        if(I_TEMP_SCALE==1&&F_TEMP_SCALE==3){
            Celsius_to_Reaumur(TEMP_READING);
            //calling Celsius_to_Reaumur function
            break;
        }
        if(I_TEMP_SCALE==1&&F_TEMP_SCALE==4){
            Celsius_to_Fahrenheit(TEMP_READING);
            //calling Celsius_to_Fahrenheit function
            break;
        }
        if(I_TEMP_SCALE==2&&F_TEMP_SCALE==1){
            Kelvin_to_Celsius(TEMP_READING);
            //calling Kelvin_to_Celsius function
            break;
        }
        if(I_TEMP_SCALE==2&&F_TEMP_SCALE==3){
            Kelvin_to_Reaumur(TEMP_READING);
            //calling Kelvin_to_Reaumur function
            break;
        }
        if(I_TEMP_SCALE==2&&F_TEMP_SCALE==4){
            Kelvin_to_Fahrenheit(TEMP_READING);
            //calling Kelvin_to_Fahrenheit function
            break;
        }
        if(I_TEMP_SCALE==3&&F_TEMP_SCALE==1){
            Reaumur_to_Celsius(TEMP_READING);
            //calling Reaumur_to_Celsius function
            break;
        }
        if(I_TEMP_SCALE==3&&F_TEMP_SCALE==2){
            Reaumur_to_kelvin(TEMP_READING);
            //calling Reaumur_to_kelvin function
            break;
        }
        if(I_TEMP_SCALE==3&&F_TEMP_SCALE==4){
            Reaumur_to_Fahrenheit(TEMP_READING);
            //calling Reaumur_to_Fahrenheit function
            break;
        }
        if(I_TEMP_SCALE==4&&F_TEMP_SCALE==1){
            Fahrenheit_to_Celsius(TEMP_READING);
            //calling Fahrenheit_to_Celsius function
            break;
        }
        if(I_TEMP_SCALE==4&&F_TEMP_SCALE==2){
            Fahrenheit_to_kelvin(TEMP_READING);
            //calling Fahrenheit_to_kelvin function
            break;
        }
        if(I_TEMP_SCALE==4&&F_TEMP_SCALE==3){
            Fahrenheit_to_Reaumur(TEMP_READING);
            //calling Fahrenheit_to_Reaumur function
            break;
        }
    }
    if(COMMAND==0)
    break;
}
}


void upi_payment(int charges){
char vcode[6];
char vpa[15];
int c;
printf("Enter VPA ID : ");
scanf("%s",vpa);
printf("\n Enter Verification Code : ");
scanf("%s",vcode);
printf("\n Payment Succesfull \n");
//  print_bill();
}

void netbanking(int charges){
char  accno[20] ;
printf("\n Net Banking Payment\n");
printf("Account Number : ");
scanf("%s",accno);
printf("\n Transaction Id :  3789654321\n");
printf("\n Payment Successful.\n");
//  print_bill();
}

void debitcard(int charges) {
char nameOnCard[30];
long cardNumber;
char expiryDate[5];
printf("\n Debit Card Payment\n");
printf("Name On The Card : ");
gets(nameOnCard);
printf("Card Number : ");
scanf("%ld",&cardNumber);
printf("Expiry Date (MM/YY) : ");
scanf("%s",expiryDate);
if ((strlen(expiryDate) != 4) ) {
    printf("\n Invalid Expiry Date Format! Please enter in format MM/YY.");
    debitcard(charges);
} else{
    printf("\n Payment Successful.\n");
    //  print_bill();
}
}

void creditcard(int charges){
char nameOnCard[30];
long cardNumber;
char expiryDate[5];
printf("\n Credit Card Payment\n");
printf("Name On The Card : ");
gets(nameOnCard);
printf("Card Number : ");
scanf("%ld",&cardNumber);
printf("Expiry Date (MM/YY) : ");
scanf("%s",expiryDate);
if ((strlen(expiryDate) != 4) ) {
    printf("\n Invalid Expiry Date Format! Please enter in format MM/YY.");
    creditcard(charges);
} else{
    printf("\n Payment Successful.\n");
    //   print_bill();
}
}



void payment_gateway(int charges){
system("cls");

int choice;

printf("\n\n Amount to be  paid is Rs. %d \n",charges);
printf("\n\n Select Payment Method: ");
printf("\n 1. UPI Payment \n 2. Net Banking/E-Wallet \n 3. Debit Card \n 4. Credit Card\n");
printf("Enter Your  Choice : ");
scanf("%d",&choice);
switch (choice)
{
case 1:
    upi_payment(charges);
    break;
case 2:
    netbanking(charges);
    break;
case 3:
    debitcard(charges);
    break;
case 4:
        creditcard(charges);
    break;
default:
    break;
}
// print_bill();
}
void adminlogin(){
    int a=0,i=0;
char uname[10],c=' '; 
char pword[10],code[10];
char user[10]="dhoni";
char pass[10]="thala";


printf("\n  ======================= ADMIN  LOGIN FORM  =======================\n  ");
printf(" \n                       ENTER USERNAME:-");
scanf("%s", &uname); 
printf(" \n                       ENTER PASSWORD:-");
while(i<10)
{
    pword[i]=getch();
    c=pword[i];
    if(c==13) break;
    printf("*");
    i++;
}
pword[i]='\0';

i=0;
//scanf("%s",&pword); 
    if(strcmp(uname,"dhoni")==0 && strcmp(pword,"thala")==0)
{
printf("  \n\n\n       WELCOME TO OUR SYSTEM !! YOUR LOGIN IS SUCCESSFUL");
//printf("\n\n\n\t\t\t\tPress any key to continue...");
//getch();//holds the screen
return;
}
else
{
    printf("\n        SORRY !!!!  LOGIN IS UNSUCESSFUL\n");
    system("cls");
    adminlogin();
}
}
void login(){
    int a=0,i=0;
char uname[10],c=' '; 
char pword[10],code[10];
char user[10]="dhoni";
char pass[10]="thala";


printf("\n  =======================  LOGIN FORM  =======================\n  ");
printf(" \n                       ENTER USERNAME:-");
scanf("%s", &uname); 
printf(" \n                       ENTER PASSWORD:-");
while(i<10)
{
    pword[i]=getch();
    c=pword[i];
    if(c==13) break;
    printf("*");
    i++;
}
pword[i]='\0';

i=0;
//scanf("%s",&pword); 
    if(strcmp(uname,"kohli")==0 && strcmp(pword,"king")==0)
{
printf("  \n\n\n       WELCOME TO OUR SYSTEM !! YOUR LOGIN IS SUCCESSFUL");
//printf("\n\n\n\t\t\t\tPress any key to continue...");
//getch();//holds the screen
return;
}
else
{
    system("cls");
    printf("\n        SORRY !!!!  LOGIN IS UNSUCESSFUL\n");
    
    login();
}


    system("cls");	
}
int id2=1000;
int changeprize(int);
void reservation(int *,int,int );
int choice1(void);
void cancel(int *);        
void ticket1(int choice,char name[10],int id2,int price,char date[15]);
void ticket2(int choice,char name[10],int id2,int price,char date[15]);
void ticket3(int choice,char name[10],int id2,int price,char date[15]);
int cmovie(void);
int movie(void);
void details(void);
int count=0;

struct moviedetails{
char name[25];
char phone[15];
int seat;
int id;
};
struct moviedetails person[300];
void MovieTicketGenerator()
{
int choice,price=500,slection,i;
int seat[3][101];
int x;
start2:
//int choice;
login();
printf("                 Simple Movie Ticket Booking System\n");
printf(" ==================================================================\n");
printf("||             1- To edit price of ticket (only admin):           ||\n");
printf("||             2- To view reserved tickets :         			  ||\n");
printf("||             3- To puchase ticket:                              ||\n");
printf("||             4- To cancel the seat:                             ||\n");
printf("||             5- Exit system:                                    ||\n");
printf("||================================================================||\n");
printf("  Enter your choice: ");
scanf("%d",&choice);


while(x!=7)
{
    
    switch(choice)
    {
        case 1:
            price=changeprize(price);
            goto start2;
            break;
        case 2:
            details();
            break;	
        case 3:	
            slection=movie();
            reservation(seat[slection-1],price,slection);	
            count++;
            break;
        case 4:
            slection=cmovie();	
            cancel(seat[slection-1]);	
            break;
        case 5:
            x=7;

            break;
        default: 
            printf("Choice not available\n");
            break;	
    }
}
exit(0);
}
int changeprize(int prize)
{
adminlogin();
    printf("Please enter new price: ");
    scanf("%d",&prize);
    //system("PAUSE");
    system("CLS");


return prize;
}
void reservation(int *array,int price,int slection)
{          
    int i,j;
    char date[15];
    printf("\n                                SCREEN\n\n\n");
    for (i=1;i<=100;i++)
    {
        if (array[i]==0)
            printf("%d\t",i);
        else 
            printf("*\t");	
        if(i%10==0)
            printf("\n\n");
    }
    printf("Please enter your name: ");
    scanf(" %19[^\n]%*[^\n]",&person[count].name);
    printf("Please enter your phone number: ");
    scanf("%u",&person[count].phone);
    printf("Please enter Date: ");
    scanf("%s",date);
    printf("Which seat number you want? ");
    scanf("%d",&j);
    if (j>100||j<1)
        {
            printf("seat1 number is unavailable in this theater\n");
            printf("Please re-enter seat number: ");
            scanf("%d",&j);
        }
    if (array[j]==1)
        {
            printf("Sorry, this ticket is already booked! Please choose another seat.\n");
            scanf("%d",&j);
        }
    else			
        array[j]=1;
    person[count].seat=j;
    FILE *fp=fopen("movieticket.txt","a");
    fprintf(fp,"%d %s %s\n",id2,person[count].name,date);
    fclose(fp);
    payment_gateway(price);
    if (slection==1)
        ticket1(j,person[count].name,id2,price,date);
    else if (slection==2) 	
        ticket2(j,person[count].name,id2,price,date);
    else 
        ticket3(j,person[count].name,id2,price,date);			
    id2++;
    
    system("pause");
    system("cls");	
    MovieTicketGenerator();
    
}

void details2(void)
{
int Cseat,bid,count=0;
char name[20],date[20];
FILE*fp=fopen("movieticket.txt","r");
// FILE*fp2=fopen("temp2.txt","w");
    while (fscanf(fp,"%d %s %s",&bid,name,date)==3)
    {
    printf("%d %s %s \n",bid,name,date);
    }
    
    fclose(fp);
    //main();

}       
void cancel(int *array)
{	
    details2();
    //printf("Enter correct serial number from display sheet:");
    int Cseat,bid,count=0;
    char name[20],date[20];
    printf("Please enter ID number of ticket: ");
    scanf("%d",&Cseat);
    FILE*fp=fopen("movieticket.txt","r");
    FILE*fp2=fopen("temp2.txt","w");
    
    while (fscanf(fp,"%d %s %s",&bid,name,date)==3)
    {
    count++;
    if (count!=Cseat)
    {
        fprintf(fp2,"%d %s %s \n",bid,name,date);
    }
    
    }
    fclose(fp);
    fclose(fp2);
    remove("movieticket.txt");
    rename("temp2.txt","movieticket.txt");
    printf("YOUR TICKET IS CANCELLED!!!");
    
    
}
void ticket1(int choice,char name[10],int id2,int price,char date[15])
{
    system("cls");
    printf("\n\n");
    printf("\t-----------------THEATER BOOKING TICKET----------------\n");
    printf("\t============================================================\n");
    printf("\t Booking ID : %d \t\t\tShow Name : PUSHPA:THE RISE\n",id2);
    printf("\t Customer  : %s\n",name);
    printf("\t\t\t                              Date      : %s\n",date);
    printf("\t                                              Time      : 08:00pm\n");
    printf("\t                                              Hall      : 02\n");
    printf("\t                                              seats No. : %d  \n",choice);
    printf("\t                                              price . : %d  \n\n",price);
    person[count].id=id2;
    printf("\t============================================================\n");
//	id2++;
//	main();
    return;
}
void details(void)
{
int Cseat,bid,count=0;
char name[20],date[20];
FILE*fp=fopen("movieticket.txt","r");
// FILE*fp2=fopen("temp2.txt","w");
    while (fscanf(fp,"%d %s %s",&bid,name,date)==3)
    {
    printf("%d %s %s \n",bid,name,date);
    }
    
    fclose(fp);
    MovieTicketGenerator();

}             
int movie(void)
{
int i;
// system("cls");
printf("\t\t\twhich movie you want to see?\n");
printf("\t\t\t----------------------------\n\n");
printf("\t\t\tpress 1 for PUSHPA\n\n");
printf("\t\t\tpress 2 for BAHUBALI\n\n");
printf("\t\t\tpress 3 for SALAAR\n");
scanf("%d",&i);
system("cls");
return i;
}
void ticket2(int choice,char name[10],int id2,int price,char date[15])
{
    system("cls");
    printf("\n\n");
    printf("\t-----------------THEATER BOOKING TICKET----------------\n");
    printf("\t============================================================\n");
    printf("\t Booking ID : %d \t\t\tShow Name : BAHUBALI\n",id2);
    printf("\t Customer  : %s\n",name);
    printf("\t\t\t                              Date      : %s\n",date);
    printf("\t                                              Time      : 09:00pm\n");
    printf("\t                                              Hall      : 03\n");
    printf("\t                                              seats No. : %d  \n",choice);
    printf("\t                                              price . : %d  \n\n",price);
    person[count].id=id2;
    printf("\t============================================================\n");
    
    return;
}
int cmovie(void)
{
int i;
printf("\t\t\twhich movie ticket you want to cancel\n");
printf("\t\t\t-------------------------------------\n");
printf("\t\t\tpress 1 for PUSHPA\n\n");
printf("\t\t\tpress 2 for BAHUBALI\n\n");
printf("\t\t\tpress 3 for SALAAR\n");
scanf("%d",&i);
return i;		
}
void ticket3(int choice,char name[10],int id2,int price,char date[15])
{
    system("cls");
    printf("\n\n");
    printf("\t-----------------THEATER BOOKING TICKET----------------\n");
    printf("\t============================================================\n");
    printf("\t Booking ID : %d \t\t\tShow Name : SALAAR \n",id2);
    printf("\t Customer  : %s\n",name);
    printf("\t\t\t                              Date      : %s\n",date);
    printf("\t                                              Time      : 10:00pm\n");
    printf("\t                                              Hall      : 04\n");
    printf("\t                                              seats No. : %d  \n",choice);
    printf("\t                                              price . : %d  \n\n",price);
    person[count].id=id2;
    printf("\t============================================================\n");
        
    return;
}




int main() {
int n;
printf("Project 31 \n");
printf ("PROJECT MENU--------- \n");
printf (ANSI_COLOR_GREEN "PRESS 1 FOR PLAYING SUDOKU" ANSI_COLOR_RESET"\n");
printf (ANSI_COLOR_YELLOW "PRESS 2 FOR PLAYING CASINO GAME" ANSI_COLOR_RESET "\n");
printf (ANSI_COLOR_BLUE "PRESS 3 FOR USING CALCULATOR" ANSI_COLOR_RESET "\n");
printf (ANSI_COLOR_MAGENTA "PRESS 4 FOR VIEWING PERIODIC TABLE" ANSI_COLOR_RESET "\n");
printf (ANSI_COLOR_CYAN "PRESS 5 FOR TEMPERATURE CONVERSION" ANSI_COLOR_RESET "\n");
printf (ANSI_COLOR_RED "PRESS 6 FOR BOOKING MOVIE TICKET" ANSI_COLOR_RESET "\n");
printf("Enter your value: ");
scanf("%d", &n);
switch (n) {
    case 1:
        sudokuGame();
        break;
    case 2:
        casinoGame();
        break;   
    
    case 3:
        CalculatorSc();
        break;
        
    case 4:
        PeriodicTable();
        break;
        
    case 5: 
        TempConvertor();
        break;
        
    case 6:
        MovieTicketGenerator();
        break;
    default:
        printf("Invalid Choice");
        
    
}
return 0;
}   

