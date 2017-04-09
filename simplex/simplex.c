/*
 *	Description : Simplex method implementation
 *	Author : Ranjan Mishra(CS1519)
 *	Date : 20th Dec 2016
 *  
 * 
 * 	Brief of simplex algorithm and how it works
 * 	The simplex algorithm operates on a linear programing is statndard format given below
     
 	Max 	c'x 
	Sub to 	Ax <= b
			x_i>= 0 		for all i
	 
	where 
 * 	c' is row vector of coefficients 
 *  x  is vector of variables
 *  A  is m*n matrix of constraints 
	   where 
		sum_over_all_ j(a_i_j*xj) <= b_i;  for all i, 
		are set of constraints to be satisfied
 *
 *  

	
          
 */


/* Necessary header files to be included
 * 
 */

	#include<stdio.h>
	#include<stdlib.h>
	#include<math.h>
	#include<string.h>
	

// maximum values of m and n defined here,
#define CONSTRAINTS 20
#define VARIABLES 20

static const double error = 1.0e-7;

int equal(double a, double b){ 
	return fabs(a-b) < error; 
}

typedef struct {
  unsigned int numVariables, numConstarints; // m=rows, n=columns, mat[m x n]
  double constraintsMatrix[CONSTRAINTS][VARIABLES];
} Tableau;


/* Funtion to read the input file, name of the input file as input  
 * returns the initail tableau using input file
 * The input file should have following format
 * first line should contain number of constraints followed by number of variables
 * then numConstrains lines each containing numVaribles coefficients for each variable in one constraints
 * below is one such example of inpput file

	3 4
	0 2 3 4
	3 4 5 1
	2 3 1 5 
 */ 
   
void readProblem(char *fileName, Tableau *t){
	FILE *fp;
	int i, j, err;
	
	fp = fopen(fileName, "r");
	if(!fp){
		perror("Unable to read such file\n");
		exit(1);
	}
	
	
	err = fscanf(fp, "%d %d", &t->numConstarints, &t->numVariables);
	if (err == 0 || err == EOF) {
		perror("Cannot read number of constraints and variables \n"); 
		exit(1);
	}
	
	
	for(i=0;i<t->numConstarints; i++) {
		for(j=0;j<t->numVariables; j++) {
			err = fscanf(fp, "%lf", &t->constraintsMatrix[i][j]);
			if (err == 0 || err == EOF) {
				perror("Cannot read constraint matrix\n"); 
				exit(1);
			}
		}
	}
	
	printf("Succesfully read input LP from file: '%s'.\n",fileName);
	fclose(fp);

}

/* Function to print the current tableau
 */


void printTableau(Tableau *t, const char* mes) {
  static int counter=0;
  int i, j;
  printf("\n%d. Tableau %s:\n", ++counter, mes);
  //nl(70);

  printf("%-6s%5s", "col:", "b[i]");
  for(j=1;j<t->numVariables; j++){ 
	printf("    x%d,", j); 
  } 
  
  printf("\n");

  for(i=0;i<t->numConstarints; i++) {
    if (i==0) 
		printf("max:"); 
	else
		printf("b%d: ", i);
    
    for(j=0;j<t->numVariables; j++) {
      if (equal((int)t->constraintsMatrix[i][j], t->constraintsMatrix[i][j]))
        printf(" %6d", (int)t->constraintsMatrix[i][j]);
      else
        printf(" %6.2lf", t->constraintsMatrix[i][j]);
      }
    printf("\n");
  }
  //nl(70);
}


/* Fuction to add slack variables
 */
 
 void addSlackVariables(Tableau *t) {
  int i, j;
  for(i=1; i<t->numConstarints; i++) {
    for(j=1; j<t->numConstarints; j++)
      t->constraintsMatrix[i][j + t->numVariables -1] = (i==j);
  }
  t->numVariables += t->numConstarints -1;
}

/*
 * Function to find pivot column and pivot row
 */
 
 void findPivot(Tableau *t, int *pivot){
	int i,j;
	int pivotColumn = 1;
	int pivotRow = 1;
	// finding the pivot column, will be one with most negative coefficient
	
	double min = t->constraintsMatrix[0][pivotColumn];
	double minRatio = -1.0;
	double ratio = 0;
	
	for(i=1;i<t->numVariables;i++){
		if(min > t->constraintsMatrix[0][i]){
			min = t->constraintsMatrix[0][i];
			pivotColumn = i;
		}
	}
	
	if(min >= 0){			// All are positive hence this is optimal
		pivot[0] = -1;
		return;
	}
	
	// finding the pivot row, we already have pivot column with us. 
	// pivot row will be one with that makes lowest increase of the variable found.
	
	for(i=1;i<t->numConstarints;i++){
		ratio = t->constraintsMatrix[i][0]/t->constraintsMatrix[i][pivotColumn];
		if((ratio > 0  && ratio < minRatio ) || minRatio < 0){
			minRatio = ratio;
			pivotRow = i;
		}
	}
	if(minRatio == -1.0){
		printf("This LP is unbounded!");
		exit(1);
	}
	
	pivot[0] = pivotColumn;
	pivot[1] = pivotRow;
}




int main(int argc, char *argv[]){
	Tableau t;
	if(argc != 2){
		perror("Please provide file name as command line input");
		exit(1);
	}
	
	readProblem(argv[1], &t);
	printTableau(&t, "afetr readng input itself");
	addSlackVariables(&t);
	printTableau(&t, "After adding slack variables");
	return 0;
}
	
