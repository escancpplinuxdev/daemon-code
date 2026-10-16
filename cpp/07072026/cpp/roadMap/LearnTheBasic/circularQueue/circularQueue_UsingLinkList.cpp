#include <iostream>
using namespace std;

// Node Definition
class Node {
public:
    int data;
    Node* next;

    Node(int val) {
        data = val;
        next = nullptr;
    }
};

// Circular Queue Class
class CircularQueueLinkedList {
private:
    Node* rear;
    int currSize;

public:
    // Constructor
    CircularQueueLinkedList() {
        rear = nullptr;
        currSize = 0;
    }

    // Destructor to clean up memory
    ~CircularQueueLinkedList() {
        while (!isEmpty()) {
            pop();
        }
    }

    // Check if empty - O(1)
    bool isEmpty() {
        return rear == nullptr;
    }

    // Push Operation - O(1)
    void push(int data) {
        Node* newNode = new Node(data);

        if (isEmpty()) {
            rear = newNode;
            rear->next = rear; // Points to itself
        } else {
            newNode->next = rear->next; // Point to front
            rear->next = newNode;       // Link old rear to new node
            rear = newNode;             // Update rear pointer
        }
        currSize++;
    }

    // Pop Operation - O(1)
    void pop() {
        if (isEmpty()) {
            cout << "Circular Queue is Empty\n";
            return;
        }

        Node* frontNode = rear->next;

        if (rear == frontNode) {
            // Case 1: Only one node in queue
            delete frontNode;
            rear = nullptr;
        } else {
            // Case 2: Multiple nodes
            rear->next = frontNode->next; // Bypass front node
            delete frontNode;
        }
        currSize--;
    }

    // Front Operation - O(1)
    int getFront() {
        if (isEmpty()) {
            cout << "Circular Queue is Empty\n";
            return -1;
        }
        return rear->next->data;
    }

    // Function to print elements in FIFO order
    void printQueue() {
        if (isEmpty()) {
            cout << "Queue is Empty\n";
            return;
        }

        Node* temp = rear->next; // Start from front
        do {
            cout << temp->data << " ";
            temp = temp->next;
        } while (temp != rear->next);
        cout << endl;
    }
};

int main() {
    CircularQueueLinkedList cq;

    cq.push(1);
    cq.push(2);
    cq.push(3);

    cout << "Queue elements: ";
    cq.printQueue(); // Output: 1 2 3

    cout << "Front element: " << cq.getFront() << endl; // Output: 1

    cq.pop(); // Removes 1
    cout << "After pop, Queue elements: ";
    cq.printQueue(); // Output: 2 3

    cq.push(4);
    cout << "After push 4, Queue elements: ";
    cq.printQueue(); // Output: 2 3 4

    return 0;
}
