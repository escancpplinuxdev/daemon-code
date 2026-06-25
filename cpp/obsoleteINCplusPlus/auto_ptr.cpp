
#include <iostream>
#include <memory>

int main()
{
	std::auto_ptr<int>ptr1(new int(42));

	std::cout<<"*ptr1 = "<<*ptr1<<"\n";

	std::auto_ptr<int> ptr2 = ptr1;     // Copying transfers ownership implicitly!
	/*	
		if(ptr1) //error: could not convert ‘ptr1’ from ‘std::auto_ptr<int>’ to ‘bool’
		std::cout<<"*ptr1 = "<<*ptr1<<"\n"; 
		else
		std::cout<<"*ptr1 = NULL\n";
	 */
	std::cout<<"*ptr2 = "<<*ptr2<<"\n";


	//================= auto_ptr replace by unique_ptr ==================//

	std::unique_ptr<int>ptr3 = std::make_unique<int>(42);

	std::cout<<"*ptr3 = "<<*ptr3<<"\n";

	std::unique_ptr<int>ptr4 = std::move(ptr3);

	if(ptr3)  // in unique_ptr we can check by this , but this give error in auto_ptr
	{
		std::cout<<"*ptr3 = "<<*ptr3<<"\n"; 
	}
	else
	{
		std::cout<<"*ptr3 = NULL\n";
	}
	std::cout<<"*ptr4 = "<<*ptr4<<"\n";

	return 0;
}

//std::auto_ptr was flawed because copying it secretly transferred ownership, leaving the original pointer null. This caused crashes when used in loops or containers. Modern C++ replaces it with std::unique_ptr, which forces you to explicitly use std::move().

