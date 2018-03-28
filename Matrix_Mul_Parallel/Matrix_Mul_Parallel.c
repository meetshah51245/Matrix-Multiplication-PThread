#include<stdio.h>
#include<stdlib.h>
#include<omp.h>


void GetNumbers(char fileName[]);

extern double **Matrix;
extern int Matrix_Size;
double **Matrix_Mul_Result = NULL;

int main()
{
	int i,j,k;
	double result = 0;
	FILE* fp;
	char delim[2];
	
	
	
    GetMatrix("inputMatrix.csv");
	
	// Memory allocated to the resultant matrix
    Matrix_Mul_Result = (double **)malloc(Matrix_Size * sizeof(double*));
    
	if (Matrix_Mul_Result == NULL)
    {
 		printf("Memory has not been allocated");
		return 1;
    }
	
	//start of parallel processing
	#pragma omp parallel private(i,j,k) 
	{
		#pragma omp for schedule(dynamic,chunk) 
		for(i = 0; i < Matrix_Size; i++)
		{
			Matrix_Mul_Result[i] = (double *)malloc(Matrix_Size * sizeof(double));		
		}	
		
		start();
	
		#pragma omp for schedule(dynamic,chunk) reduction(+:result)    
		for(i = 0; i < Matrix_Size; i++)
		{ 
			for(j = 0; j < Matrix_Size; j++)
			{  
				result = 0;
				for(k = 0; k < Matrix_Size; k++)
				{	
					result = result + Matrix[i][k] * Matrix[k][j]; // Matrix Multiplication Result
				}
				
				Matrix_Mul_Result [i][j] = result;
			}
		}
	}
	
	 stop();
	
  //write the output matrix
  fp = fopen("outputMatrix.csv", "w");
  fprintf(fp, "%d\n", Matrix_Size);
  
  for(j = 0 ; j < Matrix_Size; j++)
  {
    sprintf(delim, "%s", "");
	
    for(i = 0; i < Matrix_Size; i++)
    {
      fprintf(fp, "%s%.2f", delim, Matrix_Mul_Result[j][i]);
      sprintf(delim, "%s", ",");;
    }
      fprintf(fp, "\n");
  }
  
  fclose(fp);
   
  
  
  //free all allocated memory of matrix
  
	for(j = 0; j < Matrix_Size; j++)
	{
		free(Matrix_Mul_Result[j]);
		free(Matrix[j]);
	}
  
	free(Matrix_Mul_Result);
	free(Matrix);

	return 0;
}