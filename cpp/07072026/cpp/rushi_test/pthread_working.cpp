
// g++ b.cpp -o b /lib/x86_64-linux-gnu/libpthread.so.0 && valgrind --leak-check=full -s ./b
#include <iostream>
#include <pthread.h>
#include <unistd.h> // For sleep() simulation

// The thread worker function must return void* and take void* as an argument
void* FileScan(void* arg) {
    std::cout << "[Thread] File scan worker thread started successfully...\n";
    
    // Simulating background file processing work
    for (int i = 1; i <= 3; ++i) {
        std::cout << "[Thread] Scanning directory sector " << i << "...\n";
        sleep(1); 
    }

    std::cout << "[Thread] File scan complete. Exiting thread cleanly.\n";
    
    // Correct usage of pthread_exit: Terminates the *current* thread 
    // and can optionally pass a return pointer back to pthread_join.
    pthread_exit(NULL); 
}

int main() {
    std::cout << "[Main] Launching background processing engine...\n";

    pthread_t threadFileScan;

    // Launch the thread executing the FileScan worker function
    if (pthread_create(&threadFileScan, NULL, FileScan, NULL) != 0) {
        std::cerr << "[Main Error] Failed to create the file scanning thread!\n";
        return 1;
    }

    std::cout << "[Main] Thread spawned. Main routine continues executing concurrently...\n";
    sleep(1); // Simulating main thread work moving parallelly

    std::cout << "[Main] Waiting for file scan thread to finish (joining)...\n";
    
    // Suspend execution of main thread until threadFileScan terminates safely
    if (pthread_join(threadFileScan, NULL) != 0) {
        std::cerr << "[Main Error] Failed to join the thread properly!\n";
        return 1;
    }

    std::cout << "[Main] Thread successfully re-joined. All background assets closed.\n";
    return 0;
}




