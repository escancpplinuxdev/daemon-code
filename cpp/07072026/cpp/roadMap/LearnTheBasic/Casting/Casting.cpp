
#include <iostream>

class Animal
{
public:
/*
a.cpp:35:18: error: cannot ‘dynamic_cast’ ‘a’ (of type ‘class Animal*’) to type ‘class Dog*’ (source type is not polymorphic)
   35 |         Dog *b = dynamic_cast<Dog*>(a);
      |                  ^~~~~~~~~~~~~~~~~~~~~
make: *** [<builtin>: a] Error 1
so write virtual
*/
	virtual ~ Animal() = default; 

	void eat(){std::cout<<"eat biscut\n";}
};

class Dog : public Animal
{
public:
	void bark() { std::cout<<"Bhoo Bhoo\n";}
};

int main()
{
// upCasting
	Dog *d = new Dog();

	Animal *a = d;

		std::cout<<"Animal a upCasting\n";
	a->eat();
/*
	a->bark();
error: ‘class Animal’ has no member named ‘bark’
   24 |         a->bark();
      |            ^~~~
make: *** [<builtin>: a] Error 1
*/

	
// downCasting

	Dog *b = dynamic_cast<Dog*>(a);

	if(b != nullptr) 
	{
		std::cout<<"Dog b downCasting\n";
	 	b->bark();
	}
	else
	{
		std::cout<<"Dog b nullptr\n";
	}

	delete a;
	return 0;
}

/*


*/
