
#include <iostream>
#include <memory>   // for smart pointers
#include <vector>   // for containers
#include <string>   // for strings

using namespace std;

// ============================================================
// BASE CLASS: Resource (used by all smart pointer demos)
// ============================================================

class Resource
{
public:
    int id;
    string name;

    // Constructor
    Resource(int id = 0, const string& name = "Default") 
        : id(id), name(name)
    {
        cout << "🔧 Resource [" << id << "] '" << name << "' ACQUIRED\n";
    }

    // Destructor
    ~Resource()
    {
        cout << "💥 Resource [" << id << "] '" << name << "' DESTROYED\n";
    }

    // Member function
    //void doWork() const
    void doWork() 
    {
        cout << "⚙️  Resource [" << id << "] is working...\n";
    }

    // Display info
//    void display() const
    void display() 
    {
        cout << "📦 Resource ID: " << id << ", Name: " << name << "\n";
    }
};

// ============================================================
// 1. std::unique_ptr - Exclusive Ownership (Move-Only)
// ============================================================

void uniquePtrDemo()
{
    cout << "\n========== UNIQUE_PTR DEMO ==========\n";

    // 1.1 Create unique_ptr using make_unique (preferred, C++14)
    cout << "\n[1.1] Creating unique_ptr with make_unique\n";
    auto uPtr1 = make_unique<Resource>(101, "UniqueRes1");
    uPtr1->doWork();

    // 1.2 Create unique_ptr with new (also works)
    cout << "\n[1.2] Creating unique_ptr with new\n";
    unique_ptr<Resource> uPtr2(new Resource(102, "UniqueRes2"));
    uPtr2->doWork();

    // 1.3 Moving ownership (copy is NOT allowed)
    cout << "\n[1.3] Moving ownership (copy is not allowed)\n";
    unique_ptr<Resource> uPtr3 = move(uPtr1);   // ✅ Move - uPtr1 becomes null
    // unique_ptr<Resource> uPtr4 = uPtr1;     // ❌ Compile error! Copy not allowed

    if (uPtr1 == nullptr)
        cout << "⚠️  uPtr1 is now nullptr (ownership moved to uPtr3)\n";
    
    uPtr3->doWork();

    // 1.4 Getting raw pointer (be careful!)
    cout << "\n[1.4] Getting raw pointer using get()\n";
    Resource* rawPtr = uPtr2.get();
    rawPtr->doWork();

    // 1.5 Release ownership (manual delete required)
    cout << "\n[1.5] Releasing ownership (manual delete required)\n";
    Resource* releasedRes = uPtr3.release();   // uPtr3 becomes null
    releasedRes->doWork();
    delete releasedRes;                        // ⚠️ Must delete manually!
    cout << "✅ Released resource deleted manually\n";

    // 1.6 Reset - delete current and optionally take new
    cout << "\n[1.6] Reset - deletes current and optionally takes new\n";
    uPtr2.reset();                             // Deletes Resource 102
    cout << "✅ uPtr2 reset to null\n";

    uPtr2.reset(new Resource(103, "NewRes"));
    uPtr2->doWork();

    // 1.7 Unique_ptr in containers
    cout << "\n[1.7] Unique_ptr in vector container\n";
    vector<unique_ptr<Resource>> container;
    container.push_back(make_unique<Resource>(201, "ContainerRes1"));
    container.push_back(make_unique<Resource>(202, "ContainerRes2"));

    for (const auto& res : container)
    {
        res->display();
    }

    // 1.8 Transfer from container
    cout << "\n[1.8] Transfer from container (move)\n";
    unique_ptr<Resource> adopted = move(container[0]);
    cout << "✅ Adopted resource: ";
    adopted->display();
    cout << "⚠️  container[0] is now null\n";

    // All resources are automatically destroyed when function ends
    cout << "\n--- uniquePtrDemo() ending - all unique_ptrs destroyed ---\n";
}

// ============================================================
// 2. std::shared_ptr - Shared Ownership (Reference Counted)
// ============================================================

