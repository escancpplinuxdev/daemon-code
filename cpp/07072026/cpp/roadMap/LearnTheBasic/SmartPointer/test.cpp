#include <iostream>
#include <memory>   // All smart pointers: unique_ptr, shared_ptr, weak_ptr, make_unique, make_shared
#include <vector>
#include <string>
#include <functional> // for std::function (custom deleters)

using namespace std;

// ============================================================
// 1. Helper Class for Demonstration
// ============================================================
class Resource
{
private:
    string name;
    int id;

public:
    Resource(const string& n = "Default", int i = 0)
        : name(n), id(i)
    {
        cout << "🔵 Resource[" << name << ":" << id << "] Acquired (Constructor)\n";
    }

    ~Resource()
    {
        cout << "🔴 Resource[" << name << ":" << id << "] Destroyed (Destructor)\n";
    }

    void doWork()
    {
        cout << "⚙️  Resource[" << name << ":" << id << "] is Working...\n";
    }

    void setData(const string& n, int i)
    {
        name = n;
        id = i;
    }

    string getName() const { return name; }
    int getId() const { return id; }
};

// ============================================================
// 2. Unique Pointer Demonstrations
// ============================================================
void demoUniquePtr()
{
    cout << "\n========== UNIQUE POINTER ==========\n";

    // 2.1 Creating unique_ptr
    cout << "\n--- 2.1 Creating unique_ptr ---\n";

    // ✅ Preferred way: make_unique (C++14)
    auto uPtr1 = make_unique<Resource>("Unique1", 100);
    uPtr1->doWork();

    // Raw pointer way (not recommended)
    unique_ptr<Resource> uPtr2(new Resource("Unique2", 200));
    uPtr2->doWork();

    // 2.2 No copying allowed (unique_ptr is move-only)
    cout << "\n--- 2.2 Ownership Transfer (Move) ---\n";

    // unique_ptr<Resource> uPtr3 = uPtr1;  // ❌ Compile Error: copy constructor is deleted
    unique_ptr<Resource> uPtr3 = std::move(uPtr1);  // ✅ Move transfers ownership

    if (uPtr1 == nullptr)
        cout << "uPtr1 is now NULL (ownership moved to uPtr3)\n";

    uPtr3->doWork();

    // 2.3 Passing to function (by value with move)
    cout << "\n--- 2.3 Passing to Function ---\n";

    auto processResource = [](unique_ptr<Resource> res) {
        cout << "  Processing inside function...\n";
        res->doWork();
        // Resource deleted here when 'res' goes out of scope
    };

    auto uPtr4 = make_unique<Resource>("Unique4", 400);
    processResource(std::move(uPtr4));  // ✅ Move ownership to function
    // uPtr4 is now null

    // 2.4 Custom Deleter
    cout << "\n--- 2.4 Custom Deleter ---\n";

    auto customDeleter = [](Resource* res) {
        cout << "  🧹 Custom Deleter: Cleaning up Resource[" << res->getName() << "]\n";
        delete res;
    };

    unique_ptr<Resource, decltype(customDeleter)> uPtr5(
        new Resource("Unique5", 500),
        customDeleter
    );
    uPtr5->doWork();

    // 2.5 Getting raw pointer
    cout << "\n--- 2.5 Getting Raw Pointer ---\n";

    auto uPtr6 = make_unique<Resource>("Unique6", 600);
    Resource* raw = uPtr6.get();  // Get raw pointer (no ownership transfer)
    raw->doWork();

    Resource* released = uPtr6.release();  // Release ownership (uPtr6 becomes null)
    cout << "Released pointer: " << released->getName() << "\n";
    delete released;  // Must manually delete after release
}

