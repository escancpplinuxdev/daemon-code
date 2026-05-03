// screenshot_prevention_apparmor.cpp - BLOCK EXECUTION BEFORE IT STARTS
// Strategy: Use AppArmor/permissions to prevent binaries from executing AT ALL
// Compile: g++ -o screenshot_prevention screenshot_prevention_apparmor.cpp $(pkg-config --cflags --libs x11 xtst) -lpthread -std=c++17

#include <thread>
#include <atomic>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <sys/inotify.h>
#include <pwd.h>
#include <map>
#include <mutex>
#include <algorithm>
#include <fcntl.h>
#include <errno.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XTest.h>
#include <nlohmann/json.hpp> 
#include "main.h"

using json = nlohmann::json;

bool RemoveFirefoxScreenshotPolicy() {
	const char* policyFile = "/etc/firefox/policies/policies.json";

	// Check if file exists
	std::ifstream inFile(policyFile);
	if (!inFile.good()) {
		ScreenCaptureLog(2, (char*)"Policies file doesn't exist. Nothing to remove.\n");
		return true;  // Not an error, just nothing to do
	}

	// Check if file is empty
	inFile.seekg(0, std::ios::end);
	size_t fileSize = inFile.tellg();
	inFile.seekg(0, std::ios::beg);

	if (fileSize == 0) {
		ScreenCaptureLog(2, (char*)"Policies file is empty. Nothing to remove.\n");
		inFile.close();
		return true;
	}

	// Parse existing policies
	json policies;
	try {
		inFile >> policies;
		inFile.close();
	} catch (const std::exception& e) {
		char msg[256];
		snprintf(msg, 256, "Error parsing policies.json: %s\n", e.what());
		ScreenCaptureLog(1, msg);
		inFile.close();
		return false;
	}

	// Check if policies key exists
	if (policies.find("policies") == policies.end()) {
		ScreenCaptureLog(2, (char*)"No 'policies' key found. Nothing to remove.\n");
		return true;
	}

	// Check if DisableFirefoxScreenshots exists
	if (policies["policies"].find("DisableFirefoxScreenshots") == policies["policies"].end()) {
		ScreenCaptureLog(2, (char*)"DisableFirefoxScreenshots not found. Nothing to remove.\n");
		return true;
	}

	// Remove the specific entry
	policies["policies"].erase("DisableFirefoxScreenshots");
	ScreenCaptureLog(2, (char*)"Removed DisableFirefoxScreenshots entry\n");

	// Write back to file
	std::ofstream outFile(policyFile);
	if (!outFile.is_open()) {
		ScreenCaptureLog(1, (char*)"Failed to open policies.json for writing\n");
		return false;
	}

	outFile << policies.dump(2);
	outFile.close();

	ScreenCaptureLog(1, (char*)"Policy file updated successfully\n");
	return true;
}

bool SetFirefoxScreenshotPolicy(bool disable) {
	const char* policyDir = "/etc/firefox/policies";
	const char* policyFile = "/etc/firefox/policies/policies.json";

	// Create directory if it doesn't exist
	struct stat st;
	if (stat(policyDir, &st) != 0) {
		ScreenCaptureLog(2, (char*)"Policy directory doesn't exist. Creating...\n");
		if (mkdir(policyDir, 0755) != 0) {
			ScreenCaptureLog(1, (char*)"Failed to create policy directory\n");
			return false;
		}
	}

	json policies;
	bool fileExists = false;

	// Check if policies.json already exists and is valid
	std::ifstream inFile(policyFile);
	if (inFile.good()) {
		ScreenCaptureLog(3, (char*)"Policies file exists. Reading...\n");

		// Check if file is empty
		inFile.seekg(0, std::ios::end);
		size_t fileSize = inFile.tellg();
		inFile.seekg(0, std::ios::beg);

		if (fileSize > 0) {
			try {
				inFile >> policies;
				fileExists = true;
				ScreenCaptureLog(3, (char*)"Successfully parsed existing policies\n");
			} catch (const std::exception& e) {
				ScreenCaptureLog(2, (char*)"Existing file is corrupted/invalid. Creating fresh structure...\n");
				fileExists = false;
			}
		} else {
			ScreenCaptureLog(2, (char*)"File is empty. Creating fresh structure...\n");
			fileExists = false;
		}
		inFile.close();
	} else {
		ScreenCaptureLog(2, (char*)"Policies file not found. Creating new structure...\n");
	}

	// Create structure if file doesn't exist or was invalid
	if (!fileExists) {
		policies = {{"policies", json::object()}};
	}

	// Ensure "policies" key exists
	if (policies.find("policies") == policies.end()) {
		policies["policies"] = json::object();
	}

	// Add/Update DisableFirefoxScreenshots
	policies["policies"]["DisableFirefoxScreenshots"] = disable;

	// Write back to file
	std::ofstream outFile(policyFile);
	if (!outFile.is_open()) {
		ScreenCaptureLog(1, (char*)"Failed to open policies.json for writing\n");
		return false;
	}

	outFile << policies.dump(2);  // Pretty print with 2 space indent
	outFile.close();

	char msg[128];
	snprintf(msg, 128, "DisableFirefoxScreenshots set to %s\n", disable ? "true" : "false");
	ScreenCaptureLog(1, msg);
	return true;
}


