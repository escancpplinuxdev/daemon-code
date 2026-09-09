#include <iostream>
#include <memory> // all smart pointer header // note: ‘std::unique_ptr’ is defined in header ‘#include <memory>’

class Resource 
{
	public:
		Resource() { std::cout << "Resource Acquired\n"; } // ctor
		~Resource() { std::cout << "Resource Destroyed\n"; }  // dtor
		void doWork() { std::cout << "Working...\n"; }
};

void processResource(std::unique_ptr<Resource> res) 
{
	res->doWork();
} // res goes out of scope here and automatically deletes the resource

int main() 
{
	// 1. Prefer make_unique for safety and readability
	auto uPtr = std::make_unique<Resource>(); 

	// 2. Ownership Transfer: Copying fails, moving transfers exclusive rights
	// processResource(uPtr); // Compile Error!
	processResource(std::move(uPtr)); // Valid. uPtr becomes null.

	if(uPtr == nullptr) 	std::cout<<"uPtr becomes null \n"  ;


	return 0;
}
