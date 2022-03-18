#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//All Function are declared
int solve(int[][9]);
int check(int[][9], int, int, int);
int find_zero(int[][9], int *, int *);


int main(){
   
   //Array that holds puzzle is declared	
   int puzzle[9][9];
	
	//File with puzzle is opened
	FILE *f = fopen("Lab3 puzzle.txt", "r"); 
	
	//2 for loops are used to iterate through 2d array
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
		
			//Variable that holds contents of file is initialized
			char c;
			
			//Error checks the file
			if (fscanf(f, " %c", &c) != 1){
				printf("fail");
				exit(0);
				
			//Sets the numbers into the array	
			}else if (isdigit((unsigned char)c)){
				puzzle[i][j] = c - '0';
				
			//Sets 0 into the array	
			}else{
				puzzle[i][j] = 0;
			}
		}	
	}
    
    //row and column are both initialized	
    int row = 0;
    int column = 0;
	
	//If the puzzle is solved then the 2d array(puzzle) is printed out
        if(solve(puzzle)){
        for (int i = 0; i < 9; ++i){
            for (int j = 0; j < 9; ++j){
                printf("%d", puzzle[i][j]);
                printf(" ");
            }
            printf("\n");
        }

        }else{
        printf("%s", "No Solution to the Puzzle Provided");
        }
        return 0;
    
}

//Fucntion is used to check the validity of the number that is guessed
int check(int puzzle[][9], int row, int column, int num){
    
    //These are used to check the imediate 3x3 array of number  
    int box_x = row / 3 * 3;
    int box_y = column / 3 * 3;
    
    //for loop to iterate through all 9 spaces of array
    for (int x = 0; x < 9; ++x){
    	//Checks if any number in the row is equal to the guessed number
        if (puzzle[row][x] == num){
            return 0;
        }
        //Checks if any number in the column is equal to the guessed number
        if (puzzle[x][column] == num){
            return 0;
        }
        //Checks if any number in the imediate 3x3 is equal to the guessed number
        if (puzzle[box_x + (x % 3)][box_y + (x / 3)] == num){
            return 0;
        }
    }
    return 1;
}

//Function used to find a zero in the array
int find_zero(int puzzle[][9], int *row, int *column){
    
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            //if cell is 0 
            if(!puzzle[i][j]){
            	//points to the row and column and sets them to that which the zero found exists in
                *row = i;
                *column = j;
                return 1;
            }
        }
    }
    return 0;
}

//Function used to solve the puzzle  
int solve(int puzzle[][9]){
    
    //row and column are initialized	
    int row;
    int column;
    
    //If the cell contains a number then end and return 1
    if (!find_zero(puzzle, &row, &column)){
        return 1;
    }
    
    //for loop to try all 9 numbers to guess
    for (int i = 1; i < 10; i++){
    	//checks if the guess is correct
        if(check(puzzle, row, column, i)){
            //correct guess is set to the appropriate row and column
            puzzle[row][column] = i;
            
            //recursive call to keep check all cells
            if(solve(puzzle)) return 1;
            puzzle[row][column] = 0;
            
        }
    }
    return 0;
}