void sharedPtrDemo()
{
    cout << "\n========== SHARED_PTR DEMO ==========\n";

    // 2.1 Create shared_ptr using make_shared (preferred)
    cout << "\n[2.1] Creating shared_ptr with make_shared\n";
    auto sPtr1 = make_shared<Resource>(301, "SharedRes1");
    sPtr1->doWork();

    // 2.2 Copy - shared ownership (ref count increases)
    cout << "\n[2.2] Copying shared_ptr (shared ownership)\n";
    shared_ptr<Resource> sPtr2 = sPtr1;    // ✅ Copy - both point to same resource
    shared_ptr<Resource> sPtr3(sPtr1);     // ✅ Another copy

    cout << "📊 Reference count: " << sPtr1.use_count() << "\n";  // Should be 3

    // 2.3 Modifying through any pointer (affects all)
    cout << "\n[2.3] Modifying through any shared_ptr\n";
    sPtr2->name = "ModifiedSharedRes";
    sPtr3->display();   // Shows modified name

    // 2.4 Creating with new (also works)
    cout << "\n[2.4] Creating shared_ptr with new\n";
    shared_ptr<Resource> sPtr4(new Resource(302, "SharedRes2"));

    // 2.5 shared_ptr in containers
    cout << "\n[2.5] shared_ptr in vector container\n";
    vector<shared_ptr<Resource>> container;
    container.push_back(sPtr1);
    container.push_back(sPtr4);
    container.push_back(make_shared<Resource>(303, "SharedRes3"));

    cout << "📊 Reference count (sPtr1): " << sPtr1.use_count() << "\n";  // 4

    for (const auto& res : container)
    {
        res->display();
    }

    // 2.6 Temporary shared_ptr (scope)
    cout << "\n[2.6] Temporary shared_ptr in scope\n";
    {
        shared_ptr<Resource> temp = sPtr1;
        cout << "📊 Inside scope: ref count = " << sPtr1.use_count() << "\n";  // 5
    }
    cout << "📊 After scope: ref count = " << sPtr1.use_count() << "\n";       // 4

    // 2.7 Reset - decreases ref count
    cout << "\n[2.7] Reset - decreases reference count\n";
    sPtr2.reset();   // Removes sPtr2's ownership
    cout << "📊 After sPtr2.reset(): ref count = " << sPtr1.use_count() << "\n";  // 3

    // 2.8 Get raw pointer
    cout << "\n[2.8] Getting raw pointer using get()\n";
    Resource* rawShared = sPtr3.get();
    rawShared->doWork();

    // 2.9 Move semantics (transfers ownership without changing ref count)
    cout << "\n[2.9] Move semantics\n";
    shared_ptr<Resource> sPtr5 = make_shared<Resource>(304, "SharedRes4");
    shared_ptr<Resource> sPtr6 = move(sPtr5);   // sPtr5 becomes null
    if (sPtr5 == nullptr)
        cout << "⚠️  sPtr5 is now null (moved to sPtr6)\n";
    sPtr6->doWork();

    // All resources destroyed when function ends (when ref count reaches 0)
    cout << "\n--- sharedPtrDemo() ending - all shared_ptrs destroyed ---\n";
}

// ============================================================
// 3. std::weak_ptr - Non-Owning Observer (Breaks Cycles)
// ============================================================

