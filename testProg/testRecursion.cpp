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
 *
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
[root@PTLD-3:/tmpdata/Rushikesh_27022026/testProg]# ./testRecursion 
F(3)'s Stack Frame Pushed
F(2)'s Stack Frame Pushed
F(1)'s Stack Frame Pushed
F(1)'s Stack Frame Removed
F(1)'s Stack Frame Pushed
F(1)'s Stack Frame Removed
F(2)'s Stack Frame Removed
F(2)'s Stack Frame Pushed
F(1)'s Stack Frame Pushed
F(1)'s Stack Frame Removed
F(1)'s Stack Frame Pushed
F(1)'s Stack Frame Removed
F(2)'s Stack Frame Removed
F(3)'s Stack Frame Removed

 *
 *
 */
