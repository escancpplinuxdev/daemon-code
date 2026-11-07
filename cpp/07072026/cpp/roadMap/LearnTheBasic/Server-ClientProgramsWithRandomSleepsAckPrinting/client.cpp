#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

// Scale factor: 1 "minute" = TIME_SCALE seconds.
const double TIME_SCALE = 60.0;   // set to 1.0 for fast test
const double SEND_INTERVAL_MIN = 0.9;

// --- Formatting helpers for pretty terminal output ---
void printBanner(const string& tag, const string& msg) {
    cout << "\n╔══════════════════════════════════════════╗\n";
    cout << "║ [" << tag << "] " << msg << "\n";
    cout << "╚══════════════════════════════════════════╝" << endl;
}

void printClientSend(const string& msg) {
    cout << "\033[1;33m[CLIENT SENT]\033[0m >>> " << msg << endl;
}

void printAckRecv(const string& msg) {
    cout << "\033[1;35m[ACK-RECV by Client]\033[0m <<< " << msg << endl;
}

void printAckSend(const string& msg) {
    cout << "\033[1;32m[ACK-SENT by Client]\033[0m >>> " << msg << endl;
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("socket"); return 1; }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect"); return 1;
    }

    printBanner("CLIENT", "Connected to server on 127.0.0.1:8080");

    int msgCounter = 0;
    double intervalSec = SEND_INTERVAL_MIN * TIME_SCALE;

    while (true) {
        msgCounter++;
        string msg = "Hello from client, msg #" + to_string(msgCounter);

        // --- Step 1: Send message to server ---
        send(sock, msg.c_str(), msg.size(), 0);
        printClientSend(msg);

        // --- Step 2: Read server's ACK ---
        char ackBuf[256] = {0};
        int ackBytes = read(sock, ackBuf, sizeof(ackBuf) - 1);
        if (ackBytes <= 0) {
            printBanner("CLIENT", "Server disconnected.");
            break;
        }
        string serverAck(ackBuf, ackBytes);
        printAckRecv(serverAck);

        // --- Step 3: Send our ACK of the server's ACK ---
        string myAck = "CLIENT-ACK-" + to_string(msgCounter);
        send(sock, myAck.c_str(), myAck.size(), 0);
        printAckSend(myAck);

        // --- Step 4: Wait 0.9 min before next message ---
        printBanner("CLIENT", "Next msg in " + to_string(SEND_INTERVAL_MIN) +
                              " min (" + to_string((int)intervalSec) + "s)...");
        this_thread::sleep_for(chrono::milliseconds((long long)(intervalSec * 1000)));
    }

    close(sock);
    return 0;
}
