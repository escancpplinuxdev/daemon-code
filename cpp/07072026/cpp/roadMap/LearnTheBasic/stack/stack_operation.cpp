#include <iostream>
#include <stack>

using namespace std;

// Passing by reference (&), but restoring elements afterward

void stackOperation(stack <int> s)
{
	// The stack now looks like this (Top to Bottom): [30, 20, 10]

    // 3. size() - Get number of elements
    cout << "Stack size: " << s.size() << endl; // Output: 3

    // 4. top() - View the topmost element
    cout << "Top element: " << s.top() << endl; // Output: 30

    // 5. pop() - Remove the topmost element
    s.pop(); // Removes 30. Stack is now: [20, 10]
    cout << "New top element after pop: " << s.top() << endl; // Output: 20

    // 6. empty() - Check if the stack is empty
    if (s.empty()) {
        cout << "Stack is empty." << endl;
    } else {
        cout << "Stack is not empty." << endl; // Output
    }

    // Traversal: How to print and empty the remaining elements
    while (!s.empty()) {
    cout << "\nRemaining elements in LIFO order: ";
        cout << s.top() << " ";
        s.pop();
    }
    cout << endl; // Output: 20 10
    cout << "Stack size after stack pop : " << s.size() << endl; // Output: 3
}

void printStackByTemporaryCopy(stack <int>st)
{
		cout<<"stack element = ";
	while(! st.empty())
	{
		cout<<st.top()<<" ";
		st.pop();
	}

}

void printStackRecursive(stack<int> &s)
{
  // Base case: if stack is empty, return
  if (s.empty())
    return;

  // 1. Store the top element
  int x = s.top();
  s.pop();

  // 2. Print it (Top to Bottom)
  cout << x << " ";

  // 3. Recurse for the remaining elements
  printStackRecursive(s);

  // 4. Push the element back to restore the original stack
  s.push(x);
}

int main()
{
  stack<int> s;
  s.push(10);
  s.push(20);
  s.push(30);

  cout << "Stack (Top to Bottom): ";
  printStackRecursive(s);
  cout << endl;

  // Verify original stack is still intact
  cout << "Original top element: " << s.top() << endl; // Output: 30
  cout << "size of stack : " << s.size() << endl; // Output: 30
  cout<<"\n\n\n";



  printStackByTemporaryCopy(s);
  cout << "\nOriginal top element: " << s.top() << endl; // Output: 30
  cout << "size of stack : " << s.size() << endl; // Output: 30
  cout<<"\n\n\n";


  stackOperation(s);
  cout<<"\n\n\n";


  return 0;
}
