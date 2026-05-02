#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>

#include "scanning_file.h"
#include "scanLog.h"
#include "main.h"
//#include "eventsender.h"
#include "patternMatch.h"
#include "extractPatterns.h"
#include "initializeRegexes.h"
#include "readConfiguration.h"
#include "RegexCache.h"


//Created and written By the Ketan <ketan@escanav.com>
bool isBlock;

time_t oneDriveTokenExpiryTime;
int expiresIn = 12600;
time_t currentTime = time(NULL);
time_t tokenExpiryTime = currentTime + expiresIn;
int debugLevelRTM = 4;

std::mutex cout_mutex;

// ==================== SESSION TYPE DETECTION ====================
enum class SessionType {
     X11,
     WAYLAND,
     UNKNOWN
};

SessionType detectSessionType() {
     const char* xdg_session_type = std::getenv("XDG_SESSION_TYPE");

     if (xdg_session_type) {
          std::string session(xdg_session_type);
          if (session == "wayland") {
               return SessionType::WAYLAND;
          } else if (session == "x11") {
               return SessionType::X11;
          }
     }

     if (std::getenv("DISPLAY")) {
          return SessionType::X11;
     }

     return SessionType::UNKNOWN;
}

// ==================== WAYLAND CLIPBOARD HANDLING ====================
std::string getWaylandClipboardContent() {
     FILE* pipe = popen("timeout 0.5 wl-paste 2>/dev/null", "r");

     if (!pipe) {
          return "";
     }

     std::string result;
     char buffer[256];
     while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
          result += buffer;
     }
     pclose(pipe);

     if (!result.empty() && result.back() == '\n') {
          result.pop_back();
     }

     return result;
}

bool setWaylandClipboardContent(const std::string& content) {
     std::string escaped = content;
     for (size_t i = 0; i < escaped.length(); ++i) {
          if (escaped[i] == '"' || escaped[i] == '\\' || escaped[i] == '$') {
               escaped.insert(i, 1, '\\');
               ++i;
          }
     }

     std::string command = "echo -n \"" + escaped + "\" | wl-copy 2>/dev/null &";
     int result = system(command.c_str());

     return (result == 0);
}

void waylandClipboardMonitor() {
     std::string lastClipboard;

     std::cout << "[*] Wayland Clipboard Monitor started" << std::endl;
     scanLog(3, "[*] Wayland Clipboard Monitor started");

     while (true) {
          std::string clipboard = getWaylandClipboardContent();

          if (!clipboard.empty() && clipboard != lastClipboard) {
               {
                    std::lock_guard<std::mutex> lock(cout_mutex);
                    std::cout << "[+] Clipboard changed: " << clipboard << "\n";
               }
               lastClipboard = clipboard;

               std::string eventValue;
               if (patternMatch(clipboard, NULL, eventValue)) {
                    if (isBlock) {
                         std::cout << "string= " << eventValue << std::endl;
                         std::lock_guard<std::mutex> lock(cout_mutex);
                         std::cout << "[!] Blocking sensitive clipboard content.\n";

                         char logLine[1024] = "";
                         snprintf(logLine, sizeof(logLine), "%s Blocking sensitive clipboard content", eventValue.c_str());
                         scanLog(1, logLine);

                         setWaylandClipboardContent("[Blocked: Permission Denied]");
                    } else {
                         std::cout << "string= " << eventValue << std::endl;
                         std::lock_guard<std::mutex> lock(cout_mutex);
                         std::cout << "[!] Monitoring sensitive clipboard content.\n";

                         char logLine[1024] = "";
                         snprintf(logLine, sizeof(logLine), "%s Monitoring sensitive clipboard content", eventValue.c_str());
                         scanLog(1, logLine);
                    }
               }
          }

          usleep(500000);
     }
}

// ==================== ORIGINAL X11 CODE ====================
class X11DaemonSetup {
     private:
          std::string activeUser;
          std::string display;
          std::string xauthority;

