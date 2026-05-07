#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cctype>

// List of browser names to detect (all lowercase)
const std::vector<std::string> browsers = {"chrome", "brave", "chromium", "edge"};

// Function that returns true if filename contains any browser name (case‑insensitive)
bool isBrowserFile(std::string filename)
{
    // Convert filename to lowercase
    std::string lowerFilename = filename;
    std::transform(lowerFilename.begin(), lowerFilename.end(),
                   lowerFilename.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    // Check if any browser name is a substring of the lowercase filename
    for (const auto& browser : browsers)
    {
        if (lowerFilename.find(browser) != std::string::npos)
        {
            return true;
        }
    }
    return false;
}

int main()
{
    // List of file names that will be tested (some are .desktop, some not)
    std::vector<std::string> fileList =
    {
        "google-chrome.desktop",
        "brave-browser.desktop",
        "chromium.desktop",
        "microsoft-edge.desktop",
        "firefox.desktop",
        "custom.sh",
        "short.desktop",            // length 13, but ends with .desktop
        "a.desktop",                // length 8? Actually "a.desktop" length = 9? Let's check: 'a' + '.' + 'desktop' = 1+1+7 =9. But condition name.size() < 8 will skip? Wait: size 9 is not <8 so it passes.
        "abc.conf",
        "CHROME_setup.desktop",
        "BraveUpdate.desktop",
        "edgewebview2.desktop",
        "nomatch.txt",
	 "google-chrome.desktop",
        "brave-browser.desktop",
        "chromium.desktop",
        "microsoft-edge.desktop",
        "firefox.desktop",
        "CHROME_Setup.exe",
        "BraveUpdateTask.json",
        "custom_script.sh",
        "chromium-snap.desktop",
        "edgewebview2.dll"
    };

    std::cout << "Processing only .desktop files with name length >= 8:\n";
    std::cout << "=====================================================\n";

    for (const auto& name : fileList)
    {
        // Filter condition: skip files that are shorter than 8 chars or do not end with ".desktop"
        if (name.size() < 8 || name.substr(name.size() - 8) != ".desktop")
        {

        std::cout << "File: \"" << name << "\" →  (Skip File)\n";

            continue;
        }

        bool isBrowser = isBrowserFile(name);
        std::cout << "File: \"" << name << "\" → "
                  << (isBrowser ? "YES (browser related)" : "NO")
                  << std::endl;
    }

    return 0;
}
