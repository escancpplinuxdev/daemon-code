#include <iostream>
#include <string>
#include <unordered_set>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unistd.h>


// Helper to get current timestamp as a string (format: YYYY-MM-DD HH:MM:SS)
std::string getCurrentTimestamp() 
{
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	std::tm* tm_info = std::localtime(&now_time);
	std::stringstream ss;
	ss << std::put_time(tm_info, "%Y-%m-%d %H:%M:%S");
	return ss.str();
}

// Class to handle unique logging per event + timestamp
class UniqueEventLogger 
{
	private:
		std::unordered_set<std::string> seen;  // stores "event|timestamp" keys

	public:
		// Log an event – only if not seen before with the same timestamp
		void logEvent(const std::string& event) 
		{
			std::string timestamp = getCurrentTimestamp();
			std::string key = event + "|" + timestamp;  // unique key

			if (seen.find(key) == seen.end()) 
			{
				// New event – log it and remember it
				seen.insert(key);
				std::cout << "[" << timestamp << "] " << event << std::endl;
			}
			else
			{
				// Duplicate – silently ignore or show a debug message
				//std::cout << "[DUPLICATE] " << event << " at " << timestamp << std::endl;
			}
		}
};

int main() 
{
	UniqueEventLogger logger;

	// Simulate receiving 9 identical "file open" events at the same time
	for (int i = 0; i < 9; ++i) 
	{
		logger.logEvent("file open");
		// Optional: small delay to simulate real-time – but here we want the same timestamp
		// If you want them to have the exact same timestamp, remove any delay.
	}

	// Now try a different event
	logger.logEvent("file close");

	for (int i = 0; i < 29; ++i) 
	{
		if(i<5)		sleep(1);
		if(i<8)		sleep(1);
		logger.logEvent("file open");
		// Optional: small delay to simulate real-time – but here we want the same timestamp
		// If you want them to have the exact same timestamp, remove any delay.
	}
	logger.logEvent("file close");
	logger.logEvent("file close");
	logger.logEvent("file close");
	logger.logEvent("file close");

	return 0;
}