std::atomic<bool> g_running{true};
std::atomic<bool> g_stopping{false};

static void log_msg(const std::string &s) {
	char buf[512];
	snprintf(buf, 512, "%s\n", s.c_str());
	ScreenCaptureLog(2, buf);
}

class ScreenshotPrevention {
	public:
		ScreenshotPrevention() : 
			display(nullptr), 
			rootWindow(0),
			inotifyFd(-1) {

				log_msg("=== SCREENSHOT PREVENTION - EXECUTION BLOCKING ===");
				log_msg("Strategy: Remove execute permissions from screenshot binaries");
				log_msg("");
			}

		~ScreenshotPrevention() {
			cleanup();
		}

		bool init() {
			log_msg("=== INITIALIZATION ===");

			// Layer 1: REMOVE EXECUTE PERMISSIONS - Most effective!
			blockBinaryExecution();
			log_msg("[✓] Binary execution blocked");

			// Layer 2: X11 keyboard
			if (initX11()) {
				log_msg("[✓] Keyboard blocking");
				disableScreenshotKeys();
			} else {
				log_msg("[✗] X11 failed");
			}

			// Layer 3: File monitoring (cleanup)
			if (setupFileMonitoring()) {
				log_msg("[✓] File monitoring");
			} else {
				log_msg("[✗] File monitoring failed");
			}

			// Layer 4: GNOME shortcuts
			disableGnomeShortcuts();
			log_msg("[✓] Shortcuts disabled");

			log_msg("");
			return true;
		}