// ============================================================
// 3. Shared Pointer Demonstrations
// ============================================================
void demoSharedPtr()
{
    cout << "\n========== SHARED POINTER ==========\n";

    // 3.1 Creating shared_ptr
    cout << "\n--- 3.1 Creating shared_ptr ---\n";

    // ✅ Preferred way: make_shared (single allocation for object + control block)
    auto sPtr1 = make_shared<Resource>("Shared1", 1000);
    cout << "Reference count: " << sPtr1.use_count() << "\n";
    sPtr1->doWork();

    // Raw pointer way (two allocations)
    shared_ptr<Resource> sPtr2(new Resource("Shared2", 2000));
    cout << "Reference count: " << sPtr2.use_count() << "\n";

    // 3.2 Shared ownership
    cout << "\n--- 3.2 Shared Ownership ---\n";

    auto sPtr3 = make_shared<Resource>("Shared3", 3000);
    cout << "sPtr3 ref count: " << sPtr3.use_count() << "\n";

    shared_ptr<Resource> sPtr4 = sPtr3;  // ✅ Copy allowed
    cout << "After copy - ref count: " << sPtr3.use_count() << "\n";

    shared_ptr<Resource> sPtr5 = sPtr3;  // Another copy
    cout << "After second copy - ref count: " << sPtr3.use_count() << "\n";

    sPtr3->doWork();
    sPtr4->doWork();
    sPtr5->doWork();

    cout << "\nResetting sPtr5...\n";
    sPtr5.reset();  // Decreases reference count
    cout << "Ref count after reset: " << sPtr3.use_count() << "\n";

    // 3.3 Passing to function (by value or by reference)
    cout << "\n--- 3.3 Passing Shared to Function ---\n";

    auto processShared = [](shared_ptr<Resource> res) {
        cout << "  Processing in function (ref count: " << res.use_count() << ")\n";
        res->doWork();
    };

    auto sPtr6 = make_shared<Resource>("Shared6", 6000);
    processShared(sPtr6);  // ✅ Copies shared_ptr (increases ref count)
    cout << "After function, ref count: " << sPtr6.use_count() << "\n";

    // 3.4 Custom Deleter with shared_ptr
    cout << "\n--- 3.4 Custom Deleter with shared_ptr ---\n";

    auto sharedDeleter = [](Resource* res) {
        cout << "  🧹 Custom Shared Deleter: Deleting " << res->getName() << "\n";
        delete res;
    };

    shared_ptr<Resource> sPtr7(
        new Resource("Shared7", 7000),
        sharedDeleter
    );
    sPtr7->doWork();

    // 3.5 Aliasing Constructor
    cout << "\n--- 3.5 Aliasing Constructor ---\n";

    struct BigData
    {
        Resource res;
        int extra[100];
        BigData() : res("Aliased", 999) {}
    };

    auto big = make_shared<BigData>();
    // Aliasing: share ownership of 'big' but point to 'big->res'
    shared_ptr<Resource> aliased(big, &big->res);
    cout << "Aliased resource name: " << aliased->getName() << "\n";
    cout << "Aliased ref count: " << aliased.use_count() << "\n";
    // When aliased goes out of scope, the BigData object is destroyed
}

// ============================================================
// 4. Weak Pointer Demonstrations
// ============================================================
void demoWeakPtr()
{
    cout << "\n========== WEAK POINTER ==========\n";

    // 4.1 Creating weak_ptr from shared_ptr
    cout << "\n--- 4.1 Creating weak_ptr ---\n";

    auto sPtr = make_shared<Resource>("WeakRes", 888);
    weak_ptr<Resource> wPtr = sPtr;  // ✅ weak_ptr from shared_ptr
    cout << "weak_ptr created, ref count: " << sPtr.use_count() << "\n";

    // 4.2 Accessing through weak_ptr (must lock)
    cout << "\n--- 4.2 Accessing with lock() ---\n";

    if (auto locked = wPtr.lock())  // ✅ Get a temporary shared_ptr
    {
        cout << "Resource is alive: " << locked->getName() << "\n";
        locked->doWork();
    }
    else
    {
        cout << "Resource has been destroyed\n";
    }

    // 4.3 Expired check
    cout << "\n--- 4.3 Expired Check ---\n";

    cout << "wPtr.expired(): " << (wPtr.expired() ? "true" : "false") << "\n";

    cout << "Resetting shared_ptr...\n";
    sPtr.reset();  // Destroy the resource

    cout << "After reset, wPtr.expired(): " << (wPtr.expired() ? "true" : "false") << "\n";

    if (auto locked = wPtr.lock())
    {
        cout << "Locked resource (shouldn't happen)\n";
    }
    else
    {
        cout << "❌ Resource is gone! lock() returned nullptr.\n";
    }

    // 4.4 Breaking Circular References
    cout << "\n--- 4.4 Breaking Circular References ---\n";

    struct Node
    {
        string name;
        shared_ptr<Node> next;   // Strong reference
        weak_ptr<Node> weakPrev; // ✅ Weak reference to break cycle

        Node(const string& n) : name(n)
        {
            cout << "🔵 Node[" << name << "] created\n";
        }

        ~Node()
        {
            cout << "🔴 Node[" << name << "] destroyed\n";
        }
    };

    // Create a cycle (bad) and break it with weak_ptr
    auto node1 = make_shared<Node>("First");
    auto node2 = make_shared<Node>("Second");

    node1->next = node2;          // node1 → node2 (strong)
    node2->weakPrev = node1;      // node2 → node1 (weak) ✅ no cycle

    cout << "Node1 ref count: " << node1.use_count() << "\n";
    cout << "Node2 ref count: " << node2.use_count() << "\n";

    cout << "Resetting node1...\n";
    node1.reset();  // node1 destroyed, node2 still has ref count 1

    if (auto prev = node2->weakPrev.lock())
    {
        cout << "Previous node still alive: " << prev->name << "\n";
    }
    else
    {
        cout << "Previous node is gone (correct, because weak_ptr didn't keep it alive)\n";
    }
}