void weakPtrDemo()
{
    cout << "\n========== WEAK_PTR DEMO ==========\n";

    // 3.1 Creating weak_ptr from shared_ptr
    cout << "\n[3.1] Creating weak_ptr from shared_ptr\n";
    auto sPtr = make_shared<Resource>(401, "WeakRes1");
    weak_ptr<Resource> wPtr = sPtr;   // Observes without owning

    cout << "📊 weak_ptr expired? " << (wPtr.expired() ? "YES" : "NO") << "\n";

    // 3.2 Using lock() to get temporary shared_ptr
    cout << "\n[3.2] Using lock() to get temporary shared_ptr\n";
    if (auto temp = wPtr.lock())
    {
        cout << "✅ Successfully locked weak_ptr: ";
        temp->display();
        cout << "📊 Reference count while locked: " << temp.use_count() << "\n";
    }
    else
    {
        cout << "❌ Object is gone!\n";
    }

    // 3.3 weak_ptr doesn't affect reference count
    cout << "\n[3.3] weak_ptr doesn't affect reference count\n";
    cout << "📊 Reference count (sPtr): " << sPtr.use_count() << "\n";  // Should be 1
    cout << "   (weak_ptr doesn't increase ref count)\n";

    // 3.4 Breaking cycles (the main use of weak_ptr)
    cout << "\n[3.4] Breaking cycles with weak_ptr\n";
    
    // Create a cycle example using Resource as a node
    struct Node
    {
        int id;
        shared_ptr<Node> next;      // Shared ownership (can cause cycle)
        weak_ptr<Node> weakNext;    // Weak observer (breaks cycle)
        
        Node(int i) : id(i)
        {
            cout << "🔧 Node " << id << " created\n";
        }
        ~Node()
        {
            cout << "💥 Node " << id << " destroyed\n";
        }
    };

    // BAD: Shared_ptr cycle (memory leak!)
    cout << "\n[3.4a] Shared_ptr cycle (BAD - memory leak)\n";
    auto node1 = make_shared<Node>(1);
    auto node2 = make_shared<Node>(2);
    node1->next = node2;
    node2->next = node1;   // ❌ Cycle! node1 and node2 never destroyed
    cout << "⚠️  Node1 and Node2 hold shared_ptr to each other - memory leak!\n";
    // node1 and node2 will NOT be destroyed when out of scope

    // GOOD: Weak_ptr breaks the cycle
    cout << "\n[3.4b] Weak_ptr breaks cycle (GOOD - no leak)\n";
    auto node3 = make_shared<Node>(3);
    auto node4 = make_shared<Node>(4);
    node3->weakNext = node4;   // node3 observes node4 weakly
    node4->weakNext = node3;   // node4 observes node3 weakly
    cout << "✅ Weak_ptr breaks the cycle - no memory leak!\n";
    // node3 and node4 will be destroyed when out of scope

    // 3.5 weak_ptr in containers (safe, doesn't keep objects alive)
    cout << "\n[3.5] weak_ptr in container\n";
    vector<weak_ptr<Resource>> observers;
    observers.push_back(wPtr);
    observers.push_back(sPtr);   // This makes a shared_ptr copy if not careful!

    // Correct way: use weak_ptr only
    vector<weak_ptr<Resource>> weakObservers;
    weakObservers.push_back(sPtr);
    weakObservers.push_back(make_shared<Resource>(402, "WeakRes2"));

    // 3.6 Cleanup expired weak_ptrs
    cout << "\n[3.6] Cleaning up expired weak_ptrs\n";
    sPtr.reset();   // Resource 401 is destroyed (ref count = 0)
    cout << "✅ sPtr reset - Resource 401 destroyed\n";

    for (auto& w : weakObservers)
    {
        if (auto sp = w.lock())
            cout << "✅ Still alive: " << sp->name << "\n";
        else
            cout << "❌ Expired (already destroyed)\n";
    }

    // 3.7 Use with factory pattern (no ownership transfer)
    cout << "\n[3.7] Factory pattern with weak_ptr\n";
    auto factoryShared = make_shared<Resource>(403, "FactoryRes");
    weak_ptr<Resource> factoryWeak = factoryShared;

    // Later, somewhere else:
    if (auto temp = factoryWeak.lock())
    {
        cout << "✅ Factory object still exists: ";
        temp->display();
    }

    // All resources destroyed when function ends
    cout << "\n--- weakPtrDemo() ending - all weak_ptrs destroyed ---\n";
}

// ============================================================
// MAIN FUNCTION - Runs all demos
// ============================================================

int main()
{
    cout << "\n============================================================\n";
    cout << "     SMART POINTERS COMPLETE DEMO (C++14)\n";
    cout << "============================================================\n";

    uniquePtrDemo();
    sharedPtrDemo();
    weakPtrDemo();

    cout << "\n============================================================\n";
    cout << "     ALL DEMOS COMPLETED SUCCESSFULLY\n";
    cout << "============================================================\n";

    return 0;
}
