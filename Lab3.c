#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>

#define t_total 18

//All Functions are declared
int solve(int[][9]);
int check(int[][9], int, int, int);
int find_zero(int[][9], int *, int *);

//Global Variable are initialized
int puzzle[9][9];
int row = 0;
int column = 0;

void *checkRow(void *row){
	
	//Arguement from passed to thread is taken
	int thread_row = *((int *) row);
	//Array to hold all the 9 values in the row
	int rowValues[9] = {0};
	//Counts the summation of the row
	int total = 0;
		
		//Iterates through the row and puts value into the array
		for (int i = 0; i < 9; i++){
		 	rowValues[i] = puzzle[thread_row][i];
		}
		
		//Iterates through the array
		for (int j = 0; j < 9; j++){
		//Summation of the array
		total += rowValues[j];
			//Checks if all 9 indexes of array is filled
			if(rowValues[j] == 0){
				return (void *) 1;
				}	
		}
		
		//Checks if there are any duplicates in the array
		if (total != 45){
			return (void *) 1;
		}
	
	//Exits
	pthread_exit(NULL);
}

void *checkCol(void *col){
	//Arguement from passed to thread is taken
	int thread_col = *((int *) col);
	//Array to hold all the 9 values in the row
	int colValues[9] = {0};
	//Counts the summation of the row
	int total = 0;
		
		//Iterates through the column and puts value into the array
		for (int i = 0; i < 9; i++){
		
		 	colValues[i] = puzzle[i][thread_col];
		}
		
		//Iterates through the array
		for (int j = 0; j < 9; j++){
			//Summation of the array
			total += colValues[j];
			if(colValues[j] == 0){
				return (void *) 1;
				}	
		}
		//Checks if all 9 indexes of array is filled
		if (total != 45){
			return (void *) 1;
		}
		
	//Exits
	pthread_exit(NULL);
}


int main(){
   
   //Used to create Threads
   pthread_t threads[t_total];
   
   //Keeps count of threads
   int t_count = 0;
   
   //Used to take returned value from threads 
   void *result;
   	
   //fail keeps track of if a check has failed
   int fail = 0;
	
	
	//File with puzzle is opened
	FILE *f = fopen("Lab3 puzzle.txt", "r"); 
	
	//2 for loops are used to iterate through 2d array
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
		
			//Variable that holds contents of file is 				initialized
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

	
	
	//If the puzzle is solved then the 2d array(puzzle) is printed 		out
        if(solve(puzzle)){
        //open solution file
        FILE *f2 = fopen("solution.txt", "w");
        for (int i = 0; i < 9; ++i){
            for (int j = 0; j < 9; ++j){
            	//Save solution to the file
                fprintf(f2, "%d ", puzzle[i][j]);
            }
            fprintf(f2,"\n");
        }
        printf("Puzzle has been solved and has been saved to solution.txt");
        //close file
	fclose(f2);
        }else{
        printf("%s", "No Solution to the Puzzle Provided");
        }
        
        //Initializes 18 threads
        for (int r = 0; r < 9; r++){
        	//row is set up to send as an arguement to the thread
        	int *row = malloc(sizeof(*row));
        	*row = r;
        	//Threads to check the rows are created
		pthread_create(&threads[t_count++], NULL, checkRow, row);
		//col is set up to send as an arguement to the thread
		int *col = malloc(sizeof(*col));
        	*col = r;
        	
        	//Threads to create the columns are created
		pthread_create(&threads[t_count++], NULL, checkCol, col);
	}
	
	//All threads are joined 
        for (int h = 0; h < t_total; h++){
        	//Threads are joined
        	pthread_join(threads[h], &result);
        	//Checks return value of thread
        	if(result == 1){
        		//fail is incremented if solution on row or column is wrong
        		fail++;
        	}
        }	
        	
        //Checks if the solution has failed in any area	
        if(fail == 0){
        	//If no fails then solution is correct
		printf("\nSolution has been validated and is correct\n");
	}else{
		//if fails > 0 then solution is wrong
		printf("\nSolution is Wrong\n");
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
    	//Checks if any number in the row is equal to the guessed 		number
        if (puzzle[row][x] == num){
            return 0;
        }
        //Checks if any number in the column is equal to the guessed 		number
        if (puzzle[x][column] == num){
            return 0;
        }
        //Checks if any number in the imediate 3x3 is equal to the 		guessed number
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
            	//points to the row and column and sets them to that 		which the zero found exists in
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
