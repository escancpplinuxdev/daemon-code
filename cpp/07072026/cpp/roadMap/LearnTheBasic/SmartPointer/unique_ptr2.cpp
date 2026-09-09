
#include <iostream>
#include <cstdarg> // for va_start() , va_end()
#include <memory>  // for smart_ptr
#include <vector>
#include <string>
#include <cstdio> 
#include <unistd.h> 

using namespace std;

// ============================================================
// Forward declaration of log function
// ============================================================
void logMessage(FILE* fp, const char* fmt, ...);

// ============================================================
// LOGGING FUNCTION - writes to file (no sleep, no global)
// ============================================================
void logMessage(FILE* fp, const char* fmt, ...)
{
    if (!fp) return;
    
    va_list args;
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    fprintf(fp, "\n");
	sleep(2);
    fflush(fp);
    va_end(args);
}

// ============================================================
// BASE CLASS: Resource
// ============================================================

class Resource
{
public:
    int id;
    string name;
    FILE* logFp;

    Resource(int id = 0, const string& name = "Default", FILE* fp = nullptr) 
        : id(id), name(name), logFp(fp)
    {
        logMessage(logFp, "🔧 Resource [%d] '%s' ACQUIRED", id, name.c_str());
    }

    ~Resource()
    {
        logMessage(logFp, "💥 Resource [%d] '%s' DESTROYED", id, name.c_str());
    }

    void doWork() const
    {
        logMessage(logFp, "⚙️  Resource [%d] is working...", id);
    }

    void display() const
    {
        logMessage(logFp, "📦 Resource ID: %d, Name: %s", id, name.c_str());
    }
};

// ============================================================
// 1. std::unique_ptr Demo
// ============================================================

void uniquePtrDemo(FILE* fp)
{
    logMessage(fp, "\n========== UNIQUE_PTR DEMO ==========");

    logMessage(fp, "\n[1.1] Creating unique_ptr with make_unique");
    auto uPtr1 = make_unique<Resource>(101, "UniqueRes1", fp);
    uPtr1->doWork();

    logMessage(fp, "\n[1.2] Creating unique_ptr with new");
    unique_ptr<Resource> uPtr2(new Resource(102, "UniqueRes2", fp));
    uPtr2->doWork();

    logMessage(fp, "\n[1.3] Moving ownership (copy is not allowed)");
    unique_ptr<Resource> uPtr3 = move(uPtr1);

    if (uPtr1 == nullptr)
        logMessage(fp, "⚠️  uPtr1 is now nullptr (ownership moved to uPtr3)");
    
    uPtr3->doWork();

    logMessage(fp, "\n[1.4] Getting raw pointer using get()");
    Resource* rawPtr = uPtr2.get();
    rawPtr->doWork();

    logMessage(fp, "\n[1.5] Releasing ownership (manual delete required)");
    Resource* releasedRes = uPtr3.release();
    releasedRes->doWork();
    delete releasedRes;
    logMessage(fp, "✅ Released resource deleted manually");

    logMessage(fp, "\n[1.6] Reset - deletes current and optionally takes new");
    uPtr2.reset();
    logMessage(fp, "✅ uPtr2 reset to null");

    uPtr2.reset(new Resource(103, "NewRes", fp));
    uPtr2->doWork();

    logMessage(fp, "\n[1.7] Unique_ptr in vector container");
    vector<unique_ptr<Resource>> container;
    container.push_back(make_unique<Resource>(201, "ContainerRes1", fp));
    container.push_back(make_unique<Resource>(202, "ContainerRes2", fp));

    for (const auto& res : container)
    {
        res->display();
    }

    logMessage(fp, "\n[1.8] Transfer from container (move)");
    unique_ptr<Resource> adopted = move(container[0]);
    logMessage(fp, "✅ Adopted resource: ");
    adopted->display();
    logMessage(fp, "⚠️  container[0] is now null");

    logMessage(fp, "\n--- uniquePtrDemo() ending - all unique_ptrs destroyed ---");
}

// ============================================================
// 2. std::shared_ptr Demo
// ============================================================

void sharedPtrDemo(FILE* fp)
{
    logMessage(fp, "\n========== SHARED_PTR DEMO ==========");

    logMessage(fp, "\n[2.1] Creating shared_ptr with make_shared");
    auto sPtr1 = make_shared<Resource>(301, "SharedRes1", fp);
    sPtr1->doWork();

    logMessage(fp, "\n[2.2] Copying shared_ptr (shared ownership)");
    shared_ptr<Resource> sPtr2 = sPtr1;
    shared_ptr<Resource> sPtr3(sPtr1);

    logMessage(fp, "📊 Reference count: %ld", sPtr1.use_count());

    logMessage(fp, "\n[2.3] Modifying through any shared_ptr");
    sPtr2->name = "ModifiedSharedRes";
    sPtr3->display();

    logMessage(fp, "\n[2.4] Creating shared_ptr with new");
    shared_ptr<Resource> sPtr4(new Resource(302, "SharedRes2", fp));

    logMessage(fp, "\n[2.5] shared_ptr in vector container");
    vector<shared_ptr<Resource>> container;
    container.push_back(sPtr1);
    container.push_back(sPtr4);
    container.push_back(make_shared<Resource>(303, "SharedRes3", fp));

    logMessage(fp, "📊 Reference count (sPtr1): %ld", sPtr1.use_count());

    for (const auto& res : container)
    {
        res->display();
    }

    logMessage(fp, "\n[2.6] Temporary shared_ptr in scope");
    {
        shared_ptr<Resource> temp = sPtr1;
        logMessage(fp, "📊 Inside scope: ref count = %ld", sPtr1.use_count());
    }
    logMessage(fp, "📊 After scope: ref count = %ld", sPtr1.use_count());

    logMessage(fp, "\n[2.7] Reset - decreases reference count");
    sPtr2.reset();
    logMessage(fp, "📊 After sPtr2.reset(): ref count = %ld", sPtr1.use_count());

    logMessage(fp, "\n[2.8] Getting raw pointer using get()");
    Resource* rawShared = sPtr3.get();
    rawShared->doWork();

    logMessage(fp, "\n[2.9] Move semantics");
    shared_ptr<Resource> sPtr5 = make_shared<Resource>(304, "SharedRes4", fp);
    shared_ptr<Resource> sPtr6 = move(sPtr5);
    if (sPtr5 == nullptr)
        logMessage(fp, "⚠️  sPtr5 is now null (moved to sPtr6)");
    sPtr6->doWork();

    logMessage(fp, "\n--- sharedPtrDemo() ending - all shared_ptrs destroyed ---");
}

