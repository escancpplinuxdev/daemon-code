#include <iostream>
using namespace std;

int main()
{
    int a[] = {5,1,15,20,25};
    int i, j, k;
    i = ++a[1];
    j = a[1]++;
    k = a[i++];

    cout << i << " " << j << " " << k;
    return 0;
}

/*

Here is the detailed, step-by-step evaluation of this code.

This question tests your understanding of Pre-increment (++x), Post-increment (x++), and Sequence Points (the order in which expressions are evaluated).
The Initial Array

Before the statements, the array looks like this:

    a[0] = 5

    a[1] = 1

    a[2] = 15

    a[3] = 20

    a[4] = 25

Step-by-Step Execution
1. i = ++a[1];

    The Logic: The ++ is on the left (pre-increment). This means "increment the value first, then use the new value".

    a[1] is currently 1.

    It is incremented to 2. The array is now: {5, 2, 15, 20, 25}.

    The new value (2) is assigned to i.

    Current State: i = 2, a[1] = 2.

2. j = a[1]++;

    The Logic: The ++ is on the right (post-increment). This means "use the current value first, then increment".

    The current value of a[1] is 2. This value is assigned to j.

    After the assignment, a[1] is incremented to 3.

    Current State: j = 2, a[1] = 3.

3. k = a[i++];

This is the trickiest line. It contains a post-increment inside the array bracket. It breaks down like this:

    The compiler looks at i. Currently, i = 2.

    Because it is i++ (post-increment), it uses the current value of i (which is 2) as the index for the array. So it looks up a[2].

    a[2] is 15. This value is assigned to k.

    After the array lookup, the value of i is incremented to 3.

    Current State: k = 15, i = 3.

The Final Output

Looking at the final values of the variables requested:

    i is 3

    j is 2

    k is 15

Output:
text

3 2 15

Important Note on Undefined Behavior (Advanced)

If you were to write a line like k = a[i++] + i;, the behavior would be Undefined in C++ (prior to C++17). This is because the compiler wouldn't know whether to evaluate the i++ first (changing i to 3) or evaluate the i on the right side first (using the old value of 2).

In your code, the evaluation is perfectly safe because there is a sequence point (the ;) between every statement, and inside the array bracket a[i++], the evaluation is strictly ordered: read the index, fetch the value, then increment the index.

*/
