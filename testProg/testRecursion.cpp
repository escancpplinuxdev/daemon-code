#include <iostream>
using namespace std;

void f(int n) {
    
    cout << "F(" << n << ")'s Stack Frame Pushed\n";
    if (n > 1) {
        f(n - 1);
        f(n - 1);
    }
    cout << "F(" << n << ")'s Stack Frame Removed\n";
}

int main() {
    f(3);
    return 0;
}

/*
 * push n=3 -> n-1=2
 *	push n=2 -> n-1=1
 *		   push n=1 -> 
 *		   Remove   n=1 
 *
 *           n=2 ->  n-1=1 
 *           	    push n=1 -> 
 *           	    Remove  n=1
 *
 *	Remove	n=2 
 *	n=3 -> n-1=2
 *	push n=2 -> n-1=1
 *		   push n=1 -> 
 *		   Remove   n=1 
 *
 *           n=2 ->  n-1=1 
 *           	    push n=1 -> 
 *           	    Remove  n=1
 *
 *      Remove  n=2
 *Remove n=3	
 *
 *
