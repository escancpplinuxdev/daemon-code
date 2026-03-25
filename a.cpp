#include <iostream>
using namespace std;

// 3rd structure
struct Address
{
	string city;
	string state;
	int pincode;
};

// 2rd structure
struct Employee
{
	int empId;
	string empName;
	Address *addr;
};

// 1st structure
struct Company
{
	string companyName;
	Employee *emp;
};

int main()
{
	// Declare structures
	Address address1;
	Employee employee1;
	Company company1;

	address1.city = "Mumbai";
	address1.state = "Maharashtra";
	address1.pincode = 4000054;

	employee1.empId = 101;
	employee1.empName = "Rushi";

	employee1.addr = &address1;

	company1.companyName = "Scan Antivirus";

	company1.emp = &employee1;

	cout<<"Company Name : "<<company1.companyName <<endl;
	cout<<"Employee ID : "<<company1.emp->empId <<endl;
	cout<<"Employee Name : "<<company1.emp->empName<<endl;
	cout<<"City : "<<company1.emp->addr->city<<endl;
	cout<<"State : "<<company1.emp->addr->state<<endl;
	cout<<"Pincode : "<<company1.emp->addr->pincode<<endl;

	return 0;

}