          // Execute command and return output
          std::string executeCommand(const std::string& command) {
               std::string result;
               FILE* pipe = popen(command.c_str(), "r");
               if (!pipe) {
                    return "";
               }

               char buffer[128];
               while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                    result += buffer;
               }
               pclose(pipe);

               // Remove trailing newline
               if (!result.empty() && result.back() == '\n') {
                    result.pop_back();
               }

               return result;
          }

          // Check if file exists
          bool fileExists(const std::string& filename) {
               struct stat buffer;
               return (stat(filename.c_str(), &buffer) == 0);
          }

          // Find active user session
          std::string findActiveUser() {
               // First method: Use 'who' command to find active user
               std::string whoOutput = executeCommand("who | awk '/\\(:[0-9]+\\)/ {print $1; exit}'");
               if (!whoOutput.empty()) {
                    std::cout << "Found active user via 'who' command: " << whoOutput << std::endl;
                    return whoOutput;
               }

               // Fallback: Find user from X process
               std::string psOutput = executeCommand("ps aux | grep -E 'X(:0|11)' | grep -v grep | awk '{print $1}' | head -1");
               if (!psOutput.empty()) {
                    std::cout << "Found active user via X process: " << psOutput << std::endl;
                    return psOutput;
               }

               return "";
          }

          // Set environment variables
          void setEnvironmentVariables() {
               display = ":0";
               xauthority = "/home/" + activeUser + "/.Xauthority";

               // Set DISPLAY environment variable
               if (setenv("DISPLAY", display.c_str(), 1) != 0) {
                    std::cerr << "Failed to set DISPLAY environment variable" << std::endl;
               }

               // Set XAUTHORITY environment variable
               if (setenv("XAUTHORITY", xauthority.c_str(), 1) != 0) {
                    std::cerr << "Failed to set XAUTHORITY environment variable" << std::endl;
               }
          }

          // Copy Xauthority file to accessible location
          bool copyXauthorityFile() {
               std::string originalXauth = "/home/" + activeUser + "/.Xauthority";

               if (!fileExists(originalXauth)) {
                    std::cout << "Original .Xauthority file not found: " << originalXauth << std::endl;
                    return false;
               }

               // Create temporary Xauth file
               std::string tempXauth = "/tmp/.Xauth_daemon_" + std::to_string(getpid());

               std::string copyCommand = "cp \"" + originalXauth + "\" \"" + tempXauth + "\"";
               int result = system(copyCommand.c_str());

               if (result == 0) {
                    // Change permissions
                    std::string chmodCommand = "chmod 644 \"" + tempXauth + "\"";
                    system(chmodCommand.c_str());

                    // Update XAUTHORITY to point to temp file
                    xauthority = tempXauth;
                    if (setenv("XAUTHORITY", xauthority.c_str(), 1) != 0) {
                         std::cerr << "Failed to update XAUTHORITY environment variable" << std::endl;
                         return false;
                    }

                    std::cout << "Copied .Xauthority to: " << tempXauth << std::endl;
                    return true;
               }

               std::cerr << "Failed to copy .Xauthority file" << std::endl;
               return false;
          }

          // Set up X11 permissions
          void setupX11Permissions() {
               // Check if xhost command exists
               std::string xhostCheck = executeCommand("command -v xhost");
               if (xhostCheck.empty()) {
                    std::cout << "xhost command not found, skipping permission setup" << std::endl;
                    return;
               }

               // Run xhost command as the active user
               std::string xhostCommand = "su - \"" + activeUser + "\" -c \"xhost +SI:localuser:root\" 2>/dev/null";
               int result = system(xhostCommand.c_str());

               if (result == 0) {
                    std::cout << "X11 permissions setup successful" << std::endl;
               } else {
                    std::cout << "X11 permissions setup failed (this may be normal)" << std::endl;
               }
          }

     public:
          // Main setup function
          bool setupX11ForDaemon() {
               std::cout << "Starting X11 environment setup for daemon services..." << std::endl;

               // Find active user
               activeUser = findActiveUser();
               if (activeUser.empty()) {
                    std::cerr << "No active user session found" << std::endl;
                    return false;
               }

               std::cout << "Setting up X11 environment for user: " << activeUser << std::endl;

               // Set environment variables
               setEnvironmentVariables();

               // Copy Xauthority file to accessible location
               copyXauthorityFile();

               // Set up X11 permissions
               setupX11Permissions();

               std::cout << "X11 environment setup complete" << std::endl;
               std::cout << "DISPLAY=" << display << std::endl;
               std::cout << "XAUTHORITY=" << xauthority << std::endl;

               return true;
          }

          // Get current environment info
          void printEnvironmentInfo() {
               std::cout << "Current X11 Environment:" << std::endl;
               std::cout << "DISPLAY=" << (getenv("DISPLAY") ? getenv("DISPLAY") : "not set") << std::endl;
               std::cout << "XAUTHORITY=" << (getenv("XAUTHORITY") ? getenv("XAUTHORITY") : "not set") << std::endl;
          }

          // Clean up temporary files
          void cleanup() {
               std::string tempXauth = "/tmp/.Xauth_daemon_" + std::to_string(getpid());
               if (fileExists(tempXauth)) {
                    std::string rmCommand = "rm -f \"" + tempXauth + "\"";
                    system(rmCommand.c_str());
                    std::cout << "Cleaned up temporary Xauthority file" << std::endl;
               }
          }
};

