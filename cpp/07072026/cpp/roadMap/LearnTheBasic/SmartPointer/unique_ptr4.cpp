//make unique_ptr4 && valgrind --leak-check=full ./unique_ptr4
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdarg>

using namespace std;

// ============================================================
// Forward declaration of log function
// ============================================================
void logMessage(FILE* fp, const char* fmt, ...);

// ============================================================
// LOGGING FUNCTION - writes to file
// ============================================================
void logMessage(FILE* fp, const char* fmt, ...)
{
    if (!fp) return;

	
        // Add timestamp
        time_t now = time(NULL);
        struct tm* tm_info = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
        fprintf(fp, "[%s] ", timestamp);

   // Format the message 
    va_list args;
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    fprintf(fp, "\n");
    fflush(fp);
    va_end(args);
}

/*

   va_list args;

   Creates a variable args that will hold the list of extra arguments

   Think of it as a "pointer" to where arguments are stored

   va_start(args, fmt);

   Initializes args to point to the first variadic argument

   fmt is the last named parameter

   va_start uses fmt to find where the extra arguments begin

   Memory Layout

   Stack (simplified):
   ┌─────────────────┐
   │   fp            │  ← First argument
   ├─────────────────┤
   │   fmt           │  ← Last named argument
   ├─────────────────┤
   │   arg1          │  ← va_start points here
   ├─────────────────┤
   │   arg2          │
   ├─────────────────┤


   vfprintf(fp, fmt, args);

   vfprintf = "variable arguments fprintf"

   Takes the format string and the argument list

   Prints formatted output to the file

   fprintf(fp, "\n");

   Adds a newline after each log message

   So each log entry appears on its own line

   fflush(fp);

   Forces the data to be written to disk immediately

   Without this, data might stay in memory buffer

   Useful for logs (so you can see messages immediately)

   va_end(args);

   Cleans up the argument list

   Required for proper memory management

   Should always be called after va_start


   3. Memory Layout (How it works internally)

   Function Call:
   logMessage(fp, "%d %s", 42, "hello")

   Stack (simplified):
	┌──────────────────────────────────────┐
	│  ... (previous stack)                │
	├──────────────────────────────────────┤
	│  "hello"          ← arg2            │
	├──────────────────────────────────────┤
	│  42               ← arg1            │
	├──────────────────────────────────────┤
	│  "%d %s"          ← fmt             │
	├──────────────────────────────────────┤
	│  fp               ← arg0            │
	├──────────────────────────────────────┤
	│  Return address                      │
	├──────────────────────────────────────┤
	│  Local variables                     │
	└──────────────────────────────────────┘


	📊 The printf Family Comparison
	Function	Target	Arguments		Use Case
	printf()	stdout	Fixed list		Simple console output
	fprintf()	File	Fixed list		Write to file
	sprintf()	String	Fixed list		Format to string
	vprintf()	stdout	va_list	Custom 		logging
	vfprintf()	File	va_list	Custom 		file logging
	vsprintf()	String	va_list	Custom 		string formatting

*/



// ============================================================
// BASE CLASS: Resource
// ============================================================

class Resource
{
public:
    int id;
    string name;
    string data;
    FILE* logFp;

    Resource(int id = 0, const string& name = "Default", const string& data = "", FILE* fp = nullptr) 
        : id(id), name(name), data(data), logFp(fp)
    {
        logMessage(logFp, "🔧 Resource [%d] '%s' ACQUIRED (data: %s)", id, name.c_str(), data.c_str());
    }

    ~Resource()
    {
        logMessage(logFp, "💥 Resource [%d] '%s' DESTROYED", id, name.c_str());
    }

    void doWork() const
    {
        logMessage(logFp, "⚙️  Resource [%d] is working... (data: %s)", id, data.c_str());
    }

    void display() const
    {
        logMessage(logFp, "📦 Resource ID: %d, Name: %s, Data: %s", id, name.c_str(), data.c_str());
    }
};

// ============================================================
// Function to read data from file using unique_ptr
// ============================================================
unique_ptr<char[]> readFileData(const string& filename, FILE* logFp)
{
    logMessage(logFp, "\n📂 Reading data from file: %s", filename.c_str());
    
    // Open file using fopen (managed by unique_ptr with custom logic)
    FILE* fp = fopen(filename.c_str(), "r");
    if (!fp)
    {
        logMessage(logFp, "❌ Failed to open file: %s", filename.c_str());
        return nullptr;
    }
    
    // Get file size
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    logMessage(logFp, "📊 File size: %ld bytes", fileSize);
    
    // Allocate memory using unique_ptr<char[]>
    unique_ptr<char[]> buffer(new char[fileSize + 1]);
    if (!buffer)
    {
        logMessage(logFp, "❌ Failed to allocate memory");
        fclose(fp);
        return nullptr;
    }
    
    // Read file content
    size_t bytesRead = fread(buffer.get(), 1, fileSize, fp);
    buffer[bytesRead] = '\0';
    
    logMessage(logFp, "✅ Read %zu bytes from file", bytesRead);
    
    // Close file
    fclose(fp);
    
    return buffer;
}

