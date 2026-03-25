/**
 *g++ dynamicMatrix.cpp -o dynamicMatrix
 * */

#include <iostream>
using namespace std;

int main()
{
    int rows, cols;

    cout << "Enter number of rows: ";
    cin >> rows;

    cout << "Enter number of columns: ";
    cin >> cols;

    /* Dynamic allocation of matrix */
    int **matrix = new int*[rows];

    for(int i = 0; i < rows; i++)
    {
        matrix[i] = new int[cols]();   // () initializes with 0
    }

    /* Input matrix elements */
    cout << "\nEnter matrix elements:\n";
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            cout << "Element [" << i << "][" << j << "] : ";
            cin >> matrix[i][j];
        }
    }

    /* Display original matrix */
    cout << "\nOriginal Matrix:\n";
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

    /* Dynamic allocation for transpose */
    int **transpose = new int*[cols];

    for(int i = 0; i < cols; i++)
    {
        transpose[i] = new int[rows]();
    }

    /* Compute transpose */
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            transpose[j][i] = matrix[i][j];
        }
    }

    /* Display transpose matrix */
    cout << "\nTranspose Matrix:\n";
    for(int i = 0; i < cols; i++)
    {
        for(int j = 0; j < rows; j++)
        {
            cout << transpose[i][j] << " ";
        }
        cout << endl;
    }

    /* Free memory */
    for(int i = 0; i < rows; i++)
        delete[] matrix[i];
    delete[] matrix;

    for(int i = 0; i < cols; i++)
        delete[] transpose[i];
    delete[] transpose;

    return 0;
}
