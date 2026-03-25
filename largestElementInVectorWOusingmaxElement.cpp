#include <iostream>
#include <vector>

int main()
{
        std::vector<int> vec = {12, 45 , 7 , 89 ,23 ,56};
	std::cout<< "Original vector : ";
	for(int n : vec) std::cout<< n << " ";
	std::cout<<std::endl;

        if(!vec.empty())
        {
                int maxIndex = 0; // assume the first element of the largest element

                for (int i =1; i< vec.size(); i++)
                {
                        if (vec[i] > vec[maxIndex])
                        {
                                maxIndex = i;
                        }
                }

                int temp = vec[0];
                vec[0] = vec[maxIndex];
                vec [maxIndex] = temp;
        }

        std::cout<< "Vector after manual swap : ";
        for(int n : vec) std::cout<< n << " ";
        std::cout<< std::endl;

        return 0;
}