		void start() {
			log_msg("=== ACTIVE - Press Ctrl+C to stop ===");
			log_msg("");

			// Monitor for permission changes
			std::thread permMonitor(&ScreenshotPrevention::monitorPermissions, this);
			permMonitor.detach();

			// Clipboard monitor
			std::thread clipboardThread(&ScreenshotPrevention::clipboardMonitor, this);
			clipboardThread.detach();

			// Main loop
			while (g_running && !g_stopping) {
				interceptKeyboardEvents();
				monitorFileCreation();
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}

		void stop() {
			if (g_stopping) return;
			g_stopping = true;

			log_msg("");
			log_msg("=== STOPPING ===");
			g_running = false;

			std::this_thread::sleep_for(std::chrono::milliseconds(200));

			restoreBinaryPermissions();
			restoreScreenshotKeys();
			restoreGnomeShortcuts();

			log_msg("Screenshot blocking disabled");
		}

	private:
		Display* display;
		Window rootWindow;
		int inotifyFd;
		std::map<int, std::string> wdToDir;
		std::mutex wdMutex;

		std::vector<std::string> blockedTools = {
			"gnome-screenshot", "scrot", "flameshot", "spectacle", "maim",
			"shutter", "grim", "slurp", "ksnip", "xfce4-screenshooter",
			"mate-screenshot", "deepin-screenshot", "ksnapshot", "import",
			"shotwell", "xwd", "convert"
		};

		std::map<std::string, mode_t> originalPermissions;

		// ===== LAYER 1: BLOCK BINARY EXECUTION =====
		void blockBinaryExecution() {
			log_msg("[BLOCKING] Removing execute permissions from screenshot tools...");

			std::vector<std::string> searchPaths = {
				"/usr/bin/",
				"/bin/",
				"/usr/local/bin/"
			};

			int blockedCount = 0;

			for (const auto& tool : blockedTools) {
				for (const auto& path : searchPaths) {
					std::string fullPath = path + tool;

					struct stat fileStat;
					if (stat(fullPath.c_str(), &fileStat) == 0) {
						// Save original permissions
						originalPermissions[fullPath] = fileStat.st_mode;

						// Remove ALL execute permissions (owner, group, others)
						mode_t newMode = fileStat.st_mode & ~(S_IXUSR | S_IXGRP | S_IXOTH);

						if (chmod(fullPath.c_str(), newMode) == 0) {
							char msg[256];
							snprintf(msg, 256, "  [✓] Blocked: %s\n", fullPath.c_str());
							ScreenCaptureLog(2, msg);
							blockedCount++;
						} else {
							char msg[256];
							snprintf(msg, 256, "  [✗] Failed to block: %s (%s)\n", fullPath.c_str(), strerror(errno));
							ScreenCaptureLog(1, msg);
						}
					}
				}
			}

			char msg[128];
			snprintf(msg, 128, "[BLOCKED] %d screenshot tools blocked\n", blockedCount);
			ScreenCaptureLog(1, msg);
		}

		void restoreBinaryPermissions() {
			log_msg("Restoring binary permissions...");

			for (const auto& entry : originalPermissions) {
				if (chmod(entry.first.c_str(), entry.second) == 0) {
					char msg[256];
					snprintf(msg, 256, "  [✓] Restored: %s\n", entry.first.c_str());
					ScreenCaptureLog(2, msg);
				}
			}
		}

		// Monitor for permission changes (in case user tries to restore them)
		void monitorPermissions() {
			log_msg("[PERMISSIONS] Monitoring for permission changes");

			while (g_running && !g_stopping) {
				for (const auto& entry : originalPermissions) {
					struct stat fileStat;
					if (stat(entry.first.c_str(), &fileStat) == 0) {
						// Check if execute permission was restored
						if (fileStat.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
							// Remove it again
							mode_t newMode = fileStat.st_mode & ~(S_IXUSR | S_IXGRP | S_IXOTH);
							chmod(entry.first.c_str(), newMode);
							char msg[256];
							snprintf(msg, 256, "[BLOCKED] Re-blocked: %s\n", entry.first.c_str());
							ScreenCaptureLog(1, msg);
						}
					}
				}

				// Check every second
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}

		// ===== LAYER 2: X11 KEYBOARD =====
		bool initX11() {
			display = XOpenDisplay(nullptr);
			if (!display) return false;

			rootWindow = DefaultRootWindow(display);

			int fd = ConnectionNumber(display);
			int flags = fcntl(fd, F_GETFL, 0);
			fcntl(fd, F_SETFL, flags | O_NONBLOCK);

			XSelectInput(display, rootWindow, KeyPressMask | KeyReleaseMask);

			int printScreenKeycode = XKeysymToKeycode(display, XK_Print);
			if (printScreenKeycode != 0) {
				XGrabKey(display, printScreenKeycode, AnyModifier, rootWindow, 
						True, GrabModeAsync, GrabModeAsync);
			}

			return true;
		}

		void interceptKeyboardEvents() {
			if (!display) return;

			XEvent event;
			while (XPending(display) > 0) {
				XNextEvent(display, &event);

				if (event.type == KeyPress) {
					KeySym keysym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);
					if (keysym == XK_Print) {
						ScreenCaptureLog(1, (char*)"[BLOCKED] Print Screen key\n");
					}
				}
			}
		}

		void disableScreenshotKeys() {
			system("xmodmap -e 'keycode 107 = ' 2>/dev/null");
		}

		void restoreScreenshotKeys() {
			system("xmodmap -e 'keycode 107 = Print' 2>/dev/null");

			if (display) {
				int keycode = XKeysymToKeycode(display, XK_Print);
				if (keycode) XUngrabKey(display, keycode, AnyModifier, rootWindow);
			}
		}

		// ===== LAYER 3: FILE MONITORING =====
		bool setupFileMonitoring() {
			const char* home = getenv("HOME");
			if (!home) {
				struct passwd* pw = getpwuid(getuid());
				if (pw) home = pw->pw_dir;
			}
			if (!home) return false;

			std::vector<std::string> watchDirs = {
				std::string(home) + "/Pictures",
				std::string(home) + "/Desktop",
				std::string(home) + "/Downloads",
				std::string(home),
				"/tmp"
			};

			inotifyFd = inotify_init1(IN_NONBLOCK | IN_CLOEXEC);
			if (inotifyFd < 0) return false;

			for (const auto &dir : watchDirs) {
				struct stat st;
				if (stat(dir.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
					int wd = inotify_add_watch(inotifyFd, dir.c_str(), 
							IN_CREATE | IN_CLOSE_WRITE | IN_MOVED_TO);
					if (wd >= 0) {
						std::lock_guard<std::mutex> lk(wdMutex);
						wdToDir[wd] = dir;
					}
				}
			}

			return true;
		}

		void monitorFileCreation() {
			if (inotifyFd < 0) return;

			char buffer[4096];
			ssize_t len = read(inotifyFd, buffer, sizeof(buffer));
			if (len <= 0) return;

			size_t offset = 0;
			while (offset < static_cast<size_t>(len)) {
				struct inotify_event* ev = reinterpret_cast<struct inotify_event*>(buffer + offset);
				size_t eventSize = sizeof(struct inotify_event) + ev->len;

				if (ev->len > 0 && !(ev->mask & IN_ISDIR)) {
					std::string filename(ev->name);
					std::string lower = filename;
					std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

					bool isScreenshot = 
						lower.find("screenshot") != std::string::npos ||
						lower.find("scrot") != std::string::npos ||
						lower.find("capture") != std::string::npos ||
						lower.find("shot") != std::string::npos;

					if (isScreenshot) {
						std::string dir;
						{
							std::lock_guard<std::mutex> lk(wdMutex);
							auto it = wdToDir.find(ev->wd);
							if (it != wdToDir.end()) dir = it->second;
						}

						std::string fullPath = dir + "/" + filename;
						char msg[512];
						snprintf(msg, 512, "[DETECTED] %s\n", filename.c_str());
						ScreenCaptureLog(1, msg);

						std::this_thread::sleep_for(std::chrono::milliseconds(50));

						if (unlink(fullPath.c_str()) == 0) {
							snprintf(msg, 512, "[DELETED] %s\n", fullPath.c_str());
							ScreenCaptureLog(1, msg);
						}
					}
				}

				offset += eventSize;
			}
		}

		// ===== LAYER 4: CLIPBOARD =====
		void clipboardMonitor() {
			while (g_running && !g_stopping) {
				FILE* pipe = popen("xclip -selection clipboard -t TARGETS -o 2>/dev/null", "r");
				if (pipe) {
					char buffer[256];
					std::string output;
					while (fgets(buffer, sizeof(buffer), pipe)) {
						output += buffer;
					}
					pclose(pipe);

					if (output.find("image/") != std::string::npos) {
						ScreenCaptureLog(1, (char*)"[BLOCKED] Clipboard screenshot\n");
						system("echo -n '' | xclip -selection clipboard 2>/dev/null");
						system("echo -n '' | xclip -selection primary 2>/dev/null");
					}
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			}
		}

		void disableGnomeShortcuts() {
			system("gsettings set org.gnome.shell.keybindings screenshot '[]' 2>/dev/null");
			system("gsettings set org.gnome.shell.keybindings screenshot-window '[]' 2>/dev/null");
			system("gsettings set org.gnome.settings-daemon.plugins.media-keys screenshot '[]' 2>/dev/null");
		}

		void restoreGnomeShortcuts() {
			system("gsettings reset org.gnome.shell.keybindings screenshot 2>/dev/null");
			system("gsettings reset org.gnome.settings-daemon.plugins.media-keys screenshot 2>/dev/null");
		}

		void cleanup() {
			if (inotifyFd >= 0) close(inotifyFd);
			if (display) XCloseDisplay(display);
		}
};

ScreenshotPrevention *g_monitor = nullptr;

static void signalHandler(int signum) {

	RemoveFirefoxScreenshotPolicy();

	if (g_stopping) {
		exit(1);
	}

	g_running = false;
	if (g_monitor) {
		g_monitor->stop();
	}
}

int startScreenCapturing() {

	if (SetFirefoxScreenshotPolicy(true)) {
		ScreenCaptureLog(1, (char*)"Firefox policy set: Success!\n");
	} else {
		ScreenCaptureLog(1, (char*)"Firefox policy set: Failed!\n");
	}

	if (geteuid() != 0) {
		ScreenCaptureLog(1, (char*)"ERROR: Must run as root\n");
		return 1;
	}

	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);

	ScreenshotPrevention monitor;
	g_monitor = &monitor;

	monitor.init();
	monitor.start();

	log_msg("=== Exited ===");
	return 0;
}
