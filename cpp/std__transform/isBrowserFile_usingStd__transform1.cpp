#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cctype>

// List of browser names to detect
const std::vector<std::string> browsers = {"chrome", "brave", "chromium", "edge"};

// Function that returns true if filename contains any browser name (case‑insensitive)
bool isBrowserFile(const std::string& filename) {
    // Convert filename to lowercase
    std::string lowerFilename = filename;
    std::transform(lowerFilename.begin(), lowerFilename.end(),
                   lowerFilename.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    // Check if any browser name is a substring of the lowercase filename
    for (const auto& browser : browsers) {
        if (lowerFilename.find(browser) != std::string::npos) {
            return true;
        }
    }
    return false;
}

int main() {
    // Built‑in test inputs (array of filenames)
    std::vector<std::string> testFiles = {
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

    std::cout << "Browser file detection test:\n";
    std::cout << "============================\n";
    for (const auto& file : testFiles) {
        bool result = isBrowserFile(file);
        std::cout << "File: \"" << file << "\" → "
                  << (result ? "YES (browser related)" : "NO") << '\n';
    }

    return 0;
}
