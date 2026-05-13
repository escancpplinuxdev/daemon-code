#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Employee
{
public:
    int id;
    string name;

    Employee(int i, string n)
    {
        id = i;
        name = n;
    }

    void display() const
    {
        cout << "ID = " << id
             << " Name = " << name
             << endl;
    }
};

void printVector(const vector<int>& v, string message)
{
    cout << "\n" << message << endl;

    cout << "Size     = " << v.size() << endl;
    cout << "Capacity = " << v.capacity() << endl;

    cout << "Elements = ";

    for(const auto & x : v)
    {
        cout << x << " ";
    }

    cout << endl;
}

int main()
{
    cout << "==============================" << endl;
    cout << "VECTOR INTERVIEW DEMO PROGRAM" << endl;
    cout << "==============================" << endl;

    /*
        ------------------------------------------------
        1. VECTOR DECLARATION
        ------------------------------------------------
    */

    vector<int> v;

    cout << "\nInitial Vector" << endl;

    cout << "Size     = " << v.size() << endl;
    cout << "Capacity = " << v.capacity() << endl;

    /*
        ------------------------------------------------
        2. PUSH_BACK()
        ------------------------------------------------
    */

    cout << "\nPUSH_BACK DEMO" << endl;

    for(int i = 1; i <= 10; i++)
    {
        v.push_back(i * 10);

        cout << "Inserted : "
             << i * 10
             << "  Size = "
             << v.size()
             << "  Capacity = "
             << v.capacity()
             << endl;
    }

    printVector(v, "After push_back()");

    /*
        ------------------------------------------------
        3. RANDOM ACCESS
        ------------------------------------------------
    */

    cout << "\nRANDOM ACCESS" << endl;

    cout << "v[0]      = " << v[0] << endl;
    cout << "v.at(1)   = " << v.at(1) << endl;
    cout << "front()   = " << v.front() << endl;
    cout << "back()    = " << v.back() << endl;

    /*
        ------------------------------------------------
        4. ITERATOR
        ------------------------------------------------
    */

    cout << "\nITERATOR DEMO" << endl;

    for(vector<int>::iterator it = v.begin();
        it != v.end();
        ++it)
    {
        cout << *it << " ";
    }

    cout << endl;

    /*
        ------------------------------------------------
        5. RANGE BASED LOOP
        ------------------------------------------------
    */

    cout << "\nRANGE BASED LOOP" << endl;

    for(const auto & x : v)
    {
        cout << x << " ";
    }

    cout << endl;

    /*
        ------------------------------------------------
        6. INSERT
        ------------------------------------------------
    */

    cout << "\nINSERT DEMO" << endl;

    v.insert(v.begin() + 2, 999);

    printVector(v, "After insert at position 2");

    /*
        ------------------------------------------------
        7. ERASE
        ------------------------------------------------
    */

    cout << "\nERASE DEMO" << endl;

    v.erase(v.begin() + 3);

    printVector(v, "After erase");

    /*
        ------------------------------------------------
        8. POP_BACK
        ------------------------------------------------
    */

    cout << "\nPOP_BACK DEMO" << endl;

    v.pop_back();

    printVector(v, "After pop_back()");

    /*
        ------------------------------------------------
        9. RESERVE
        ------------------------------------------------
    */

    cout << "\nRESERVE DEMO" << endl;

    vector<int> reserveVector;

    reserveVector.reserve(100);

    cout << "Reserved Capacity = "
         << reserveVector.capacity()
         << endl;

    /*
        ------------------------------------------------
        10. RESIZE
        ------------------------------------------------
    */

    cout << "\nRESIZE DEMO" << endl;

    reserveVector.resize(10);

    cout << "Size     = "
         << reserveVector.size()
         << endl;

    cout << "Capacity = "
         << reserveVector.capacity()
         << endl;

    /*
        ------------------------------------------------
        11. EMPLACE_BACK
        ------------------------------------------------
    */

    cout << "\nEMPLACE_BACK DEMO" << endl;

    vector<Employee> employees;

    employees.emplace_back(101, "Rushikesh");
    employees.emplace_back(102, "Amit");

    for(const auto & e : employees)
    {
        e.display();
    }

    /*
        ------------------------------------------------
        12. SORT
        ------------------------------------------------
    */

    cout << "\nSORT DEMO" << endl;

    vector<int> sortVector =
    {
        55,
        11,
        99,
        22,
        66
    };

    sort(sortVector.begin(),
         sortVector.end());

    for(const auto & x : sortVector)
    {
        cout << x << " ";
    }

    cout << endl;

    /*
        ------------------------------------------------
        13. REVERSE
        ------------------------------------------------
    */

    cout << "\nREVERSE DEMO" << endl;

    reverse(sortVector.begin(),
            sortVector.end());

    for(const auto & x : sortVector)
    {
        cout << x << " ";
    }

    cout << endl;

    /*
        ------------------------------------------------
        14. VECTOR OF STRINGS
        ------------------------------------------------
    */

    cout << "\nVECTOR<STRING> DEMO" << endl;

    vector<string> names;

    names.push_back("Linux");
    names.push_back("C++");
    names.push_back("Kernel");

    for(const auto & s : names)
    {
        cout << s << endl;
    }

    /*
        ------------------------------------------------
        15. POINTER INVALIDATION
        ------------------------------------------------
    */

    cout << "\nPOINTER INVALIDATION DEMO" << endl;

    vector<int> pointerVector;

    pointerVector.push_back(10);
    pointerVector.push_back(20);

    int* ptr = &pointerVector[0];

    cout << "Before Reallocation Address = "
         << ptr
         << endl;

    for(int i = 0; i < 100; i++)
    {
        pointerVector.push_back(i);
    }

    cout << "After Reallocation Address = "
         << &pointerVector[0]
         << endl;

    cout << "\nOld pointer may become dangling!"
         << endl;

    /*
        ------------------------------------------------
        16. CLEAR
        ------------------------------------------------
    */

    cout << "\nCLEAR DEMO" << endl;

    v.clear();

    cout << "After clear()" << endl;

    cout << "Size     = "
         << v.size()
         << endl;

    cout << "Capacity = "
         << v.capacity()
         << endl;

    /*
        ------------------------------------------------
        17. EMPTY
        ------------------------------------------------
    */

    cout << "\nEMPTY DEMO" << endl;

    if(v.empty())
    {
        cout << "Vector is empty" << endl;
    }

    /*
        ------------------------------------------------
        18. SHRINK_TO_FIT
        ------------------------------------------------
    */

    cout << "\nSHRINK_TO_FIT DEMO" << endl;

    vector<int> shrinkVector;

    shrinkVector.reserve(100);

    shrinkVector.push_back(1);
    shrinkVector.push_back(2);

    cout << "Before shrink_to_fit()" << endl;

    cout << "Size     = "
         << shrinkVector.size()
         << endl;

    cout << "Capacity = "
         << shrinkVector.capacity()
         << endl;

    shrinkVector.shrink_to_fit();

    cout << "After shrink_to_fit()" << endl;

    cout << "Size     = "
         << shrinkVector.size()
         << endl;

    cout << "Capacity = "
         << shrinkVector.capacity()
         << endl;

    /*
        ------------------------------------------------
        19. FINAL INTERVIEW SUMMARY
        ------------------------------------------------
    */

    cout << "\n====================================" << endl;
    cout << "IMPORTANT INTERVIEW POINTS" << endl;
    cout << "====================================" << endl;

    cout << "1. Vector uses contiguous memory" << endl;
    cout << "2. push_back() is amortized O(1)" << endl;
    cout << "3. insert/erase middle is O(n)" << endl;
    cout << "4. Reallocation invalidates iterators" << endl;
    cout << "5. reserve() avoids reallocations" << endl;
    cout << "6. emplace_back() avoids extra copies" << endl;
    cout << "7. Vector provides cache locality" << endl;

    cout << "\nProgram Finished Successfully" << endl;

    return 0;
}
