// screenshot_monitor.cpp – standalone screenshot blocker (Hardcoded user = rushi)
// Compile: g++ -std=c++17 -o screenshot_monitor screenshot_monitor.cpp -lX11 -lXtst -lpthread -I/usr/include/nlohmann
// Run: sudo ./screenshot_monitor

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <atomic>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <algorithm>
#include <cstdlib>
#include <cstdio>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XTest.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

// ---------- Logging ----------
static std::mutex g_logMutex;

void ScreenCaptureLog(int level, const char* msg) {
    std::lock_guard<std::mutex> lock(g_logMutex);
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    char timeBuf[32];
    std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&now_time));
    std::cout << "[" << timeBuf << "] [LEVEL " << level << "] " << msg;
    std::cout.flush();
}

// ---------- Detect session type ----------
static bool is_wayland_session() {
    const char* session = getenv("XDG_SESSION_TYPE");
    if (session && strcmp(session, "wayland") == 0) return true;
    if (getenv("WAYLAND_DISPLAY") != nullptr) return true;
    return false;
}

// ---------- Get the actual logged‑in user (hardcoded to rushi) ----------
static std::string getSessionUser() {
    // Hardcode to rushi for this test – we know the user is rushi
    return "rushi";
}

// ---------- Get DBus address for the user ----------
static std::string getUserDBusAddress(const std::string& user) {
    // On systemd-based systems, the user bus is at /run/user/<UID>/bus
    struct passwd* pw = getpwnam(user.c_str());
    if (!pw) return "";
    uid_t uid = pw->pw_uid;
    std::string busPath = "/run/user/" + std::to_string(uid) + "/bus";
    if (access(busPath.c_str(), F_OK) == 0) {
        return "unix:path=" + busPath;
    }
    // fallback: try the old socket path
    std::string socketPath = "/run/user/" + std::to_string(uid) + "/dbus-session";
    if (access(socketPath.c_str(), F_OK) == 0) {
        return "unix:path=" + socketPath;
    }
    return "";
}

static void runAsUser(const std::string& cmd, bool wait = true) {
    std::string user = getSessionUser();
    if (user.empty()) {
        system(cmd.c_str());
        return;
    }
    std::string dbus = getUserDBusAddress(user);
    std::string fullCmd = "sudo -u " + user + " ";
    if (!dbus.empty()) {
        fullCmd += "env DBUS_SESSION_BUS_ADDRESS=\"" + dbus + "\" ";
    }
    // Also set DISPLAY=:0 for X11 apps (though not needed for gsettings)
    fullCmd += cmd;
    if (wait) {
        system(fullCmd.c_str());
    } else {
        system((fullCmd + " &").c_str());
    }
}

// ---------- Firefox Policies ----------
bool RemoveFirefoxScreenshotPolicy() {
    const char* policyFile = "/etc/firefox/policies/policies.json";
    std::ifstream inFile(policyFile);
    if (!inFile.good()) {
        ScreenCaptureLog(2, "Policies file doesn't exist. Nothing to remove.\n");
        return true;
    }
    inFile.seekg(0, std::ios::end);
    size_t fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    if (fileSize == 0) {
        ScreenCaptureLog(2, "Policies file is empty. Nothing to remove.\n");
        inFile.close();
        return true;
    }
    json policies;
    try {
        inFile >> policies;
        inFile.close();
    } catch (const std::exception& e) {
        ScreenCaptureLog(1, (std::string("Error parsing policies.json: ") + e.what() + "\n").c_str());
        inFile.close();
        return false;
    }
    if (policies.find("policies") == policies.end() ||
        policies["policies"].find("DisableFirefoxScreenshots") == policies["policies"].end()) {
        ScreenCaptureLog(2, "DisableFirefoxScreenshots not found. Nothing to remove.\n");
        return true;
    }
    policies["policies"].erase("DisableFirefoxScreenshots");
    ScreenCaptureLog(2, "Removed DisableFirefoxScreenshots entry\n");
    std::ofstream outFile(policyFile);
    if (!outFile.is_open()) {
        ScreenCaptureLog(1, "Failed to open policies.json for writing\n");
        return false;
    }
    outFile << policies.dump(2);
    outFile.close();
    ScreenCaptureLog(1, "Policy file updated successfully\n");
    return true;
}