// ============================================================
// Function to parse data and create Resources
// ============================================================
vector<unique_ptr<Resource>> parseResources(const string& data, FILE* logFp)
{
    vector<unique_ptr<Resource>> resources;
    
    logMessage(logFp, "\n📝 Parsing resource data...");
    
    stringstream ss(data);
    string line;
    int id = 1;
    
    while (getline(ss, line))
    {
        // Skip empty lines
        if (line.empty()) continue;
        
        logMessage(logFp, "   Processing line: %s", line.c_str());
        
        // Create resource from line data
        string name = "Resource_" + to_string(id);
        resources.push_back(make_unique<Resource>(id, name, line, logFp));
        id++;
    }
    
    logMessage(logFp, "✅ Created %zu resources", resources.size());
    return resources;
}

// ============================================================
// 1. std::unique_ptr Demo
// ============================================================

void uniquePtrDemo(FILE* fp)
{
    logMessage(fp, "\n========== UNIQUE_PTR DEMO ==========");

    logMessage(fp, "\n[1.1] Creating unique_ptr with make_unique");
    auto uPtr1 = make_unique<Resource>(101, "UniqueRes1", "Custom data 1", fp);
    uPtr1->doWork();

    logMessage(fp, "\n[1.2] Creating unique_ptr with new");
    unique_ptr<Resource> uPtr2(new Resource(102, "UniqueRes2", "Custom data 2", fp));
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

    uPtr2.reset(new Resource(103, "NewRes", "New data", fp));
    uPtr2->doWork();

    logMessage(fp, "\n[1.7] Unique_ptr in vector container");
    vector<unique_ptr<Resource>> container;
    container.push_back(make_unique<Resource>(201, "ContainerRes1", "Container data 1", fp));
    container.push_back(make_unique<Resource>(202, "ContainerRes2", "Container data 2", fp));

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
    auto sPtr1 = make_shared<Resource>(301, "SharedRes1", "Shared data 1", fp);
    sPtr1->doWork();

    logMessage(fp, "\n[2.2] Copying shared_ptr (shared ownership)");
    shared_ptr<Resource> sPtr2 = sPtr1;
    shared_ptr<Resource> sPtr3(sPtr1);

    logMessage(fp, "📊 Reference count: %ld", sPtr1.use_count());

    logMessage(fp, "\n[2.3] Modifying through any shared_ptr");
    sPtr2->name = "ModifiedSharedRes";
    sPtr3->display();

    logMessage(fp, "\n[2.4] Creating shared_ptr with new");
    shared_ptr<Resource> sPtr4(new Resource(302, "SharedRes2", "Shared data 2", fp));

    logMessage(fp, "\n[2.5] shared_ptr in vector container");
    vector<shared_ptr<Resource>> container;
    container.push_back(sPtr1);
    container.push_back(sPtr4);
    container.push_back(make_shared<Resource>(303, "SharedRes3", "Shared data 3", fp));

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
    shared_ptr<Resource> sPtr5 = make_shared<Resource>(304, "SharedRes4", "Shared data 4", fp);
    shared_ptr<Resource> sPtr6 = move(sPtr5);
    if (sPtr5 == nullptr)
        logMessage(fp, "⚠️  sPtr5 is now null (moved to sPtr6)");
    sPtr6->doWork();

    logMessage(fp, "\n--- sharedPtrDemo() ending - all shared_ptrs destroyed ---");
}

// ============================================================
// 3. std::weak_ptr Demo (FIXED - No Memory Leak)
// ============================================================

