#include <iostream>
#include <map>
#include <string>
#include <utility>   // for std::pair

using namespace std;

int main() 
{
    // 1. Declare the map (key = pair<string,string>, value = string)
    map<pair<string, string>, string> myMap;

    // 2. Declare an iterator (as in your question)
    map<pair<string, string>, string>::iterator itPkgList;

    cout << "=== Insertion Operations ===" << endl;

    // 2a. Insert using operator[]
    myMap[{"Alice", "Bob"}] = "Friends";
    myMap[{"Charlie", "Dave"}] = "Colleagues";

    // 2b. Insert using insert() with make_pair
    myMap.insert(make_pair(make_pair("Eve", "Frank"), "Partners"));

    // 2c. Insert using emplace (C++11, constructs in-place)
    myMap.emplace(pair<string,string>("Grace", "Heidi"), "Sisters");

    // 2d. Insert using insert with initializer list
    myMap.insert({{"Ivan", "Judy"}, "Neighbors"});

    cout << "Current map size: " << myMap.size() << endl;

    // 3. Display all entries (iteration)
    cout << "\n=== Iteration (using iterator) ===" << endl;
    for (itPkgList = myMap.begin(); itPkgList != myMap.end(); ++itPkgList) 
    {
        const auto& key = itPkgList->first;
        const auto& value = itPkgList->second;
        cout << "Key: (" << key.first << ", " << key.second << ") -> Value: " << value << endl;
    }

    // 3b. Range-based for loop (C++11)
    cout << "\n=== Iteration (range-based) ===" << endl;
    for (const auto& entry : myMap) 
    {
        cout << "(" << entry.first.first << ", " << entry.first.second << ") => " << entry.second << endl;
    }

    // 4. Access values
    cout << "\n=== Access Operations ===" << endl;
    // Using operator[] (inserts default if missing)
    cout << "myMap[{Alice, Bob}] = " << myMap[{"Alice", "Bob"}] << endl;
    // Using at() (throws if missing)
    try
    {
        cout << "myMap.at({Charlie, Dave}) = " << myMap.at({"Charlie", "Dave"}) << endl;
    }
    catch (const out_of_range& e)
    {
        cout << "Key not found" << endl;
    }

    // 5. Lookup (find)
    cout << "\n=== Lookup (find) ===" << endl;
    pair<string,string> searchKey = {"Eve", "Frank"};
    itPkgList = myMap.find(searchKey);
    if (itPkgList != myMap.end()) 
    {
        cout << "Found: " << itPkgList->second << endl;
    } else
    {
        cout << "Key not found" << endl;
    }

    // 6. Count (returns 0 or 1 for map)
    cout << "\n=== Count ===" << endl;
    cout << "Count of {Alice, Bob}: " << myMap.count({"Alice", "Bob"}) << endl;
    cout << "Count of {Zoe, Zoe}: " << myMap.count({"Zoe", "Zoe"}) << endl;

    // 7. Size and empty
    cout << "\n=== Size & Empty ===" << endl;
    cout << "Size: " << myMap.size() << endl;
    cout << "Is empty? " << (myMap.empty() ? "Yes" : "No") << endl;

    // 8. Erasing elements
    cout << "\n=== Erasing ===" << endl;

    // 8a. Erase by key
    size_t erased = myMap.erase({"Grace", "Heidi"});
    cout << "Erased {Grace, Heidi}: " << erased << " element(s)" << endl;

    // 8b. Erase by iterator (found via find)
    itPkgList = myMap.find({"Ivan", "Judy"});
    if (itPkgList != myMap.end()) 
    {
        myMap.erase(itPkgList);
        cout << "Erased {Ivan, Judy} using iterator" << endl;
    }

    // 8c. Clear all
    // myMap.clear(); // Uncomment to clear all

    cout << "After erasures, size: " << myMap.size() << endl;

    // 9. Bounds operations (lower_bound, upper_bound, equal_range)
    cout << "\n=== Bounds Operations ===" << endl;
    // Insert some keys for demonstration
    myMap[{"AAA", "AAA"}] = "First";
    myMap[{"BBB", "BBB"}] = "Second";
    myMap[{"CCC", "CCC"}] = "Third";

    // lower_bound: first key >= given
    auto itLow = myMap.lower_bound({"BBB", "BBB"});
    if (itLow != myMap.end()) 
    {
        cout << "lower_bound({BBB, BBB}): (" << itLow->first.first << ", " << itLow->first.second << ") => " << itLow->second << endl;
    }

    // upper_bound: first key > given
    auto itUp = myMap.upper_bound({"BBB", "BBB"});
    if (itUp != myMap.end()) 
    {
        cout << "upper_bound({BBB, BBB}): (" << itUp->first.first << ", " << itUp->first.second << ") => " << itUp->second << endl;
    }

    // equal_range: returns [lower, upper)
    auto range = myMap.equal_range({"BBB", "BBB"});
    cout << "equal_range({BBB, BBB}) contains:" << endl;
    for (auto it = range.first; it != range.second; ++it) 
    {
        cout << "  (" << it->first.first << ", " << it->first.second << ") => " << it->second << endl;
    }

    // 10. Modify a value (key unchanged)
    cout << "\n=== Modifying Value ===" << endl;
    myMap[{"Alice", "Bob"}] = "Best Friends";
    cout << "New value for {Alice, Bob}: " << myMap[{"Alice", "Bob"}] << endl;

    // Final map contents
    cout << "\n=== Final Map ===" << endl;
    for (const auto& e : myMap) 
    {
        cout << "(" << e.first.first << ", " << e.first.second << ") -> " << e.second << endl;
    }

    return 0;
}
