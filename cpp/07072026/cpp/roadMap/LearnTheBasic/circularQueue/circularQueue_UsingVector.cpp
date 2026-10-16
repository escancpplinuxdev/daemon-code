#include <iostream>
#include <vector>
using namespace std;

class CircularQueue
{
private:
    vector<int> arr;
    int front;
    int rear;
    int capacity;

public:

    CircularQueue(int size)
        : arr(size), front(-1), rear(-1), capacity(size)
    {
    }

    bool isEmpty() const
    {
        return front == -1;
    }

    bool isFull() const
    {
        return (rear + 1) % capacity == front;
    }

    void enqueue(int value)
    {
        if (isFull())
        {
            cout << "Queue Overflow! Cannot insert "
                 << value << endl;
            return;
        }

        if (isEmpty())
        {
            front = 0;
            rear = 0;
        }
        else
        {
            rear = (rear + 1) % capacity;
        }

        arr[rear] = value;

        cout << "Inserted: " << value << endl;
        cout << "Front index: " << front << endl;
        cout << "Rear index: " << rear << endl;
    }

    void dequeue()
    {
        if (isEmpty())
        {
            cout << "Queue Underflow!" << endl;
            return;
        }

        int value = arr[front];

        if (front == rear)
        {
            front = -1;
            rear = -1;
        }
        else
        {
            front = (front + 1) % capacity;
        }

        cout << "Deleted: " << value << endl;
        cout << "Front index: " << front << endl;
        cout << "Rear index: " << rear << endl;
    }

    void peek() const
    {
        if (isEmpty())
        {
            cout << "Queue is empty!" << endl;
            return;
        }

        cout << "Front element: " << arr[front] << endl;
    }

    void display() const
    {
        if (isEmpty())
        {
            cout << "Queue is empty!" << endl;
            return;
        }

        cout << "Queue elements: ";

        int i = front;

        while (true)
        {
            cout << arr[i] << " ";

            if (i == rear)
            {
                break;
            }

            i = (i + 1) % capacity;
        }

        cout << endl;
    }
};

int main()
{
    CircularQueue q(5);

    cout << "\n--- Insert Elements ---\n";

    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    q.enqueue(40);
    q.enqueue(50);

    cout << "\n--- Queue Full ---\n";

    q.display();
    q.enqueue(60);

    cout << "\n--- Delete Elements ---\n";

    q.dequeue();
    q.dequeue();

    cout << "\n--- Insert Using Circular Movement ---\n";

    q.enqueue(60);
    q.enqueue(70);

    cout << "\n--- Final Queue ---\n";

    q.display();
    q.peek();

    cout << "\n--- Remove All Elements ---\n";

    q.dequeue();
    q.dequeue();
    q.dequeue();
    q.dequeue();
    q.dequeue();

    cout << "\n--- Empty Queue ---\n";

    q.display();

    return 0;
}

/*

Array vs. Linked List Circular Queue

    Fixed vs Dynamic Capacity:

        Array implementation requires a predefined fixed capacity.

        Linked List implementation grows and shrinks dynamically without capacity limits (no overflow unless system memory is exhausted).

    Memory Overhead:

        Array elements are stored contiguously without extra metadata.

        Linked List nodes store an extra 8-byte next pointer per element.

*/
