#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

/**
 * Checks if the given domain is an Amazon domain.
 * Returns true if the domain contains "amazon" (case‑insensitive).
 */
bool isDomainMatch(const std::string& domain)
{
    // Convert to lowercase for case‑insensitive comparison
    std::string lower = domain;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower.find("amazon") != std::string::npos;
}




int main()
{
    std::vector<std::string> testDomains = {
        "images-na.ssl-images-amazon.com",
        "amazon.com",
        "fls-eu.amazon.in",
        "m.media-amazon.com",
        "images-eu.ssl-images-amazon.com",
        "metrics.media-amazon.com",
        "aax-eu.amazon-adsystem.com",
        "c.amazon-adsystem.com",
        "fls-na.amazon.com",
        "unagi-na.amazon.com",
        "unagi.amazon.in",
        "data.amazon.in",
        "unagi-eu.amazon.com"
    };

    for (const auto& d : testDomains) {
        std::cout << d << " -> " << (isDomainMatch(d) ? "Amazon" : "Not Amazon") << '\n';
    }

    // Negative test
    std::cout << "google.com -> " << (isDomainMatch("google.com") ? "Amazon" : "Not Amazon") << '\n';
    return 0;
}
