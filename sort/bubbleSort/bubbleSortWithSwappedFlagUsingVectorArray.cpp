
#include <iostream>
#include <vector>
using namespace std;

//void printVector(const vector<int>&a)
void printVector(const vector<int>a) // nothing activity perform on a here just printing of a
{
	for(int n : a)
		cout<<n<<" ";
//		cout<<" "<<n;
	cout<<endl;
}

void bubbleSort(vector<int> &a) // here swapping activity perform that's why &a
{
	int n=a.size();
	
	for(int i=0;i<n-1;i++)
	{
		bool swapped = false;
		for(int j=0; j<n-1-i;j++)
		{
			if(a[j]>a[j+1])
			{
				int temp = a[j];
				a[j]=a[j+1];
				a[j+1]=temp;
				swapped=true;
			}
		
		}
		if(!swapped)
			break;
	}
}

int main()
{
	vector<int>a={8,5,3,1,7,6,2,4};
	cout<<"UnSorted Array : ";
	printVector(a);
	bubbleSort(a);
	cout<<"Sorted Array   : ";
	printVector(a);

	return 0;
}
