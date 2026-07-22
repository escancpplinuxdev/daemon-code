
#include <iostream>
#include <unordered_set>
#include <vector>

int main()
{
	// declare an unordered_set of integers.
	std::unordered_set<int>s;
	for(int i = 1; i<=10; i++)	
	{
		s.insert(i);
	}

	// display all elements present in unordered_set
	for(auto it = s.begin(); it != s.end(); ++it)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<"\n";
	std::cout<<"Size of unordered_set = '"<<s.size()<<"'\n";
	s.emplace(14);
	s.insert(54);

	std::cout<<"Size of unordered_set = '"<<s.size()<<"'\n";

//	std::cout<<"s.bucket_size = '"<<bucket_size(s)<<"'\n";
	std::cout<<"s.max_size() = '"<<s.max_size()<<"'\n";
	std::cout<<"s.max_bucket_count() = '"<<s.max_bucket_count()<<"'\n";
	std::unordered_set<int> v = {s.begin(),s.end()};

	for(auto it = v.cbegin(); it != v.cend(); ++it)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<"\n";

	std::cout<<"7 present -> "<<(s.count(7) ? "yes" : "no")<<"\n";

	if (s.find(4) != s.end())
	{
		std::cout<<"4 present \n";
		s.erase(4);
	}
	else
	{
		std::cout<<"Not present \n";
	}

	// display all elements present in unordered_set
	for(auto it = s.begin(); it != s.end(); ++it)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<"\n";

	std::cout<<"Size of unordered_set = '"<<s.size()<<"'\n";

	if(s.empty())
	{
		std::cout<<"set is empty\n";
	}
	else	
	{
		std::cout<<"set Not empty\n";
	}
	s.erase(s.begin(),s.end());

	if(s.empty())
        {
                std::cout<<"set is empty\n";
        }
        else
        {
                std::cout<<"set Not empty\n";
        }

	std::cout<<"Size of unordered_set = '"<<s.size()<<"'\n";

	for(int i = 1; i<=10; i++)	
	{
		s.insert(i);
	}
	
	for(auto it = s.begin() ; it != s.end(); ++it)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<"\n";

	std::cout<<"Size of unordered_set = '"<<s.size()<<"'\n";
	s.clear();
	std::cout<<"Size of unordered_set = '"<<s.size()<<"'\n";

#if __cplusplux >= 202002L // for -std=c++20
	std::cout<<"s.contains(29) = "<<std::boolalpha<<" = "<<s.contains(29)<<"\n";
#endif
	return 0;
}

/*


*/