bool SetFirefoxScreenshotPolicy(bool disable) {
    const char* policyDir = "/etc/firefox/policies";
    const char* policyFile = "/etc/firefox/policies/policies.json";
    struct stat st;
    if (stat(policyDir, &st) != 0) {
        ScreenCaptureLog(2, "Policy directory doesn't exist. Creating...\n");
        if (mkdir(policyDir, 0755) != 0) {
            ScreenCaptureLog(1, "Failed to create policy directory\n");
            return false;
        }
    }
    json policies;
    bool fileExists = false;
    std::ifstream inFile(policyFile);
    if (inFile.good()) {
        inFile.seekg(0, std::ios::end);
        size_t fileSize = inFile.tellg();
        inFile.seekg(0, std::ios::beg);
        if (fileSize > 0) {
            try {
                inFile >> policies;
                fileExists = true;
            } catch (const std::exception& e) {
                ScreenCaptureLog(2, "Existing file is corrupted/invalid. Creating fresh structure...\n");
                fileExists = false;
            }
        } else {
            ScreenCaptureLog(2, "File is empty. Creating fresh structure...\n");
        }
        inFile.close();
    }
    if (!fileExists) {
        policies = {{"policies", json::object()}};
    }
    if (policies.find("policies") == policies.end()) {
        policies["policies"] = json::object();
    }
    policies["policies"]["DisableFirefoxScreenshots"] = disable;
    std::ofstream outFile(policyFile);
    if (!outFile.is_open()) {
        ScreenCaptureLog(1, "Failed to open policies.json for writing\n");
        return false;
    }
    outFile << policies.dump(2);
    outFile.close();
    char msg[128];
    snprintf(msg, 128, "DisableFirefoxScreenshots set to %s\n", disable ? "true" : "false");
    ScreenCaptureLog(1, msg);
    return true;
}

// ---------- Static monitor data ----------
static Display* g_display = nullptr;
static Window g_rootWindow = 0;
static int g_inotifyFd = -1;
static std::map<int, std::string> g_wdToDir;
static std::mutex g_wdMutex;

static std::vector<std::string> g_blockedTools = {
    "gnome-screenshot", "scrot", "flameshot", "spectacle", "maim",
    "shutter", "grim", "slurp", "ksnip", "xfce4-screenshooter",
    "mate-screenshot", "deepin-screenshot", "ksnapshot", "import",
    "shotwell", "xwd", "convert"
};
static std::map<std::string, mode_t> g_originalPermissions;

static std::thread g_permThread;
static std::thread g_clipboardThread;
static std::thread g_fileThread;

static std::atomic<bool> g_running{false};
static std::atomic<bool> g_stopping{false};
static std::atomic<bool> monitor_started{false};
static std::atomic<bool> monitor_stopping{false};