// ============================================================
// 5. Mixed Usage: Vector of Smart Pointers
// ============================================================
void demoVectorOfSmartPointers()
{
    cout << "\n========== VECTOR OF SMART POINTERS ==========\n";

    vector<unique_ptr<Resource>> resources;

    for (int i = 0; i < 5; ++i)
    {
        string name = "VecRes" + to_string(i);
        resources.push_back(make_unique<Resource>(name, i * 100));
    }

    cout << "\nProcessing resources...\n";
    for (auto& res : resources)
    {
        res->doWork();
    }

    // Transfer ownership out of vector
    cout << "\nTransferring ownership out...\n";
    auto moved = std::move(resources[2]);
    moved->doWork();

    cout << "Vector size after move: " << resources.size() << "\n";
    // The moved slot is now empty
}

// ============================================================
// 6. enable_shared_from_this
// ============================================================
class SharableResource : public enable_shared_from_this<SharableResource>
{
private:
    string name;

public:
    SharableResource(const string& n) : name(n)
    {
        cout << "🔵 SharableResource[" << name << "] created\n";
    }

    ~SharableResource()
    {
        cout << "🔴 SharableResource[" << name << "] destroyed\n";
    }

    shared_ptr<SharableResource> getShared()
    {
        return shared_from_this();  // ✅ Safely returns a shared_ptr to this
    }

    void doWork()
    {
        cout << "⚙️  SharableResource[" << name << "] working\n";
    }
};

void demoEnableSharedFromThis()
{
    cout << "\n========== ENABLE_SHARED_FROM_THIS ==========\n";

    auto sPtr = make_shared<SharableResource>("EnableMe");
    auto sPtr2 = sPtr->getShared();  // ✅ Returns another shared_ptr

    cout << "Ref count: " << sPtr.use_count() << "\n";
    sPtr2->doWork();

    // ❌ WRONG: Calling shared_from_this before object is owned by shared_ptr
    // SharableResource bad("Bad");
    // auto badPtr = bad.getShared();  // Throws bad_weak_ptr
}

// ============================================================
// 7. Comparison and Summary
// ============================================================
void demoComparisonTable()
{
    cout << "\n========== COMPARISON SUMMARY ==========\n";

    cout << R"(
    ┌─────────────────┬──────────────┬───────────────┬──────────────┐
    │ Feature         │ unique_ptr   │ shared_ptr    │ weak_ptr     │
    ├─────────────────┼──────────────┼───────────────┼──────────────┤
    │ Ownership       │ Exclusive    │ Shared        │ Non-owning   │
    │ Copyable        │ ❌ No        │ ✅ Yes        │ ✅ Yes       │
    │ Movable         │ ✅ Yes       │ ✅ Yes        │ ✅ Yes       │
    │ Reference Count │ 1            │ >=1           │ 0 (observer) │
    │ Overhead        │ Low (0)      │ High (atomic) │ Low          │
    │ Use for         │ Unique owner │ Shared owner  │ Break cycles │
    │ Preferred way   │ make_unique  │ make_shared   │ weak_ptr<T>  │
    └─────────────────┴──────────────┴───────────────┴──────────────┘
    )";
}

// ============================================================
// 8. Main Function - Run All Demos
// ============================================================
int main()
{
    cout << "\n🚀 SMART POINTERS COMPLETE DEMONSTRATION\n";
    cout << "===========================================\n";

    demoUniquePtr();
    demoSharedPtr();
    demoWeakPtr();
    demoVectorOfSmartPointers();
    demoEnableSharedFromThis();
    demoComparisonTable();

    cout << "\n✅ All demonstrations completed successfully!\n";
    return 0;
}
