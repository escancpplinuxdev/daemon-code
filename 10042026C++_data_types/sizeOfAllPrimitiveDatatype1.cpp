#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    // Helper to print a line with fixed width for the first column
    auto printLine = [](const string& desc, const string& val, size_t size) {
        cout << left << setw(50) << (desc + " = " + val) 
             << " sizeof = " << size << endl;
    };

    // 1. bool
    bool b1;            // uninitialized (garbage)
    bool b2 = true;
    bool b3 = false;
    cout << "\n--- bool ---\n";
    printLine("size of bool b1 (uninitialized)", to_string(b1) + " (garbage)", sizeof(b1));
    printLine("size of bool b2", to_string(b2), sizeof(b2));
    printLine("size of bool b3", to_string(b3), sizeof(b3));

    // 2. char / signed char / unsigned char
    char c1;                // uninitialized
    char c2 = 'A';
    signed char sc = -5;
    unsigned char uc = 200;
    cout << "\n--- char / signed char / unsigned char ---\n";
    printLine("size of char c1 (uninit)", string("'") + c1 + "' (garbage)", sizeof(c1));
    printLine("size of char c2", string("'") + c2 + "'", sizeof(c2));
    printLine("size of signed char sc", to_string(sc), sizeof(sc));
    printLine("size of unsigned char uc", to_string(uc), sizeof(uc));

    // 3. short / unsigned short
    short s1;               // uninitialized
    short s2 = 32767;
    unsigned short us = 65535;
    cout << "\n--- short / unsigned short ---\n";
    printLine("size of short s1 (uninit)", to_string(s1) + " (garbage)", sizeof(s1));
    printLine("size of short s2", to_string(s2), sizeof(s2));
    printLine("size of unsigned short us", to_string(us), sizeof(us));

    // 4. int / unsigned int
    int i1;                 // uninitialized
    int i2 = 42;
    unsigned int ui = 4000000000U;
    cout << "\n--- int / unsigned int ---\n";
    printLine("size of int i1 (uninit)", to_string(i1) + " (garbage)", sizeof(i1));
    printLine("size of int i2", to_string(i2), sizeof(i2));
    printLine("size of unsigned int ui", to_string(ui), sizeof(ui));

    // 5. long
    long l1;                // uninitialized
    long l2 = 123456789L;
    cout << "\n--- long (size depends on platform) ---\n";
    printLine("size of long l1 (uninit)", to_string(l1) + " (garbage)", sizeof(l1));
    printLine("size of long l2", to_string(l2), sizeof(l2));
    cout << "Note: On Linux 64-bit, long is 8 bytes; on Windows 64-bit, long is 4 bytes.\n";

    // 6. long long / unsigned long long
    long long ll1;          // uninitialized
    long long ll2 = 9223372036854775807LL;
    unsigned long long ull = 18446744073709551615ULL;
    cout << "\n--- long long / unsigned long long ---\n";
    printLine("size of long long ll1 (uninit)", to_string(ll1) + " (garbage)", sizeof(ll1));
    printLine("size of long long ll2", to_string(ll2), sizeof(ll2));
    printLine("size of unsigned long long ull", to_string(ull), sizeof(ull));

    // 7. float
    float f1;               // uninitialized
    float f2 = 3.14f;
    float f3 = -3.14f;
    float f4 = +3.14f;
    cout << "\n--- float ---\n";
    printLine("size of float f1 (uninit)", to_string(f1) + " (garbage)", sizeof(f1));
    printLine("size of float f2", to_string(f2), sizeof(f2));
    printLine("size of float f3", to_string(f3), sizeof(f3));
    printLine("size of float f4", to_string(f4), sizeof(f4));

    // 8. double
    double d1;              // uninitialized
    double d2 = 3.141592653589793;
    double d3 = -2.71828;
    double d4 = +1.41421;
    cout << "\n--- double ---\n";
    printLine("size of double d1 (uninit)", to_string(d1) + " (garbage)", sizeof(d1));
    printLine("size of double d2", to_string(d2), sizeof(d2));
    printLine("size of double d3", to_string(d3), sizeof(d3));
    printLine("size of double d4", to_string(d4), sizeof(d4));

    // 9. long double
    long double ld1;        // uninitialized
    long double ld2 = 3.14159265358979323846L;
    cout << "\n--- long double (platform dependent) ---\n";
    printLine("size of long double ld1 (uninit)", to_string(ld1) + " (garbage)", sizeof(ld1));
    printLine("size of long double ld2", to_string(ld2), sizeof(ld2));

    // 10. Pointers
    int *p_int1;            // uninitialized pointer
    int x = 100;
    int *p_int2 = &x;
    void *p_void = &x;
    cout << "\n--- pointers (int*, void*) ---\n";
    // Use hex for pointer values
    auto ptrToStr = [](void* p) {
        stringstream ss;
        ss << "0x" << hex << reinterpret_cast<uintptr_t>(p);
        return ss.str();
    };
    printLine("size of int* p_int1 (uninit)", "(garbage)", sizeof(p_int1));
    printLine("size of int* p_int2", ptrToStr(p_int2) + " (address of x)", sizeof(p_int2));
    printLine("size of void* p_void", ptrToStr(p_void) + " (address of x)", sizeof(p_void));

    // 11. void
    cout << "\n--- void ---\n";
    cout << left << setw(50) << "void has no size; void* is a pointer" 
         << " sizeof = " << sizeof(void*) << " bytes" << endl;

    return 0;
}
