#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <vector>

// Helper to print a string with a label
void print(const std::string& label, const std::string& s) {
    std::cout << label << ": \"" << s << "\"" << std::endl;
}

int main() {
    // ------------------------------------------------------------------
    // 1. Initialization and assignment
    // ------------------------------------------------------------------
    std::string s1;                     // empty string
    std::string s2 = "Hello";           // C‑string assignment
    std::string s3("World");            // constructor
    std::string s4(5, 'A');             // "AAAAA"
    std::string s5(s2);                 // copy constructor
    std::string s6(s2, 1, 3);           // from s2, index 1, length 3 → "ell"

    print("s1", s1);
    print("s2", s2);
    print("s3", s3);
    print("s4", s4);
    print("s5", s5);
    print("s6", s6);

    // ------------------------------------------------------------------
    // 2. Length / Capacity
    // ------------------------------------------------------------------
    std::cout << "\n--- Length / Capacity ---\n";
    std::cout << "s2.length() = " << s2.length() << std::endl;       // 5
    std::cout << "s2.size()   = " << s2.size() << std::endl;         // same
    std::cout << "s2.capacity() = " << s2.capacity() << std::endl;
    std::cout << "s2.empty() = " << (s2.empty() ? "true" : "false") << std::endl;

    // ------------------------------------------------------------------
    // 3. Concatenation
    // ------------------------------------------------------------------
    std::cout << "\n--- Concatenation ---\n";
    std::string sum = s2 + " " + s3;     // operator+
    print("s2 + ' ' + s3", sum);
    s2 += " there";                      // append
    print("s2 after += \" there\"", s2);
    s2.append("!");
    print("s2.append(\"!\")", s2);
    s2.push_back('?');                   // add single char
    print("s2 after push_back('?')", s2);

    // ------------------------------------------------------------------
    // 4. Substring
    // ------------------------------------------------------------------
    std::cout << "\n--- Substring ---\n";
    std::string sub = s2.substr(6, 5);   // start at index 6, length 5
    print("substr(6,5) from s2", sub);
    std::string toEnd = s2.substr(6);    // from index 6 to end
    print("substr(6)", toEnd);

    // ------------------------------------------------------------------
    // 5. Searching (find, rfind, find_first_of, etc.)
    // ------------------------------------------------------------------
    std::cout << "\n--- Searching ---\n";
    std::string txt = "The quick brown fox jumps over the lazy dog";
    std::string word = "fox";
    size_t pos = txt.find(word);
    if (pos != std::string::npos)
        std::cout << "Found \"" << word << "\" at index " << pos << std::endl;
    else
        std::cout << "Not found" << std::endl;

    pos = txt.find('o');                 // first 'o'
    std::cout << "First 'o' at index " << pos << std::endl;
    pos = txt.rfind('o');                // last 'o'
    std::cout << "Last 'o' at index " << pos << std::endl;

    // find_first_of: any character from set
    pos = txt.find_first_of("aeiou");
    std::cout << "First vowel at index " << pos << " ('" << txt[pos] << "')" << std::endl;

    // find_first_not_of
    pos = txt.find_first_not_of("The ");
    std::cout << "First char not in \"The \" at index " << pos << " ('" << txt[pos] << "')" << std::endl;

    // ------------------------------------------------------------------
    // 6. Replace
    // ------------------------------------------------------------------
    std::cout << "\n--- Replace ---\n";
    std::string rep = "I like C++";
    rep.replace(7, 3, "Python");         // replace 3 chars starting at 7 with "Python"
    print("replace(7,3,\"Python\")", rep);
    rep.replace(rep.begin(), rep.begin()+1, "You");
    print("replace(begin, begin+1, \"You\")", rep);

    // ------------------------------------------------------------------
    // 7. Insert
    // ------------------------------------------------------------------
    std::cout << "\n--- Insert ---\n";
    std::string ins = "Hello World";
    ins.insert(5, " beautiful");         // insert at index 5
    print("insert(5, \" beautiful\")", ins);
    ins.insert(ins.begin()+6, 'X');      // insert character at position (iterator)
    print("insert(begin+6, 'X')", ins);

    // ------------------------------------------------------------------
    // 8. Erase
    // ------------------------------------------------------------------
    std::cout << "\n--- Erase ---\n";
    std::string eras = "1234567890";
    eras.erase(3, 4);                    // erase 4 chars starting at index 3 → "123890"
    print("erase(3,4)", eras);
    eras.erase(2);                       // erase from index 2 to end → "12"
    print("erase(2)", eras);
    eras.erase(eras.begin());            // erase first character → "2"
    print("erase(begin())", eras);

    // ------------------------------------------------------------------
    // 9. Comparison
    // ------------------------------------------------------------------
    std::cout << "\n--- Comparison ---\n";
    std::string a = "apple", b = "banana";
    int cmp = a.compare(b);
    if (cmp < 0) std::cout << a << " < " << b << std::endl;
    else if (cmp > 0) std::cout << a << " > " << b << std::endl;
    else std::cout << a << " == " << b << std::endl;

    if (a == "apple") std::cout << "a equals apple" << std::endl;
    if (a != "orange") std::cout << "a is not orange" << std::endl;

    // ------------------------------------------------------------------
    // 10. Case conversion (using algorithm)
    // ------------------------------------------------------------------
    std::cout << "\n--- Case conversion ---\n";
    std::string mixed = "Hello World";
    std::string lower = mixed;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    print("to lower", lower);
    std::string upper = mixed;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    print("to upper", upper);

    // ------------------------------------------------------------------
    // 11. Trimming whitespace (custom)
    // ------------------------------------------------------------------
    std::cout << "\n--- Trimming ---\n";
    std::string ws = "   Hello   ";
    size_t start = ws.find_first_not_of(" \t\n\r");
    size_t end = ws.find_last_not_of(" \t\n\r");
    std::string trimmed = (start == std::string::npos) ? "" : ws.substr(start, end - start + 1);
    print("trimmed", trimmed);

    // ------------------------------------------------------------------
    // 12. Splitting a string (using stringstream)
    // ------------------------------------------------------------------
    std::cout << "\n--- Splitting ---\n";
    std::string csv = "one,two,three,four";
    std::stringstream ss(csv);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    std::cout << "Split by ',': ";
    for (const auto& t : tokens) std::cout << "\"" << t << "\" ";
    std::cout << std::endl;

    // ------------------------------------------------------------------
    // 13. Joining (C++20 has join; here custom for earlier versions)
    // ------------------------------------------------------------------
    std::cout << "\n--- Joining ---\n";
    std::vector<std::string> parts = {"C++", "is", "powerful"};
    std::string joined;
    for (size_t i = 0; i < parts.size(); ++i) {
        joined += parts[i];
        if (i != parts.size()-1) joined += " ";
    }
    print("joined with spaces", joined);

    // ------------------------------------------------------------------
    // 14. Conversion to/from C‑style string
    // ------------------------------------------------------------------
    std::cout << "\n--- C-string conversion ---\n";
    const char* cstr = s2.c_str();          // read‑only C string
    std::cout << "c_str(): " << cstr << std::endl;
    char* data = &s2[0];                   // writable buffer (C++11 and later)
    data[0] = 'h';
    print("after modifying via data()", s2);

    // ------------------------------------------------------------------
    // 15. Numeric conversions (std::string <-> numbers)
    // ------------------------------------------------------------------
    std::cout << "\n--- Numeric conversions ---\n";
    std::string numStr = "12345";
    int num = std::stoi(numStr);
    std::cout << "stoi(\"" << numStr << "\") = " << num << std::endl;
    long long ll = std::stoll("9876543210");
    std::cout << "stoll(\"9876543210\") = " << ll << std::endl;
    double d = std::stod("3.14159");
    std::cout << "stod(\"3.14159\") = " << d << std::endl;

    // Convert number to string
    std::string fromInt = std::to_string(42);
    print("to_string(42)", fromInt);
    std::string fromDouble = std::to_string(3.14159);
    print("to_string(3.14159)", fromDouble);

    // ------------------------------------------------------------------
    // 16. Swap strings
    // ------------------------------------------------------------------
    std::cout << "\n--- Swap ---\n";
    std::string sA = "first", sB = "second";
    sA.swap(sB);
    print("sA after swap", sA);
    print("sB after swap", sB);

    return 0;
}
