#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <vector>

using namespace std;

// ============================================================
// CLASS DEMONSTRATING CONSTRUCTOR/DESTRUCTOR CALLS
// ============================================================
class Calculator {
private:
    double value;
    static int objectCount;
    
public:
    // Constructor
    Calculator(double val = 0.0) : value(val) {
        objectCount++;
        cout << "🔧 Calculator object created! (Count: " << objectCount << ")" << endl;
    }
    
    // Destructor
    ~Calculator() {
        cout << "🗑️  Calculator object destroyed! (Value: " << value << ")" << endl;
        objectCount--;
    }
    
    // Member functions
    double getValue() const { return value; }
    void setValue(double val) { value = val; }
    double square() const { return value * value; }
    double cube() const { return value * value * value; }
    
    static int getObjectCount() { return objectCount; }
};

int Calculator::objectCount = 0;

// ============================================================
// DEMONSTRATION FUNCTIONS
// ============================================================

// 1. Basic new[] and delete[] for primitive types
void demoPrimitiveArray() {
    cout << "\n========== 1. PRIMITIVE ARRAY ==========\n";
    
    // Allocate array of 10 integers
    int* arr = new int[10];
    
    // Initialize values
    for (int i = 0; i < 10; i++) {
        arr[i] = i * 10;
    }
    
    // Display values
    cout << "Array values: ";
    for (int i = 0; i < 10; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    
    // Clean up
    delete[] arr;  // ✅ IMPORTANT: Use delete[] for arrays
    cout << "✅ Array deleted successfully!\n";
}

// 2. new[] and delete[] with objects
void demoObjectArray() {
    cout << "\n========== 2. OBJECT ARRAY ==========\n";
    
    // Allocate array of 5 Calculator objects
    Calculator* calcs = new Calculator[5];
    
    // Initialize each object
    for (int i = 0; i < 5; i++) {
        calcs[i].setValue(i * 2.5);
    }
    
    // Display values
    cout << "Object values: ";
    for (int i = 0; i < 5; i++) {
        cout << calcs[i].getValue() << " ";
    }
    cout << endl;
    
    // Clean up (destructor called for each object)
    delete[] calcs;  // ✅ Destructor called 5 times
    cout << "✅ All objects deleted!\n";
}

// 3. new[] with initialization
void demoInitialization() {
    cout << "\n========== 3. INITIALIZATION ==========\n";
    
    // Uninitialized (garbage values)
    int* arr1 = new int[10];
    cout << "Uninitialized first element: " << arr1[0] << " (garbage)\n";
    
    // Zero-initialized
    int* arr2 = new int[10]();
    cout << "Zero-initialized first element: " << arr2[0] << endl;
    
    // Initializer list (C++11)
    int* arr3 = new int[10]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    cout << "Initializer list first 3: " << arr3[0] << ", " << arr3[1] << ", " << arr3[2] << endl;
    
    // Clean up
    delete[] arr1;
    delete[] arr2;
    delete[] arr3;
}

// 4. 2D array with new[] and delete[]
void demo2DArray() {
    cout << "\n========== 4. 2D ARRAY ==========\n";
    
    int rows = 3, cols = 4;
    
    // Allocate array of pointers (rows)
    int** matrix = new int*[rows];
    
    // Allocate each row (columns)
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }
    
    // Initialize
    int counter = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = counter++;
        }
    }
    
    // Display
    cout << "2D Matrix:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << setw(4) << matrix[i][j] << " ";
        }
        cout << endl;
    }
    
    // Clean up: delete each row first, then delete rows pointer
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    cout << "✅ 2D array deleted!\n";
}

// 5. Dynamic array resizing
void demoResizing() {
    cout << "\n========== 5. RESIZING ARRAY ==========\n";
    
    int oldSize = 5;
    int newSize = 10;
    
    // Original array
    int* arr = new int[oldSize];
    for (int i = 0; i < oldSize; i++) {
        arr[i] = i + 1;
    }
    
    cout << "Original array: ";
    for (int i = 0; i < oldSize; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    
    // Resize: allocate new, copy, delete old
    int* newArr = new int[newSize];
    
    // Copy old data
    for (int i = 0; i < oldSize; i++) {
        newArr[i] = arr[i];
    }
    
    // Initialize new elements
    for (int i = oldSize; i < newSize; i++) {
        newArr[i] = (i + 1) * 10;
    }
    
    // Delete old array
    delete[] arr;
    arr = newArr;
    
    cout << "Resized array: ";
    for (int i = 0; i < newSize; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    
    // Clean up
    delete[] arr;
}

// 6. Exception safety with new[]
void demoExceptionSafety() {
    cout << "\n========== 6. EXCEPTION SAFETY ==========\n";
    
    try 
    {
        int size = 10;
        // std::nothrow prevents throwing exception
        int* arr = new(std::nothrow) int[size];
        
        if (arr == nullptr) {
            cout << "❌ Memory allocation failed!\n";
            return;
        }
        
        // Use the array
        for (int i = 0; i < size; i++) {
            arr[i] = i * i;
        }
        
        cout << "Array initialized successfully!\n";
        cout << "Last element: " << arr[size - 1] << endl;
        
        delete[] arr;
        
    } catch (const std::bad_alloc& e) {
        cout << "❌ Exception caught: " << e.what() << endl;
    }
}

// 7. Array of strings (dynamic)
void demoStringArray() {
    cout << "\n========== 7. STRING ARRAY ==========\n";
    
    int count = 5;
    string* names = new string[count];
    
    names[0] = "Alice";
    names[1] = "Bob";
    names[2] = "Charlie";
    names[3] = "Diana";
    names[4] = "Eve";
    
    cout << "Names: ";
    for (int i = 0; i < count; i++) {
        cout << names[i];
        if (i < count - 1) cout << ", ";
    }
    cout << endl;
    
    delete[] names;
}

// 8. Modern C++ alternatives (recommended)
void demoModernAlternatives() {
    cout << "\n========== 8. MODERN C++ ALTERNATIVES ==========\n";
    
    // Option 1: std::vector (recommended)
    cout << "Using std::vector:\n";
    vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int val : vec) {
        cout << val << " ";
    }
    cout << endl;
    
    // Option 2: std::unique_ptr (smart pointer)
    cout << "Using std::unique_ptr:\n";
    unique_ptr<int[]> arr = make_unique<int[]>(10);
    for (int i = 0; i < 10; i++) {
        arr[i] = i * 10;
    }
    for (int i = 0; i < 10; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    // No manual delete needed!
    
    // Option 3: std::array (fixed size)
    cout << "Using std::array:\n";
    array<int, 10> arr2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int val : arr2) {
        cout << val << " ";
    }
    cout << endl;
}

// ============================================================
// MAIN FUNCTION
// ============================================================

int main() {
    cout << "===========================================\n";
    cout << "   C++ new[] and delete[] Demonstration\n";
    cout << "===========================================\n";
    
    cout << "\n📚 Demonstrating array allocation with new[] and delete[]\n";
    cout << "Initial object count: " << Calculator::getObjectCount() << endl;
    
    // Run all demonstrations
    demoPrimitiveArray();
    demoObjectArray();
    demoInitialization();
    demo2DArray();
    demoResizing();
    demoExceptionSafety();
    demoStringArray();
    demoModernAlternatives();
    
    cout << "\n===========================================\n";
    cout << "Final object count: " << Calculator::getObjectCount() << endl;
    cout << "✅ All demonstrations completed!\n";
    cout << "===========================================\n";
    
    return 0;
}
