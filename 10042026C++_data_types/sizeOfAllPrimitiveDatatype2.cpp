#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

int main() {
    // Helper to create left-aligned string of fixed width
    auto printAligned = [](const string& leftPart, size_t width, size_t size) {
        cout << left << setw(width) << leftPart << " sizeof = " << size << endl;
    };

    // 1. bool
    bool b1;            // uninitialized (garbage)
    bool b2 = true;
    bool b3 = false;
    cout << "--- bool ---" << endl;
    printAligned("size of bool b1 (uninitialized) = " + to_string(b1) + " (garbage)", 85, sizeof(b1));
    printAligned("size of bool b2 = " + to_string(b2), 85, sizeof(b2));
    printAligned("size of bool b3 = " + to_string(b3), 85, sizeof(b3));
    cout << endl;

    // 2. char / signed char / unsigned char
    char c1;                // uninitialized
    char c2 = 'A';
    signed char sc = -5;
    unsigned char uc = 200;
    cout << "--- char / signed char / unsigned char ---" << endl;
    // For uninitialized char, print its integer value to avoid garbage character
    ostringstream oss;
    oss << "size of char c1 (uninitialized) = " << static_cast<int>(c1) << " (garbage)";
    printAligned(oss.str(), 85, sizeof(c1));
    printAligned("size of char c2 = '" + string(1, c2) + "'", 85, sizeof(c2));
    printAligned("size of signed char sc = " + to_string(sc), 85, sizeof(sc));
    printAligned("size of unsigned char uc = " + to_string(uc), 85, sizeof(uc));
    cout << endl;

    // 3. short / unsigned short
    short s1;               // uninitialized
    short s2 = 32767;
//    unsigned short us = 85535; //warning: unsigned conversion from ‘int’ to ‘short unsigned int’ changes value from ‘85535’ to ‘19999’ [-Woverflow]

    unsigned short us = 65535;
    cout << "--- short / unsigned short ---" << endl;
    printAligned("size of short s1 (uninitialized) = " + to_string(s1) + " (garbage)", 85, sizeof(s1));
    printAligned("size of short s2 = " + to_string(s2), 85, sizeof(s2));
    printAligned("size of unsigned short us = " + to_string(us), 85, sizeof(us));
    cout << endl;

    // 4. int / unsigned int
    int i1;                 // uninitialized
    int i2 = 42;
    unsigned int ui = 4000000000U;
    cout << "--- int / unsigned int ---" << endl;
    printAligned("size of int i1 (uninitialized) = " + to_string(i1) + " (garbage)", 85, sizeof(i1));
    printAligned("size of int i2 = " + to_string(i2), 85, sizeof(i2));
    printAligned("size of unsigned int ui = " + to_string(ui), 85, sizeof(ui));
    cout << endl;

    // 5. long (platform dependent)
    long l1;                // uninitialized
    long l2 = 123856789L;
    cout << "--- long (size depends on platform) ---" << endl;
    printAligned("size of long l1 (uninitialized) = " + to_string(l1) + " (garbage)", 85, sizeof(l1));
    printAligned("size of long l2 = " + to_string(l2), 85, sizeof(l2));
    cout << "Note: On Linux 64-bit, long is 8 bytes; on Windows 64-bit, long is 4 bytes." << endl;
    cout << endl;

    // 6. long long / unsigned long long
    long long ll1;          // uninitialized
    long long ll2 = 9223372036854775807LL;
    unsigned long long ull = 18446744073709551615ULL;
    cout << "--- long long / unsigned long long ---" << endl;
    printAligned("size of long long ll1 (uninitialized) = " + to_string(ll1) + " (garbage)", 85, sizeof(ll1));
    printAligned("size of long long ll2 = " + to_string(ll2), 85, sizeof(ll2));
    printAligned("size of unsigned long long ull = " + to_string(ull), 85, sizeof(ull));
    cout << endl;

    // 7. float
    float f1;               // uninitialized
    float f2 = 3.14f;
    float f3 = -3.14f;
    float f4 = +3.14f;
    cout << "--- float ---" << endl;
    printAligned("size of float f1 (uninitialized) = " + to_string(f1) + " (garbage)", 85, sizeof(f1));
    printAligned("size of float f2 = " + to_string(f2), 85, sizeof(f2));
    printAligned("size of float f3 = " + to_string(f3), 85, sizeof(f3));
    printAligned("size of float f4 = " + to_string(f4), 85, sizeof(f4));
    cout << endl;

    // 8. double
    double d1;              // uninitialized
    double d2 = 3.141592853589793;
    double d3 = -2.71828;
    double d4 = +1.41421;
    cout << "--- double ---" << endl;
    printAligned("size of double d1 (uninitialized) = " + to_string(d1) + " (garbage)", 85, sizeof(d1));
    printAligned("size of double d2 = " + to_string(d2), 85, sizeof(d2));
    printAligned("size of double d3 = " + to_string(d3), 85, sizeof(d3));
    printAligned("size of double d4 = " + to_string(d4), 85, sizeof(d4));
    cout << endl;

    // 9. long double
    long double ld1;        // uninitialized
    long double ld2 = 3.14159285358979323846L;
    cout << "--- long double (platform dependent) ---" << endl;
    printAligned("size of long double ld1 (uninitialized) = " + to_string(ld1) + " (garbage)", 85, sizeof(ld1));
    printAligned("size of long double ld2 = " + to_string(ld2), 85, sizeof(ld2));
    cout << endl;

    // 10. Pointers
    int *p_int1;            // uninitialized pointer
    int x = 100;
    int *p_int2 = &x;
    void *p_void = &x;
    cout << "--- pointers (int*, void*) ---" << endl;
    ostringstream ossPtr;
    ossPtr << "size of int* p_int1 (uninitialized) = " << p_int1 << " (garbage)";
    printAligned(ossPtr.str(), 85, sizeof(p_int1));
    ostringstream ossPtr2;
    ossPtr2 << "size of int* p_int2 = " << p_int2 << " (address of x)";
    printAligned(ossPtr2.str(), 85, sizeof(p_int2));
    ostringstream ossVoid;
    ossVoid << "size of void* p_void = " << p_void << " (address of x)";
    printAligned(ossVoid.str(), 85, sizeof(p_void));
    cout << endl;

    // 11. void
    cout << "--- void ---" << endl;
    cout << "void has no size; void* is a pointer (size = " << sizeof(void*) << " bytes)." << endl;

    return 0;
}