std::string getClipboardContent(Display* display, const std::string& selectionName) {
     Atom selection = XInternAtom(display, selectionName.c_str(), False);
     Atom target = XInternAtom(display, "UTF8_STRING", False);
     Atom property = XInternAtom(display, "XSEL_DATA", False);

     Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 1, 1, 0, 0, 0);
     XConvertSelection(display, selection, target, property, window, CurrentTime);
     XFlush(display);

     std::string result;
     XEvent event;

     while (true) {
          XNextEvent(display, &event);
          if (event.type == SelectionNotify && event.xselection.selection == selection) {
               if (event.xselection.property) {
                    Atom type;
                    int format;
                    unsigned long len, bytes_left;
                    unsigned char* data = nullptr;

                    if (XGetWindowProperty(display, window, property, 0, (~0L), False,
                                   AnyPropertyType, &type, &format,
                                   &len, &bytes_left, &data) == Success && data) {
                         result = std::string(reinterpret_cast<char*>(data), len);
                         XFree(data);
                    }
               }
               break;
          }
     }

     XDestroyWindow(display, window);
     return result;
}


/*std::string getClipboardContent(Display* display, const std::string& selectionName) {
  Atom selection = XInternAtom(display, selectionName.c_str(), False);
  Atom target = XInternAtom(display, "UTF8_STRING", False);
  Atom property = XInternAtom(display, "XSEL_DATA", False);

  Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 1, 1, 0, 0, 0);
  XConvertSelection(display, selection, target, property, window, CurrentTime);
  XFlush(display);

  std::string result;
  XEvent event;

  while (true) {
  XNextEvent(display, &event);
  if (event.type == SelectionNotify) {
  if (event.xselection.property != None) {
  Atom type;
  int format;
  unsigned long nitems, bytes_after;
  unsigned char *buffer = nullptr;

  if (XGetWindowProperty(display, window, property, 0, 0x1fffffff, False, AnyPropertyType,
  &type, &format, &nitems, &bytes_after, &buffer) == Success) {
  if (buffer) {
  result = reinterpret_cast<const char*>(buffer);
  XFree(buffer);
  }
  }
  }
  XDestroyWindow(display, window);
  break;
  }
  }

  return result;
  }
 */
