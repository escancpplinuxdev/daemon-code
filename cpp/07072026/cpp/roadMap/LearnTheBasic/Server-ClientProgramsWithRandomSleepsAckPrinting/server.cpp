#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

// Scale factor: 1 "minute" in the problem = TIME_SCALE seconds here.
// Set to 60.0 for real minutes, or e.g. 1.0 to speed up testing.
const double TIME_SCALE = 60.0;   // 1 min = 60 sec (set to 1.0 for fast test)

// Random sleep durations (in minutes) for the server between reads
const vector<double> SLEEP_MINUTES = {2.0, 3.0, 1.5, 2.9, 3.2, 0.7, 5.0};

// --- Formatting helpers for pretty terminal output ---
void printBanner(const string& tag, const string& msg) {
    cout << "\n╔══════════════════════════════════════════╗\n";
    cout << "║ [" << tag << "] " << msg << "\n";
    cout << "╚══════════════════════════════════════════╝" << endl;
}

void printServer(const string& msg) {
    // Distinct format for SERVER printing
    cout << "\033[1;36m[SERVER]\033[0m >>> " << msg << endl;
}

void printAckSend(const string& msg) {
    cout << "\033[1;32m[ACK-SENT by Server]\033[0m <<< " << msg << endl;
}

void printAckRecv(const string& msg) {
    cout << "\033[1;35m[ACK-RECV by Server]\033[0m >>> " << msg << endl;
}

int main() {
    srand((unsigned)time(nullptr));

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); return 1; }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind"); return 1;
    }
    if (listen(server_fd, 1) < 0) {
        perror("listen"); return 1;
    }

    printBanner("SERVER", "Listening on port 8080...");

    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) { perror("accept"); return 1; }

    printBanner("SERVER", "Client connected!");

    int msgCounter = 0;
    while (true) {
        char buf[1024] = {0};
        int bytesRead = read(client_fd, buf, sizeof(buf) - 1);
        if (bytesRead <= 0) {
            printBanner("SERVER", "Client disconnected.");
            break;
        }

        string msg(buf, bytesRead);
        msgCounter++;

        // --- Step 1: Immediately send an ACK back to client ---
        string ackMsg = "ACK-" + to_string(msgCounter);
        send(client_fd, ackMsg.c_str(), ackMsg.size(), 0);
        printAckSend(ackMsg);

        // --- Step 2: Choose a random sleep from the fixed list ---
        double sleepMin = SLEEP_MINUTES[rand() % SLEEP_MINUTES.size()];
        double sleepSec = sleepMin * TIME_SCALE;

        printBanner("SERVER", "Received msg #" + to_string(msgCounter) +
                              ". Sleeping " + to_string(sleepMin) + " min (" +
                              to_string((int)sleepSec) + "s) before printing...");

        this_thread::sleep_for(chrono::milliseconds((long long)(sleepSec * 1000)));

        // --- Step 3: Print the message on the server terminal ---
        printServer("MSG #" + to_string(msgCounter) + ": " + msg);

        // --- Step 4: Wait for client's ACK of our ACK ---
        char ackBuf[256] = {0};
        int ackBytes = read(client_fd, ackBuf, sizeof(ackBuf) - 1);
        if (ackBytes > 0) {
            printAckRecv(string(ackBuf, ackBytes));
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
