
#include <iostream>
#include <thread>
#include <atomic>

using namespace std;

atomic<int> counter(0);

void increment() 
{
    for (int i = 0; i < 1000; i++) 
	{
        counter++;  // Atomic operation
    }
}

int main() 
{
    thread t1(increment);
    thread t2(increment);

    t1.join();
    t2.join();

    cout << "Final Counter Value: " << counter << endl;
}



/*
You are missing the #include <atomic> header at the top of your file. Because std::atomic is defined in that header, the compiler does not recognize atomic<int> and throws cascading errors wherever counter is used.
Corrected Code
Add #include <atomic> right below #include <thread>:
cpp

#include <iostream>
#include <thread>
#include <atomic>  // <-- Add this line

using namespace std;

atomic<int> counter(0);

void increment() {
    for (int i = 0; i < 1000; i++) {
        counter++;  // Atomic operation
    }
}

int main() {
    thread t1(increment);
    thread t2(increment);

    t1.join();
    t2.join();

    cout << "Final Counter Value: " << counter << endl;
    return 0;
}

Use code with caution.
Compiling Note
If you get a linker error (such as undefined reference to pthread_create) when running make a, compile with the -pthread flag instead:
bash

g++ -pthread a.cpp -o a && ./a

Use code with caution.
If you run into any linker errors with make or want to explore memory ordering options for std::atomic, let me know and I can walk you through them!



*/
