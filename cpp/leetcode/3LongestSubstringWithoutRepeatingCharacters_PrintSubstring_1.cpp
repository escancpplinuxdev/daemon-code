#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

int lengthOfLongestSubstring(string &s, string &longestSubstr)
{
    unordered_map<char, int> lastIndex;
    int left = 0;
    int maxLen = 0;
    int bestStart = 0;

    for (int right = 0; right < s.length(); ++right)
    {
        char c = s[right];

        // Debug: show current step
        cout << "left = '" << left << "'\n";
        cout << "right = '" << right << "'\n";
        cout << "c = '" << c << "'\n";

        // Use find to check existence without inserting
        auto it = lastIndex.find(c);
        bool found = (it != lastIndex.end());
        int lastPos = found ? it->second : -1;

        // Debug: print existence info (no insertion)
        if (found)
        {
            cout << "lastIndex['" << c << "'] = " << lastPos << "\n";
        }
        else
        {
            cout << "lastIndex['" << c << "'] not present\n";
        }

        // Condition: found && lastPos >= left
        bool condition = found && (lastPos >= left);
        cout << "lastIndex.find(c) != lastIndex.end() ? " << (found ? "true" : "false") << "\n";
        cout << "lastIndex[c] = " << (found ? to_string(lastPos) : "not present") << "\n";
        cout << "(lastIndex.find(c) != lastIndex.end() && lastIndex[c]>=left) = "
             << (found ? (lastPos >= left ? "1 && 1" : "1 && 0") : "0 && ?") << " = "
             << (condition ? "1" : "0") << "\n";

        int oldLeft = left;
        if (condition)
        {
            left = lastPos + 1;
            cout << "before left = '" << oldLeft << "'\n";
            cout << "left = lastIndex[c] + 1 = after left = '" << left << "'\n";
        }
        else
        {
            cout << "left unchanged = '" << left << "'\n";
        }

        // Update map (now we insert/overwrite)
        lastIndex[c] = right;
        cout << "lastIndex[c] = right -> lastIndex['" << c << "'] = '" << right << "'\n";

        // Print full map
        cout << "Current lastIndex = { ";
        for (auto &p : lastIndex)
        {
            cout << "'" << p.first << "':" << p.second << " ";
        }
        cout << "}\n";

        int curLen = right - left + 1;
        cout << "curLen = right - left +1 = '" << curLen << "'\n";
        cout << "maxLen = '" << maxLen << "'\n";

        if (curLen > maxLen)
        {
            maxLen = curLen;
            bestStart = left;
            cout << "curLen > maxLen -> maxLen = curLen = '" << maxLen << "'\n";
            cout << "bestStart = left = '" << bestStart << "'\n";
        }
        else
        {
            cout << "curLen <= maxLen -> no change\n";
        }

        cout << "\n";
    }

    longestSubstr = s.substr(bestStart, maxLen);
    return maxLen;
}

int main()
{
    string s = "abcabcabcdbb";
    string longest;
    int len = lengthOfLongestSubstring(s, longest);
    cout << "Input: " << s << "\n";
    cout << "Longest substring: \"" << longest << "\"\n";
    cout << "Length: " << len << "\n";
    return 0;
}