// ---------- Binary execution blocking ----------
static void blockBinaryExecution() {
    ScreenCaptureLog(2, "[BLOCKING] Removing execute permissions from screenshot tools...\n");
    std::vector<std::string> searchPaths = {"/usr/bin/", "/bin/", "/usr/local/bin/"};
    int blockedCount = 0;
    for (const auto& tool : g_blockedTools) {
        for (const auto& path : searchPaths) {
            std::string fullPath = path + tool;
            struct stat fileStat;
            if (stat(fullPath.c_str(), &fileStat) == 0) {
                g_originalPermissions[fullPath] = fileStat.st_mode;
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

static void restoreBinaryPermissions() {
    ScreenCaptureLog(2, "Restoring binary permissions...\n");
    for (const auto& entry : g_originalPermissions) {
        if (chmod(entry.first.c_str(), entry.second) == 0) {
            char msg[256];
            snprintf(msg, 256, "  [✓] Restored: %s\n", entry.first.c_str());
            ScreenCaptureLog(2, msg);
        }
    }
}

static void monitorPermissionsLoop() {
    ScreenCaptureLog(2, "[PERMISSIONS] Monitoring for permission changes\n");
    while (g_running && !g_stopping) {
        for (const auto& entry : g_originalPermissions) {
            struct stat fileStat;
            if (stat(entry.first.c_str(), &fileStat) == 0) {
                if (fileStat.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
                    mode_t newMode = fileStat.st_mode & ~(S_IXUSR | S_IXGRP | S_IXOTH);
                    chmod(entry.first.c_str(), newMode);
                    char msg[256];
                    snprintf(msg, 256, "[BLOCKED] Re-blocked: %s\n", entry.first.c_str());
                    ScreenCaptureLog(1, msg);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

// ---------- X11 keyboard (only if X11 session) ----------
static bool initX11() {
    if (is_wayland_session()) {
        ScreenCaptureLog(2, "[WAYLAND] Skipping X11 key grab.\n");
        g_display = nullptr;
        return true;
    }
    g_display = XOpenDisplay(nullptr);
    if (!g_display) {
        ScreenCaptureLog(2, "[X11] Cannot open display.\n");
        return false;
    }
    g_rootWindow = DefaultRootWindow(g_display);
    int fd = ConnectionNumber(g_display);
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    XSelectInput(g_display, g_rootWindow, KeyPressMask | KeyReleaseMask);
    int printKeycode = XKeysymToKeycode(g_display, XK_Print);
    if (printKeycode != 0) {
        XGrabKey(g_display, printKeycode, AnyModifier, g_rootWindow,
                True, GrabModeAsync, GrabModeAsync);
    }
    return true;
}

static void interceptKeyboardEvents() {
    if (!g_display) return;
    XEvent event;
    while (XPending(g_display) > 0) {
        XNextEvent(g_display, &event);
        if (event.type == KeyPress) {
            KeySym keysym = XkbKeycodeToKeysym(g_display, event.xkey.keycode, 0, 0);
            if (keysym == XK_Print) {
                ScreenCaptureLog(1, "[BLOCKED] Print Screen key (X11)\n");
            }
        }
    }
}

// ---- Print Screen / SysRq blocking (X11 only) ----
static const int printKeycodes[] = {107, 111, 112, 113, 117};
static const int numPrintKeycodes = sizeof(printKeycodes)/sizeof(printKeycodes[0]);

static void disableScreenshotKeysX11() {
    for (int i = 0; i < numPrintKeycodes; ++i) {
        char cmd[64];
        snprintf(cmd, sizeof(cmd), "xmodmap -e 'keycode %d = NoSymbol' 2>/dev/null", printKeycodes[i]);
        system(cmd);
    }
    system("setxkbmap -print | xkbcomp - -xkb | "
           "sed -e 's/.*key.*PRINT.*{.*};.*//g' | "
           "xkbcomp - -xkb $DISPLAY 2>/dev/null");
    system("echo 0 > /proc/sys/kernel/sysrq 2>/dev/null");
    system("sysctl -w kernel.sysrq=0 2>/dev/null");
    ScreenCaptureLog(2, "PrintScreen key & SysRq disabled (X11).\n");
}

static void restoreScreenshotKeysX11() {
    for (int i = 0; i < numPrintKeycodes; ++i) {
        char cmd[64];
        snprintf(cmd, sizeof(cmd), "xmodmap -e 'keycode %d = Print' 2>/dev/null", printKeycodes[i]);
        system(cmd);
    }
    system("setxkbmap -option '' 2>/dev/null");
    system("echo 1 > /proc/sys/kernel/sysrq 2>/dev/null");
    system("sysctl -w kernel.sysrq=1 2>/dev/null");
    ScreenCaptureLog(2, "PrintScreen key & SysRq restored.\n");
}

// ---------- File monitoring ----------
static bool setupFileMonitoring() {
    const char* home = getenv("HOME");
    if (!home) {
        struct passwd* pw = getpwuid(getuid());
        if (pw) home = pw->pw_dir;
    }
    if (!home) {
        ScreenCaptureLog(1, "Cannot determine home directory. File monitoring disabled.\n");
        return false;
    }
    std::vector<std::string> watchDirs = {
        std::string(home) + "/Pictures",
        std::string(home) + "/Desktop",
        std::string(home) + "/Downloads",
        std::string(home),
        "/tmp"
    };
    g_inotifyFd = inotify_init1(IN_NONBLOCK | IN_CLOEXEC);
    if (g_inotifyFd < 0) return false;
    for (const auto& dir : watchDirs) {
        struct stat st;
        if (stat(dir.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
            int wd = inotify_add_watch(g_inotifyFd, dir.c_str(),
                    IN_CREATE | IN_CLOSE_WRITE | IN_MOVED_TO);
            if (wd >= 0) {
                std::lock_guard<std::mutex> lk(g_wdMutex);
                g_wdToDir[wd] = dir;
            }
        }
    }
    return true;
}

static void fileMonitorLoop() {
    if (g_inotifyFd < 0) return;
    char buffer[4096];
    while (g_running && !g_stopping) {
        ssize_t len = read(g_inotifyFd, buffer, sizeof(buffer));
        if (len <= 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }
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
                        std::lock_guard<std::mutex> lk(g_wdMutex);
                        auto it = g_wdToDir.find(ev->wd);
                        if (it != g_wdToDir.end()) dir = it->second;
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
}

// ---------- Clipboard ----------
static void clipboardMonitorLoop() {
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
                ScreenCaptureLog(1, "[BLOCKED] Clipboard screenshot\n");
                system("echo -n '' | xclip -selection clipboard 2>/dev/null");
                system("echo -n '' | xclip -selection primary 2>/dev/null");
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

// ---------- GNOME / Desktop shortcuts (run as user) ----------
static void disableGnomeShortcuts() {
    std::string user = getSessionUser();
    if (user.empty()) {
        ScreenCaptureLog(2, "Could not determine user; skipping desktop shortcuts.\n");
        return;
    }
    std::vector<std::string> cmds = {
        "gsettings set org.gnome.shell.keybindings screenshot '[]'",
        "gsettings set org.gnome.shell.keybindings screenshot-window '[]'",
        "gsettings set org.gnome.settings-daemon.plugins.media-keys screenshot '[]'",
        "dconf write /org/gnome/shell/keybindings/screenshot \"@as []\"",
        // Also try to disable the "Take Screenshot" action in the shell
        "gsettings set org.gnome.shell.keybindings show-screenshot-ui '[]'",
        // KDE
        "kwriteconfig5 --file ~/.config/kglobalshortcutsrc --group 'org.kde.kwin' --key 'Screenshot' 'none' 2>/dev/null",
        "kwriteconfig5 --file ~/.config/kglobalshortcutsrc --group 'org.kde.kwin' --key 'Screenshot Window' 'none' 2>/dev/null"
    };
    for (const auto& cmd : cmds) {
        runAsUser(cmd, true);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    ScreenCaptureLog(2, ("Desktop shortcuts disabled for user " + user + "\n").c_str());
}

static void restoreGnomeShortcuts() {
    std::string user = getSessionUser();
    if (user.empty()) return;
    std::vector<std::string> cmds = {
        "gsettings reset org.gnome.shell.keybindings screenshot",
        "gsettings reset org.gnome.shell.keybindings screenshot-window",
        "gsettings reset org.gnome.settings-daemon.plugins.media-keys screenshot",
        "dconf reset /org/gnome/shell/keybindings/screenshot",
        "gsettings reset org.gnome.shell.keybindings show-screenshot-ui",
        "kwriteconfig5 --file ~/.config/kglobalshortcutsrc --group 'org.kde.kwin' --key 'Screenshot' 'Meta+Print' 2>/dev/null",
        "kwriteconfig5 --file ~/.config/kglobalshortcutsrc --group 'org.kde.kwin' --key 'Screenshot Window' 'Alt+Print' 2>/dev/null"
    };
    for (const auto& cmd : cmds) {
        runAsUser(cmd, true);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    ScreenCaptureLog(2, ("Desktop shortcuts restored for user " + user + "\n").c_str());
}

// ---------- Cleanup ----------
static void cleanupX11() {
    if (g_display) XCloseDisplay(g_display);
    g_display = nullptr;
}

static void cleanupInotify() {
    if (g_inotifyFd >= 0) close(g_inotifyFd);
    g_inotifyFd = -1;
}

// ---------- Public API ----------
bool init_screenshot_monitor() {
    ScreenCaptureLog(2, "=== SCREENSHOT PREVENTION INIT ===\n");
    if (is_wayland_session())
        ScreenCaptureLog(2, "Detected Wayland session.\n");
    else
        ScreenCaptureLog(2, "Detected X11 session.\n");

    blockBinaryExecution();

    if (initX11()) {
        if (!is_wayland_session()) {
            disableScreenshotKeysX11();
            ScreenCaptureLog(2, "[✓] X11 key grab & remap done.\n");
        }
    } else {
        ScreenCaptureLog(2, "[✗] X11 init failed (keyboard may not be blocked)\n");
    }

    if (!setupFileMonitoring()) {
        ScreenCaptureLog(2, "[✗] File monitoring failed\n");
    } else {
        ScreenCaptureLog(2, "[✓] File monitoring ready\n");
    }

    disableGnomeShortcuts();
    ScreenCaptureLog(2, "[✓] Desktop shortcuts disabled (user session)\n");

    return true;
}

void start_screenshot_monitor() {
    if (monitor_started) {
        ScreenCaptureLog(2, "Monitor already started\n");
        return;
    }

    g_running = true;
    g_stopping = false;
    monitor_started = true;
    monitor_stopping = false;

    ScreenCaptureLog(2, "=== SCREENSHOT MONITOR ACTIVE ===\n");

    g_permThread = std::thread(monitorPermissionsLoop);
    g_clipboardThread = std::thread(clipboardMonitorLoop);
    g_fileThread = std::thread(fileMonitorLoop);

    while (g_running && !g_stopping) {
        interceptKeyboardEvents();   // no‑op on Wayland
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    if (g_permThread.joinable()) g_permThread.join();
    if (g_clipboardThread.joinable()) g_clipboardThread.join();
    if (g_fileThread.joinable()) g_fileThread.join();

    restoreBinaryPermissions();
    if (!is_wayland_session())
        restoreScreenshotKeysX11();
    restoreGnomeShortcuts();

    cleanupInotify();
    cleanupX11();

    monitor_started = false;
    monitor_stopping = false;
    ScreenCaptureLog(2, "=== SCREENSHOT MONITOR STOPPED ===\n");
}

void stop_screenshot_monitor() {
    if (!monitor_started || monitor_stopping) return;
    monitor_stopping = true;
    g_stopping = true;
    g_running = false;
    ScreenCaptureLog(2, "Stop signal received – waiting for threads...\n");
}

// ---------- Signal handler ----------
static void signalHandler(int sig) {
    (void)sig;
    if (monitor_stopping) return;
    RemoveFirefoxScreenshotPolicy();
    stop_screenshot_monitor();
}

// ---------- main() ----------
int main(int argc, char** argv) {
    if (geteuid() != 0) {
        std::cerr << "ERROR: Must run as root.\n";
        return 1;
    }

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    if (SetFirefoxScreenshotPolicy(true)) {
        ScreenCaptureLog(1, "Firefox policy set: Success!\n");
    } else {
        ScreenCaptureLog(1, "Firefox policy set: Failed!\n");
    }

    if (!init_screenshot_monitor()) {
        ScreenCaptureLog(1, "Initialization failed.\n");
        return 1;
    }

    start_screenshot_monitor();

    RemoveFirefoxScreenshotPolicy();
    ScreenCaptureLog(2, "=== Exited ===\n");
    return 0;
}
