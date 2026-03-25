/*
 * 	make a
 * */

#include <iostream>
using namespace std; // ambiguty error, namespace pollution, naming confilict

int main()
{
	int rows, cols; // horizantal rows // aade  rows
			// vertical columns // khade column

	// dynamic allocation of matrix
	cout<<"Enter no of rows: ";
	cin>>rows;
	cout<<"Enter no of cols: ";
	cin >> cols;


	// dynamic allocation of matrix
	int **matrix = new int *[rows];

	for(int i = 0; i< rows;i++)
	{
		matrix[i] = new int [cols](); // () initializes with 0
	}


	// input martix elements
	
	cout <<"Enter matrix elements:\n";
	for(int i=0; i < rows;i++)
	{
		for(int j=0;j<cols;j++)
				{
					cout<<"Elements ["<<i<<"]["<<j<<"] :";
					cin>>matrix[i][j];
				}
	}

	//Display original matrix 
	cout<<"\noriginal martix:\n";
	for(int i=0; i< rows;i++)
	{
		for(int j=0;j<cols;j++)
		{
			cout<<matrix[i][j]<<" ";
		}
		cout<<endl;
	}

	
	//Display allocation for transpse
	
	cout<<"\nTranspose  initaist \n\n"	;
	int **transpose = new int*[cols];

	for(int i = 0; i<cols;i++)
	{
		transpose[i] = new int [rows]();
		cout<<"transpose["<<i<<"] = "<<transpose[i]<<endl;
		cout<<"*transpose["<<i<<"] = "<<*transpose[i]<<endl;
		cout<<"**transpose["<<i<<"] = "<<*(*transpose[i])<<endl;
		cout<<"\n";
	}
	
	cout<<"\n\nTranspose  Working \n\n"	;
	for(int i =0; i< rows;i++)
	{
		for(int j=0; j< cols;j++)
		{
			transpose [j][i] = matrix[i][j];
		cout<<"transpose["<<j<<"]["<<i<<"] = "<<transpose[j][i]<<endl;
		}
	}

	//Display transpose matrix
	
	cout<<"\nTranspose matrix:\n";
	for (int i=0; i< cols;i++)
	{
		for(int j=0;j<rows;j++)
		{
			cout<<transpose[i][j]<<" ";
		}
		cout<<endl;
	}

	//free memory 
	for(int i=0; i< rows; i++)
		delete[] matrix[i];
	delete[] matrix;

	for(int i =0; i< cols; i++)
		delete[] transpose[i];
	delete[] transpose;

	return 0;

}