void setClipboardContent(Display* display, const std::string& newText) {
     Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 1, 1, 0, 0, 0);
     XSelectInput(display, window, PropertyChangeMask);

     Atom clipboard = XInternAtom(display, "CLIPBOARD", False);
     XSetSelectionOwner(display, clipboard, window, CurrentTime);
     XFlush(display);

     if (XGetSelectionOwner(display, clipboard) != window) {
          scanLog(3,"Failed to own CLIPBOARD");
          return;
     }

     Atom utf8 = XInternAtom(display, "UTF8_STRING", False);
     Atom text = XInternAtom(display, "TEXT", False);
     Atom targets = XInternAtom(display, "TARGETS", False);

     if (XSetSelectionOwner(display, XA_PRIMARY, window, CurrentTime) == 0) {
          scanLog(3,"Failed to own PRIMARY");
          return;
     }

     bool done = false;
     unsigned long startTime = time(nullptr);

     while (!done) {
          while (XPending(display)) {
               XEvent event;
               XNextEvent(display, &event);

               if (event.type == SelectionRequest) {
                    XSelectionRequestEvent* req = &event.xselectionrequest;
                    XSelectionEvent sev{};
                    sev.type = SelectionNotify;
                    sev.display = req->display;
                    sev.requestor = req->requestor;
                    sev.selection = req->selection;
                    sev.target = req->target;
                    sev.time = req->time;
                    sev.property = req->property;

                    if (req->target == targets) {
                         Atom available[] = {utf8, text};
                         XChangeProperty(display, req->requestor, req->property, XA_ATOM, 32,
                                   PropModeReplace, (unsigned char*)available, 2);
                    } else if (req->target == utf8 || req->target == text) {
                         XChangeProperty(display, req->requestor, req->property, req->target, 8,
                                   PropModeReplace, (const unsigned char*)newText.c_str(), newText.length());
                         done = true;
                    } else {
                         sev.property = None;
                    }

                    XSendEvent(display, req->requestor, True, 0, (XEvent*)&sev);
                    XFlush(display);
               }
          }

          if (time(nullptr) - startTime > 3) {
               break;
          }

          usleep(10000);
     }

     XDestroyWindow(display, window);
}

void ownSelection(Display* display, Atom selectionAtom, const std::string& newText) {
     Atom utf8 = XInternAtom(display, "UTF8_STRING", False);
     Atom targets = XInternAtom(display, "TARGETS", False);
     Atom text = XInternAtom(display, "TEXT", False);

     Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 1, 1, 0, 0, 0);
     XSetSelectionOwner(display, selectionAtom, window, CurrentTime);
     XFlush(display);

     if (XGetSelectionOwner(display, selectionAtom) != window) {
          std::lock_guard<std::mutex> lock(cout_mutex);
          std::cerr << "[!] Failed to own PRIMARY.\n";
          XDestroyWindow(display, window);
          scanLog(3,"Failed to own PRIMARY");
          return;
     }

     bool done = false;
     unsigned long startTime = time(nullptr);

     while (!done) {
          while (XPending(display)) {
               XEvent event;
               XNextEvent(display, &event);

               if (event.type == SelectionRequest) {
                    XSelectionRequestEvent* req = &event.xselectionrequest;
                    XSelectionEvent sev{};
                    sev.type = SelectionNotify;
                    sev.display = req->display;
                    sev.requestor = req->requestor;
                    sev.selection = req->selection;
                    sev.target = req->target;
                    sev.time = req->time;
                    sev.property = req->property;

                    if (req->target == targets) {
                         Atom available[] = {utf8, text};
                         XChangeProperty(display, req->requestor, req->property, XA_ATOM, 32,
                                   PropModeReplace, (unsigned char*)available, 2);
                    } else if (req->target == utf8 || req->target == text) {
                         XChangeProperty(display, req->requestor, req->property, req->target, 8,
                                   PropModeReplace, (const unsigned char*)newText.c_str(), newText.length());
                         done = true;
                    } else {
                         sev.property = None;
                    }

                    XSendEvent(display, req->requestor, True, 0, (XEvent*)&sev);
                    XFlush(display);
               }
          }

          if (time(nullptr) - startTime > 3) {
               break;
          }

          usleep(10000);
     }

     XDestroyWindow(display, window);
}

