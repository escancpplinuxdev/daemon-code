
#include <bits/stdc++.h>
using namespace std;

void rearrangeArray(int arr[], int n)
{
    // Encode both old and new values in the same array
    // New value = arr[arr[i]]
    // We store it as: arr[i] = arr[i] + (arr[arr[i]] % n) * n
    // Then later we decode: arr[i] = arr[i] / n
    
    // Step 1: Encode
    for (int i = 0; i < n; i++)
    {
        int oldValue = arr[i] % n;
        int newValue = arr[oldValue] % n;
        arr[i] = oldValue + newValue * n;
    }
    
    // Step 2: Decode
    for (int i = 0; i < n; i++)
    {
        arr[i] = arr[i] / n;
    }
}

int main()
{
    int T;
    cin >> T;
    
    while (T--)
    {
        int n;
        cin >> n;
        
        int arr[n];
        for (int i = 0; i < n; i++)
        {
            cin >> arr[i];
        }
        
        rearrangeArray(arr, n);
        
        for (int i = 0; i < n; i++)
        {
            cout << arr[i] << " ";
        }
        cout << endl;
    }
    
    return 0;
}



/*

Rearrange Array
Easy
0/40
Average time to solve is 15m
Asked in companies
Problem statement

You're given an array ARR[] of size N, where every element belongs to the range 0 to N-1. Your task is to rearrange the given array so that ARR[i] becomes ARR[ARR[i]] and make sure that this task is done with O(1) extra space.
Detailed explanation ( Input/output format, Notes, Images )
Constraints :

1 <= T <= 10^2
1 <= N <= 10^4
0 <= ARR[i] <= N-1

Where 'ARR[i]' is 'ith' element of ARR.

Time Limit : 1 sec

Sample Input 1 :

1 	//one array
4 	//4 element
0 1 2 3 //array element

Sample Output 1:

0 1 2 3    

Explanation of Sample Input 1 :

We can see that ARR[ARR[0]] i.e ARR[0] is 0, hence ARR[0] in the output array will be 0. Also, ARR[ARR[1]] i.e ARR[1]  is1, hence ARR[1] in the output array will be 1. Similarly,  ARR[ARR[2]] i.e ARR[2] is 2, hence ARR[2] in the output array will be 2. Finally, ARR[ARR[3]] i.e ARR[3] is 3, hence ARR[3] in the output array will be 3.

Sample Input 2 :

1
4
3 2 0 1

Sample Output 2 :

1 0 3 2

Explanation of Sample Input 2 :

We can see that ARR[ARR[0]] i.e ARR[3] is 1, hence ARR[0] in the output array will be 1. Also, ARR[ARR[1]] i.e ARR[2] is 0, hence ARR[1] in the output array will be 0. Similarly,  ARR[ARR[2]] i.e ARR[0] is 3, hence ARR[2] in the output array will be 3. Finally, ARR[ARR[3]] i.e ARR[1] is 2, hence ARR[3] in the output array will be 2.


#include <bits/stdc++.h> 
void rearrangeArray(int arr[], int n)
{
	// Write your code here
}

give this with int main() and header

*/
