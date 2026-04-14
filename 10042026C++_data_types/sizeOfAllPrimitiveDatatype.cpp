#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    // 1. bool
    bool b1;            // uninitialized (garbage)
    bool b2 = true;
    bool b3 = false;
    cout << "--- bool ---" << endl;
    cout << "size of bool b1 (uninitialized) = " << b1 << " (garbage) \t\t sizeof = " << sizeof(b1) << endl;
    cout << "size of bool b2 = " << b2 << " \t\t\t\t sizeof 			=" << sizeof(b2) << endl;
    cout << "size of bool b3 = " << b3 << " \t\t\t\t sizeof = " << sizeof(b3) << endl;
    cout << endl;

    // 2. char / signed char / unsigned char
    char c1;                // uninitialized
    char c2 = 'A';
    signed char sc = -5;
    unsigned char uc = 200;
    cout << "--- char / signed char / unsigned char ---" << endl;
    cout << "size of char c1 (uninit) = '" << c1 << "' (garbage) \t\t sizeof = " << sizeof(c1) << endl;
    cout << "size of char c2 = '" << c2 << "' \t\t\t\t sizeof = " << sizeof(c2) << endl;
    cout << "size of signed char sc = " << (int)sc << " \t\t\t sizeof = " << sizeof(sc) << endl;
    cout << "size of unsigned char uc = " << (int)uc << " \t\t\t sizeof = " << sizeof(uc) << endl;
    cout << endl;

    // 3. short / unsigned short
    short s1;               // uninitialized
    short s2 = 32767;
    unsigned short us = 65535;
    cout << "--- short / unsigned short ---" << endl;
    cout << "size of short s1 (uninit) = " << s1 << " (garbage) \t\t sizeof = " << sizeof(s1) << endl;
    cout << "size of short s2 = " << s2 << " \t\t\t\t sizeof = " << sizeof(s2) << endl;
    cout << "size of unsigned short us = " << us << " \t\t\t sizeof = " << sizeof(us) << endl;
    cout << endl;

    // 4. int / unsigned int
    int i1;                 // uninitialized
    int i2 = 42;
    unsigned int ui = 4000000000U;
    cout << "--- int / unsigned int ---" << endl;
    cout << "size of int i1 (uninit) = " << i1 << " (garbage) \t\t sizeof = " << sizeof(i1) << endl;
    cout << "size of int i2 = " << i2 << " \t\t\t\t sizeof = " << sizeof(i2) << endl;
    cout << "size of unsigned int ui = " << ui << " \t\t sizeof = " << sizeof(ui) << endl;
    cout << endl;

    // 5. long (platform dependent: Linux 64-bit = 8, Windows 64-bit = 4)
    long l1;                // uninitialized
    long l2 = 123456789L;
    cout << "--- long (size depends on platform) ---" << endl;
    cout << "size of long l1 (uninit) = " << l1 << " (garbage) \t\t sizeof = " << sizeof(l1) << endl;
    cout << "size of long l2 = " << l2 << " \t\t\t\t sizeof = " << sizeof(l2) << endl;
    cout << "Note: On Linux 64-bit, long is 8 bytes; on Windows 64-bit, long is 4 bytes." << endl;
    cout << endl;

    // 6. long long / unsigned long long
    long long ll1;          // uninitialized
    long long ll2 = 9223372036854775807LL;
    unsigned long long ull = 18446744073709551615ULL;
    cout << "--- long long / unsigned long long ---" << endl;
    cout << "size of long long ll1 (uninit) = " << ll1 << " (garbage) \t sizeof = " << sizeof(ll1) << endl;
    cout << "size of long long ll2 = " << ll2 << " \t sizeof = " << sizeof(ll2) << endl;
    cout << "size of unsigned long long ull = " << ull << " \t sizeof = " << sizeof(ull) << endl;
    cout << endl;

    // 7. float (already in your example, but included for completeness)
    float f1;               // uninitialized
    float f2 = 3.14f;
    float f3 = -3.14f;
    float f4 = +3.14f;
    cout << "--- float ---" << endl;
    cout << "size of float f1 (uninit) = " << f1 << " (garbage) \t\t sizeof = " << sizeof(f1) << endl;
    cout << "size of float f2 = " << f2 << " \t\t\t sizeof = " << sizeof(f2) << endl;
    cout << "size of float f3 = " << f3 << " \t\t\t sizeof = " << sizeof(f3) << endl;
    cout << "size of float f4 = " << f4 << " \t\t\t sizeof = " << sizeof(f4) << endl;
    cout << endl;

    // 8. double
    double d1;              // uninitialized
    double d2 = 3.141592653589793;
    double d3 = -2.71828;
    double d4 = +1.41421;
    cout << "--- double ---" << endl;
    cout << "size of double d1 (uninit) = " << d1 << " (garbage) \t\t sizeof = " << sizeof(d1) << endl;
    cout << "size of double d2 = " << d2 << " \t\t sizeof = " << sizeof(d2) << endl;
    cout << "size of double d3 = " << d3 << " \t\t sizeof = " << sizeof(d3) << endl;
    cout << "size of double d4 = " << d4 << " \t\t sizeof = " << sizeof(d4) << endl;
    cout << endl;

    // 9. long double (size varies: 10, 12, or 16 bytes on x86)
    long double ld1;        // uninitialized
    long double ld2 = 3.14159265358979323846L;
    cout << "--- long double (platform dependent) ---" << endl;
    cout << "size of long double ld1 (uninit) = " << ld1 << " (garbage) \t sizeof = " << sizeof(ld1) << endl;
    cout << "size of long double ld2 = " << ld2 << " \t\t sizeof = " << sizeof(ld2) << endl;
    cout << endl;

    // 10. Pointers (to int, to void)
    int *p_int1;            // uninitialized pointer
    int x = 100;
    int *p_int2 = &x;
    void *p_void = &x;
    cout << "--- pointers (int*, void*) ---" << endl;
    cout << "size of int* p_int1 (uninit) = " << p_int1 << " (garbage) \t sizeof = " << sizeof(p_int1) << endl;
    cout << "size of int* p_int2 = " << p_int2 << " (address of x) \t sizeof = " << sizeof(p_int2) << endl;
    cout << "size of void* p_void = " << p_void << " (address of x) \t sizeof = " << sizeof(p_void) << endl;
    cout << endl;

    // 11. void (cannot declare void variable, but void* already covered)
    cout << "--- void ---" << endl;
    cout << "void has no size; void* is a pointer (size = " << sizeof(void*) << " bytes)." << endl;

    return 0;
}