// ============================================================
// 3. std::weak_ptr Demo
// ============================================================

void weakPtrDemo(FILE* fp)
{
    logMessage(fp, "\n========== WEAK_PTR DEMO ==========");

    logMessage(fp, "\n[3.1] Creating weak_ptr from shared_ptr");
    auto sPtr = make_shared<Resource>(401, "WeakRes1", fp);
    weak_ptr<Resource> wPtr = sPtr;

    logMessage(fp, "📊 weak_ptr expired? %s", (wPtr.expired() ? "YES" : "NO"));

    logMessage(fp, "\n[3.2] Using lock() to get temporary shared_ptr");
    if (auto temp = wPtr.lock())
    {
        logMessage(fp, "✅ Successfully locked weak_ptr: ");
        temp->display();
        logMessage(fp, "📊 Reference count while locked: %ld", temp.use_count());
    }
    else
    {
        logMessage(fp, "❌ Object is gone!");
    }

    logMessage(fp, "\n[3.3] weak_ptr doesn't affect reference count");
    logMessage(fp, "📊 Reference count (sPtr): %ld", sPtr.use_count());
    logMessage(fp, "   (weak_ptr doesn't increase ref count)");

    logMessage(fp, "\n[3.4] Breaking cycles with weak_ptr");
    
    struct Node
    {
        int id;
        shared_ptr<Node> next;
        weak_ptr<Node> weakNext;
        FILE* logFp;
        
        Node(int i, FILE* fp) : id(i), logFp(fp)
        {
            logMessage(logFp, "🔧 Node %d created", id);
        }
        ~Node()
        {
            logMessage(logFp, "💥 Node %d destroyed", id);
        }
    };

    logMessage(fp, "\n[3.4a] Shared_ptr cycle (BAD - memory leak)");
    auto node1 = make_shared<Node>(1, fp);
    auto node2 = make_shared<Node>(2, fp);
    node1->next = node2;
    node2->next = node1;
    logMessage(fp, "⚠️  Node1 and Node2 hold shared_ptr to each other - memory leak!");

    logMessage(fp, "\n[3.4b] Weak_ptr breaks cycle (GOOD - no leak)");
    auto node3 = make_shared<Node>(3, fp);
    auto node4 = make_shared<Node>(4, fp);
    node3->weakNext = node4;
    node4->weakNext = node3;
    logMessage(fp, "✅ Weak_ptr breaks the cycle - no memory leak!");

    logMessage(fp, "\n[3.5] weak_ptr in container");
    vector<weak_ptr<Resource>> observers;
    observers.push_back(wPtr);
    observers.push_back(sPtr);

    vector<weak_ptr<Resource>> weakObservers;
    weakObservers.push_back(sPtr);
    weakObservers.push_back(make_shared<Resource>(402, "WeakRes2", fp));

    logMessage(fp, "\n[3.6] Cleaning up expired weak_ptrs");
    sPtr.reset();
    logMessage(fp, "✅ sPtr reset - Resource 401 destroyed");

    for (auto& w : weakObservers)
    {
        if (auto sp = w.lock())
            logMessage(fp, "✅ Still alive: %s", sp->name.c_str());
        else
            logMessage(fp, "❌ Expired (already destroyed)");
    }

    logMessage(fp, "\n[3.7] Factory pattern with weak_ptr");
    auto factoryShared = make_shared<Resource>(403, "FactoryRes", fp);
    weak_ptr<Resource> factoryWeak = factoryShared;

    if (auto temp = factoryWeak.lock())
    {
        logMessage(fp, "✅ Factory object still exists: ");
        temp->display();
    }

    logMessage(fp, "\n--- weakPtrDemo() ending - all weak_ptrs destroyed ---");
}

// ============================================================
// MAIN FUNCTION
// ============================================================

int main()
{
    // Open log file once
    FILE* fp = fopen("/tmp/smart_ptr.txt", "w");
    if (!fp)
    {
        fprintf(stderr, "Failed to open /tmp/smart_ptr.txt for writing\n");
        return 1;
    }

    // Write header
    fprintf(fp, "============================================================\n");
    fprintf(fp, "     SMART POINTERS COMPLETE DEMO (C++14)\n");
    fprintf(fp, "============================================================\n");
    fflush(fp);

    // Run demos
    uniquePtrDemo(fp);
    sharedPtrDemo(fp);
    weakPtrDemo(fp);

    // Write footer
    fprintf(fp, "\n============================================================\n");
    fprintf(fp, "     ALL DEMOS COMPLETED SUCCESSFULLY\n");
    fprintf(fp, "============================================================\n");
    fflush(fp);

    // Close file
    fclose(fp);

    return 0;
}
