
#include <iostream>
#include <memory>

class  Employee
{
public:
	int eno = 0;
	std::string name = "raj";
};

int main()
{
//	std::unique_ptr<Employee> emp = std::make_unique<Employee>();

	Employee *emp = new Employee();

	std::cout<<"Name = "<<emp->name<<"\n";
	std::cout<<"Eno  = "<<emp->eno<<"\n";
	return 0;
}

/*


*/
