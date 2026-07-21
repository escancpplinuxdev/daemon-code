
//#include <iostream>
#include <bits/stdc++.h> // it include all standard libraries at once.
			 // use in competative programming
			 // but not use for production (Due to complie time overhead.)
using namespace std;

int main()
{
	int x, y ;
	cin >> x>>y;
	cout<<"Value of x : '"<<x<<"'\t Value of y : '"<<y<<"'\n";

	unordered_map<int,int>m= {{4,5},{5,6},{7,8}};
	for (auto it : m)
	{
		std::cout<<it.first<<" | "<<it.second<<"\n";
	}

	const char * temp = "/tmpdata/cpp/07072026/cpp/roadMap/LearnBasic/InputOutput/testFileopen.txt";
	FILE *fp = fopen(temp, "a+");

	if(fp != NULL)
	{	char buffer[256]= "";
		while(fgets(buffer,sizeof(buffer),fp) )
		{
			size_t len = strlen(buffer);
	
			// skip empty line
			if(buffer[0] == '\n')
			{
				continue;
			}

			// remove end newline
			if(len > 0 && buffer[len -1] == '\n')
			{
				buffer[len - 1] = '\0';
			}

			std::cout<<"buffer = '"<<buffer<<"'\n";
		}
		fclose(fp);
	}

	return 0;
}

/*


 */