Display* openDisplayWithFallback() {
     Display* display = nullptr;

     display = XOpenDisplay(nullptr);
     if (display) {
          return display;
     }

     const char* displays[] = {":0", ":0.0", ":1", ":1.0", nullptr};

     for (int i = 0; displays[i] != nullptr; i++) {
          display = XOpenDisplay(displays[i]);
          if (display) {
               std::cout << "Connected to display: " << displays[i] << std::endl;
               return display;
          }
     }

     return nullptr;
}

void mouseEvent()
{
     Display* display = openDisplayWithFallback();
     if (!display) {
          std::cerr << "Unable to open display for PRIMARY.\n";
          scanLog(1,"Unable to open display for PRIMARY");
          return;
     }

     int i = 0;
     std::string lastPrimary;

     while (true) {
          std::string primary = getClipboardContent(display, "PRIMARY");
          if ((!primary.empty() && primary != lastPrimary) || i == 1) {
               {
                    i = 0;
                    std::lock_guard<std::mutex> lock(cout_mutex);
                    std::cout << "[+] Mouse-selected (PRIMARY) changed: " << primary << "\n";
               }
               lastPrimary = primary;

               std::string eventValue;
               if(patternMatch(primary,NULL,eventValue)){
                    {
                         if(isBlock)
                         {
                              //Added by Omkar Gulambe on 5 March 2026 for custom words
                              std::string iseventWlstr = eventValue.substr(0,9);
                              if(iseventWlstr == "[WLSstr |")
                              {
                                   std::cout << "[!] Monitoring sensitive PRIMARY selection.\n";
                                   char logLine[1024]="";
                                   snprintf(logLine,sizeof(logLine),"%s Monitoring sensitive PRIMARY selection",eventValue.c_str());
                                   scanLog(1,logLine);
                              }
                              else
                              {
                                   i =1;
                                   std::lock_guard<std::mutex> lock(cout_mutex);
                                   std::cout << "[!] Blocking sensitive PRIMARY selection.\n";
                                   char logLine[1024]="";
                                   snprintf(logLine,sizeof(logLine),"%s Blocking sensitive PRIMARY Selection",eventValue.c_str());
                                   scanLog(1,logLine);
                                   ownSelection(display, XA_PRIMARY, "[Blocked: Sensitive Data]");
                              } 
                         }
                         else
                         {
                              std::cout<<"string= "<<eventValue<<std::endl;
                              std::cout << "[!] Monitoring sensitive PRIMARY selection.\n";
                              char logLine[1024]="";
                              snprintf(logLine,sizeof(logLine),"%s Monitoring sensitive PRIMARY selection",eventValue.c_str());
                              scanLog(1,logLine);

                         }
                    }

               }
          }
          usleep(300000);
     }

     XCloseDisplay(display);


}