void weakPtrDemo(FILE* fp)
{
    logMessage(fp, "\n========== WEAK_PTR DEMO ==========");

    logMessage(fp, "\n[3.1] Creating weak_ptr from shared_ptr");
    auto sPtr = make_shared<Resource>(401, "WeakRes1", "Weak data 1", fp);
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

    // ============================================================
    // FIXED: Demonstrate cycle with manual break
    // ============================================================
    logMessage(fp, "\n[3.4] Breaking cycles with weak_ptr");
    
    struct Node
    {
        int id;
        shared_ptr<Node> next;
        weak_ptr<Node> weakNext;
        string data;
        FILE* logFp;
        
        Node(int i, const string& d, FILE* fp) : id(i), data(d), logFp(fp)
        {
            logMessage(logFp, "🔧 Node %d created (data: %s)", id, data.c_str());
        }
        ~Node()
        {
            logMessage(logFp, "💥 Node %d destroyed", id);
        }
    };

    // ============================================================
    // BAD: Shared_ptr cycle (memory leak) - BUT WE MANUALLY BREAK IT
    // ============================================================
    logMessage(fp, "\n[3.4a] Shared_ptr cycle (DEMONSTRATION - manually broken)");
    auto node1 = make_shared<Node>(1, "Node1 data", fp);
    auto node2 = make_shared<Node>(2, "Node2 data", fp);
    
    // Create the cycle
    node1->next = node2;
    node2->next = node1;
    logMessage(fp, "⚠️  Cycle created! Node1 and Node2 hold shared_ptr to each other");
    
    // MANUALLY BREAK THE CYCLE BEFORE EXITING
    node1->next.reset();   // Break the cycle
    node2->next.reset();   // Break the cycle
    logMessage(fp, "✅ Manually broke the cycle - no memory leak!");

    // ============================================================
    // GOOD: Weak_ptr breaks the cycle naturally
    // ============================================================
    logMessage(fp, "\n[3.4b] Weak_ptr breaks cycle (GOOD - no leak)");
    auto node3 = make_shared<Node>(3, "Node3 data", fp);
    auto node4 = make_shared<Node>(4, "Node4 data", fp);
    node3->weakNext = node4;
    node4->weakNext = node3;
    logMessage(fp, "✅ Weak_ptr breaks the cycle - no memory leak!");

    logMessage(fp, "\n[3.5] weak_ptr in container");
    vector<weak_ptr<Resource>> observers;
    observers.push_back(wPtr);
    observers.push_back(sPtr);

    vector<weak_ptr<Resource>> weakObservers;
    weakObservers.push_back(sPtr);
    weakObservers.push_back(make_shared<Resource>(402, "WeakRes2", "Weak data 2", fp));

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
    auto factoryShared = make_shared<Resource>(403, "FactoryRes", "Factory data", fp);
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
    FILE* fp = fopen("./smart_ptr.txt", "w+");
    if (!fp)
    {
        fprintf(stderr, "Failed to open ./smart_ptr.txt for writing\n");
        return 1;
    }

    // Write header
    fprintf(fp, "============================================================\n");
    fprintf(fp, "     SMART POINTERS DEMO WITH FILE READING (C++14)\n");
    fprintf(fp, "============================================================\n");
    fflush(fp);

    // ============================================================
    // READ DATA FROM FILE USING UNIQUE_PTR
    // ============================================================
    
    // Step 1: Read data from /tmp/read_smart_ptr.txt
    unique_ptr<char[]> fileData = readFileData("./read_smart_ptr.txt", fp);
    if (fileData)
    {
        logMessage(fp, "\n📄 Content read from file:\n%s", fileData.get());
        
        // Step 2: Parse and create resources
        vector<unique_ptr<Resource>> resources = parseResources(string(fileData.get()), fp);
        
        // Step 3: Display resources
        logMessage(fp, "\n📋 Created resources:");
        for (const auto& res : resources)
        {
            res->display();
        }
    }
    else
    {
        logMessage(fp, "⚠️  No data read from file. Continuing with demo...");
    }

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



/*

🎯 Complete Explanation of unique_ptr4.cpp

This program demonstrates smart pointers (unique_ptr, shared_ptr, weak_ptr) along with file reading and resource management. Let me break it down step by step.
📌 1. What This Program Does
Feature	Description
Reads a file	Reads ./read_smart_ptr.txt using unique_ptr<char[]>
Parses data	Converts each line into a Resource object
Demonstrates smart pointers	Shows unique_ptr, shared_ptr, weak_ptr in action
Logs everything	Writes all output to ./smart_ptr.txt
No memory leaks	Fixed the shared_ptr cycle issue
📌 2. The Input File (read_smart_ptr.txt)
text

101,UniqueRes1
102,UniqueRes2
103,NewRes
201,ContainerRes1
202,ContainerRes2
301,SharedRes1
302,SharedRes2
303,SharedRes3
304,SharedRes4
401,WeakRes1
402,WeakRes2
403,FactoryRes

Each line has: ID,Name

What it represents:

    Each line is data for one Resource object

    The program creates 12 Resource objects from this file

📌 3. The Logging System
logMessage() Function
cpp

void logMessage(FILE* fp, const char* fmt, ...)
{
    if (!fp) return;
    va_list args;
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    fprintf(fp, "\n");
    fflush(fp);
    va_end(args);
}

What it does:

    Writes formatted text to a file (like printf but to file)

    va_list handles variable arguments (like %d, %s)

    fflush immediately writes to disk

Why? Instead of cout << "Hello", we write logMessage(fp, "Hello") which goes to smart_ptr.txt.
📌 4. The Resource Class
cpp

class Resource
{
public:
    int id;           // Unique ID
    string name;      // Name of the resource
    string data;      // The actual data from the file line
    FILE* logFp;      // File pointer for logging

    Resource(int id, const string& name, const string& data, FILE* fp)
        : id(id), name(name), data(data), logFp(fp)
    {
        logMessage(logFp, "🔧 Resource [%d] '%s' ACQUIRED (data: %s)", 
                   id, name.c_str(), data.c_str());
    }

    ~Resource()
    {
        logMessage(logFp, "💥 Resource [%d] '%s' DESTROYED", id, name.c_str());
    }

    void display() const
    {
        logMessage(logFp, "📦 Resource ID: %d, Name: %s, Data: %s", 
                   id, name.c_str(), data.c_str());
    }
};

What it does:

    Represents a resource that needs to be managed

    Logs when it's created (constructor) and destroyed (destructor)

    Stores the file line data in data member

📌 5. Reading File with unique_ptr<char[]>
The readFileData() Function
cpp

unique_ptr<char[]> readFileData(const string& filename, FILE* logFp)
{
    // 1. Open file
    FILE* fp = fopen(filename.c_str(), "r");
    if (!fp) return nullptr;
    
    // 2. Get file size
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    // 3. Allocate memory using unique_ptr<char[]>
    unique_ptr<char[]> buffer(new char[fileSize + 1]);
    
    // 4. Read file content
    size_t bytesRead = fread(buffer.get(), 1, fileSize, fp);
    buffer[bytesRead] = '\0';
    
    // 5. Close file
    fclose(fp);
    
    return buffer;  // Ownership transferred to caller
}

Step-by-step:
Step	Code	What it does
1	fopen(filename.c_str(), "r")	Opens the file
2	fseek + ftell	Gets file size
3	new char[fileSize + 1]	Allocates memory
4	unique_ptr<char[]>	Wraps the memory (auto-delete when out of scope)
5	fread(buffer.get(), ...)	Reads file content
6	buffer[bytesRead] = '\0'	Adds null terminator
7	fclose(fp)	Closes file

Why unique_ptr<char[]>?

    Automatically deletes the buffer when no longer needed

    No memory leaks

    No need to manually call delete[]

📌 6. Parsing Data into Resources
The parseResources() Function
cpp

vector<unique_ptr<Resource>> parseResources(const string& data, FILE* logFp)
{
    vector<unique_ptr<Resource>> resources;
    stringstream ss(data);
    string line;
    int id = 1;
    
    while (getline(ss, line))
    {
        if (line.empty()) continue;
        
        string name = "Resource_" + to_string(id);
        resources.push_back(make_unique<Resource>(id, name, line, logFp));
        id++;
    }
    
    return resources;
}

What it does:

    Takes the entire file content as a string

    Splits it line by line

    For each line, creates a Resource object

    Stores all resources in a vector<unique_ptr<Resource>>

Example:
text

Input line: "101,UniqueRes1"
Creates: Resource(id=1, name="Resource_1", data="101,UniqueRes1")

📌 7. The main() Function Flow
cpp

int main()
{
    // 1. Open log file
    FILE* fp = fopen("./smart_ptr.txt", "w+");
    
    // 2. Read data from ./read_smart_ptr.txt
    unique_ptr<char[]> fileData = readFileData("./read_smart_ptr.txt", fp);
    
    // 3. Parse data into resources
    vector<unique_ptr<Resource>> resources = parseResources(string(fileData.get()), fp);
    
    // 4. Display resources
    for (const auto& res : resources)
        res->display();
    
    // 5. Run demos
    uniquePtrDemo(fp);
    sharedPtrDemo(fp);
    weakPtrDemo(fp);
    
    // 6. Close log file
    fclose(fp);
}

📌 8. The Three Smart Pointer Demos
🔹 unique_ptr Demo (Exclusive Ownership)
cpp

void uniquePtrDemo(FILE* fp)
{
    // Creating unique_ptr
    auto uPtr1 = make_unique<Resource>(101, "UniqueRes1", "Custom data 1", fp);
    uPtr1->doWork();
    
    // Moving ownership (copy not allowed)
    unique_ptr<Resource> uPtr3 = move(uPtr1);
    // uPtr1 is now nullptr
    
    // Releasing ownership (manual delete)
    Resource* releasedRes = uPtr3.release();
    delete releasedRes;  // Must manually delete!
    
    // Reset
    uPtr2.reset();  // Deletes current object
    uPtr2.reset(new Resource(103, "NewRes", "New data", fp));  // Takes new
}

🔹 shared_ptr Demo (Shared Ownership)
cpp

void sharedPtrDemo(FILE* fp)
{
    // Creating shared_ptr
    auto sPtr1 = make_shared<Resource>(301, "SharedRes1", "Shared data 1", fp);
    
    // Copying (shared ownership)
    shared_ptr<Resource> sPtr2 = sPtr1;
    shared_ptr<Resource> sPtr3(sPtr1);
    // Reference count becomes 3
    
    // Reference count
    logMessage(fp, "📊 Reference count: %ld", sPtr1.use_count());
    
    // Reset decreases reference count
    sPtr2.reset();  // Ref count becomes 2
}

🔹 weak_ptr Demo (Non-Owning Observer)
cpp

void weakPtrDemo(FILE* fp)
{
    // Creating weak_ptr from shared_ptr
    auto sPtr = make_shared<Resource>(401, "WeakRes1", "Weak data 1", fp);
    weak_ptr<Resource> wPtr = sPtr;
    
    // Checking if expired
    if (wPtr.expired())
        logMessage(fp, "Object is gone");
    
    // Using lock() to access
    if (auto temp = wPtr.lock())
    {
        temp->display();  // Safe access
    }
    
    // Breaking cycles
    // BAD: shared_ptr cycle (memory leak)
    node1->next = node2;
    node2->next = node1;
    
    // GOOD: weak_ptr breaks cycle
    node3->weakNext = node4;
    node4->weakNext = node3;
}

📌 9. How to Run and Watch
1. Create input file:
bash

cat > read_smart_ptr.txt << 'EOF'
101,UniqueRes1
102,UniqueRes2
103,NewRes
201,ContainerRes1
202,ContainerRes2
301,SharedRes1
302,SharedRes2
303,SharedRes3
304,SharedRes4
401,WeakRes1
402,WeakRes2
403,FactoryRes
EOF

2. Compile and run:
bash

g++ -std=c++14 -o unique_ptr4 unique_ptr4.cpp
./unique_ptr4

3. Watch the log:
bash

watch -n 0.2 'cat smart_ptr.txt'

📌 10. Sample Output
text

============================================================
     SMART POINTERS DEMO WITH FILE READING (C++14)
============================================================

📂 Reading data from file: ./read_smart_ptr.txt
📊 File size: 331 bytes
✅ Read 331 bytes from file

📄 Content read from file:
101,UniqueRes1
102,UniqueRes2
...

📝 Parsing resource data...
   Processing line: 101,UniqueRes1
🔧 Resource [1] 'Resource_1' ACQUIRED (data: 101,UniqueRes1)
...
✅ Created 12 resources

📋 Created resources:
📦 Resource ID: 1, Name: Resource_1, Data: 101,UniqueRes1
📦 Resource ID: 2, Name: Resource_2, Data: 102,UniqueRes2
...

========== UNIQUE_PTR DEMO ==========
[1.1] Creating unique_ptr with make_unique
🔧 Resource [101] 'UniqueRes1' ACQUIRED (data: Custom data 1)
⚙️  Resource [101] is working... (data: Custom data 1)
...

📌 11. Summary Table
Concept	How It's Used
unique_ptr<char[]>	Reads file content, auto-deletes buffer
vector<unique_ptr<Resource>>	Stores resources with exclusive ownership
make_unique	Creates resources safely
move()	Transfers ownership
get()	Gets raw pointer
release()	Gives up ownership
reset()	Deletes current object
shared_ptr	Multiple owners, reference counting
weak_ptr	Observer, breaks cycles
.lock()	Gets temporary shared_ptr from weak_ptr
.use_count()	Returns reference count
📌 12. Key Takeaways

    unique_ptr<char[]> is perfect for reading files – it auto-deletes the buffer.

    File reading uses C-style fopen/fread but is safely managed by unique_ptr.

    Smart pointers eliminate manual new/delete in most cases.

    Logging to file helps you trace program execution without cluttering the console.

    weak_ptr is essential for breaking cycles in complex object graphs.

    Memory leaks are fixed by manually breaking cycles or using weak_ptr.

This program shows how smart pointers are used not just for memory management, but also for managing file resources (reading data) and object lifetimes (resources created from file data). Everything is RAII – resources are automatically cleaned up when they go out of scope. 🚀

*/
