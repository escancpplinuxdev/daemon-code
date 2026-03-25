#include <iostream> //cin cout
#include <unistd.h> //sleep(5)
#include <condition_variable> 
#include <mutex>
#include <thread>
#include <atomic>
// --- Global variable ---
std::condition_variable cv;
int a=0;
std::mutex mt;
bool turn_t1 = true; // boolean to track whose turn it is
//std::atomic<int> t = 1;
int t = 1;
void print(int a)
{
	std::cout<<" "<<a<<"\n";
}

void t1Add()
{
	std::cout<<"t1 start (Addition)\n";
	int b=5, c=10;
		
	for (int i = 0 ; i< 10 ; i++)
	{	
		// step 1 : lock the mutex before touching share area
		std::unique_lock<std::mutex> lock (mt);
		// step 2 : wait until its T1's turn
		cv.wait(lock, [] {return turn_t1; });
	//	sleep(1);
		a = (b + i)+ c ;
		std::cout<< t++<<"] Addition "<<(b+i)<<" + "<<c <<" = ";
		print(a);
		turn_t1 = false;
		cv.notify_one();
	}

	std::cout<<"t1 end\n";
	
}

void t2Multiply()
{
	std::cout<<"t2 start (Multiplification)\n";
	int b=2, c=4;
	for (int i = 0 ; i< 10 ; i++)
	{
		// step 1 : lock the mutex 
		std::unique_lock<std::mutex> lock(mt);
		// step 2 : wait until T1 sets turn_T1 to false
		cv.wait(lock, [] {return !turn_t1; });
		
		// step 3 : perform the work
		a = (b+i) * c ;
	//	sleep(1);
		std::cout<<t++<<"] Multipy "<<(b+i)<<" * "<<c <<" = ";
		print(a);

		// step 4 : Hand over the turn back to T1 and notify it
		turn_t1 = true;
		cv.notify_one();
	}


	std::cout<<"t2 end\n";

}

int main()
{
	std::thread t1(t1Add);
	std::thread t2(t2Multiply);


	t1.join();
	t2.join();

	std::cout<< "All operation complete .\n";
	return 0;
}