void clipboardEvent()
{
     Display* display = openDisplayWithFallback();
     if (!display) {
          std::cerr << "Unable to open display for CLIPBOARD.\n";
          scanLog(1,"Unable to open display for CLIPBOARD");
          return ;
     }

     std::string lastClipboard;

     while (true) {
          std::string clipboard = getClipboardContent(display, "CLIPBOARD");
          if (!clipboard.empty() && clipboard != lastClipboard) {
               {
                    std::lock_guard<std::mutex> lock(cout_mutex);
                    std::cout << "[+] Clipboard changed: " << clipboard << "\n";

               }
               lastClipboard = clipboard;

               std::string eventValue;
               if(patternMatch(clipboard,NULL,eventValue)){
                    {
                         if(isBlock)
                         {
                              //Added by Omkar Gulambe on 5 March 2026 for custom words
                              std::string iseventWlstr = eventValue.substr(0,9);
                              if(iseventWlstr == "[WLSstr |")
                              {
                                   std::cout << "[!] Monitoring sensitive clipboard content.\n";
                                   char logLine[1024]="";
                                   snprintf(logLine,sizeof(logLine),"%s Monitoring sensitive clipboard content",eventValue.c_str());
                                   scanLog(1,logLine); 
                              }
                              else
                              {
                                   std::cout<<"string= "<<eventValue<<std::endl;
                                   std::lock_guard<std::mutex> lock(cout_mutex);
                                   std::cout << "[!] Blocking sensitive clipboard content.\n";
                                   char logLine[1024]="";
                                   snprintf(logLine,sizeof(logLine),"%s Blocking sensitive clipboard content",eventValue.c_str());
                                   scanLog(1,logLine);
                                   setClipboardContent(display, "[Blocked: Permission Denied]");
                              }
                         }
                         else
                         {
                              std::cout<<"string= "<<eventValue<<std::endl;
                              std::cout << "[!] Monitoring sensitive clipboard content.\n";
                              char logLine[1024]="";
                              snprintf(logLine,sizeof(logLine),"%s Monitoring sensitive clipboard content",eventValue.c_str());
                              scanLog(1,logLine);
                         }
                    }
               }
          }
          usleep(300000);
     }

     XCloseDisplay(display);

}


int main() {
     SessionType session = detectSessionType();

     if (session == SessionType::WAYLAND) {
          std::cout << "\n--- Wayland Setup ---" << std::endl;
          scanLog(3, "--- Wayland Setup ---");

          if (system("command -v wl-paste > /dev/null 2>&1") != 0) {
               std::cerr << "[!] ERROR: wl-clipboard not installed!\n";
               std::cerr << "[!] Install: sudo apt install wl-clipboard\n";
               scanLog(1, "ERROR: wl-clipboard not installed");
               return 1;
          }

          std::cout << "[*] Wayland detected. Starting clipboard monitor...\n";
          scanLog(3, "[*] Wayland clipboard monitor started");

          isBlockPattern = true;

          char status[50] = "";
          const char* confPath = "/opt/MicroWorld/etc/contentScanner.conf";
          if (readConfiguration(confPath, "ContentScanner:clipBoardMonitor", status) == 0) {
               if (atoi(status) == 2) {
                    isBlock = true;
               }
          }

          if (extractPatterns(confPath, "patterns") == false) {
               return 0;
          }
          if(!initializeRegexes("/opt/MicroWorld/etc/contentScanner.conf","customRegex"))
          {
               return -1;
          }

          waylandClipboardMonitor();

     } else {
          X11DaemonSetup x11Setup;
          if (x11Setup.setupX11ForDaemon()) {
               std::cout << "\n--- X11 Setup Successful ---" << std::endl;
               x11Setup.printEnvironmentInfo();
               std::cout << "[*] Watching both CLIPBOARD and PRIMARY in separate threads...\n";
               char status[50]="";
               scanLog(1,"--- X11 Setup Successful ---");
               scanLog(1,"[*] Watching both CLIPBOARD and PRIMARY in separate threads...");

               isBlockPattern = true;

               const char* confPath = "/opt/MicroWorld/etc/contentScanner.conf";
               if (readConfiguration(confPath,"ContentScanner:clipBoardMonitor",status) == 0)
               {
                    if(atoi(status) == 2)
                    {
                         isBlock = true;
                    }
               }
               if(extractPatterns(confPath,"patterns")==false)
               {
                    return 0;
               }
               if(!initializeRegexes("/opt/MicroWorld/etc/contentScanner.conf","customRegex"))
               {
                    return -1;
               }

               std::thread t1(clipboardEvent);
               std::thread t2(mouseEvent);

               t1.join();
               t2.join();

          } else {
               std::cerr << "X11 setup failed!" << std::endl;
               scanLog(1,"X11 setup failed!");
               return 1;
          }
     }

     ClearRegexCache();
     return 0;
}
