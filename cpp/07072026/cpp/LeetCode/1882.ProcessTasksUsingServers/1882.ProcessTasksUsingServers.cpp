

/*
   1882. Process Tasks Using Servers
   Medium
   Topics
   premium lock iconCompanies
   Hint

   You are given two 0-indexed integer arrays servers and tasks of lengths n​​​​​​ and m​​​​​​ respectively. servers[i] is the weight of the i​​​​​​th​​​​ server, and tasks[j] is the time needed to process the j​​​​​​th​​​​ task in seconds.

   Tasks are assigned to the servers using a task queue. Initially, all servers are free, and the queue is empty.

   At second j, the jth task is inserted into the queue (starting with the 0th task being inserted at second 0). As long as there are free servers and the queue is not empty, the task in the front of the queue will be assigned to a free server with the smallest weight, and in case of a tie, it is assigned to a free server with the smallest index.

   If there are no free servers and the queue is not empty, we wait until a server becomes free and immediately assign the next task. If multiple servers become free at the same time, then multiple tasks from the queue will be assigned in order of insertion following the weight and index priorities above.

   A server that is assigned task j at second t will be free again at second t + tasks[j].

   Build an array ans​​​​ of length m, where ans[j] is the index of the server the j​​​​​​th task will be assigned to.

   Return the array ans​​​​.



   Example 1:

Input: servers = [3,3,2], tasks = [1,2,3,2,1,2]
Output: [2,2,0,2,1,2]
Explanation: Events in chronological order go as follows:
- At second 0, task 0 is added and processed using server 2 until second 1.
- At second 1, server 2 becomes free. Task 1 is added and processed using server 2 until second 3.
- At second 2, task 2 is added and processed using server 0 until second 5.
- At second 3, server 2 becomes free. Task 3 is added and processed using server 2 until second 5.
- At second 4, task 4 is added and processed using server 1 until second 5.
- At second 5, all servers become free. Task 5 is added and processed using server 2 until second 7.

Example 2:

Input: servers = [5,1,4,3,2], tasks = [2,1,2,4,5,2,1]
Output: [1,4,1,4,1,3,2]
Explanation: Events in chronological order go as follows: 
- At second 0, task 0 is added and processed using server 1 until second 2.
- At second 1, task 1 is added and processed using server 4 until second 2.
- At second 2, servers 1 and 4 become free. Task 2 is added and processed using server 1 until second 4. 
- At second 3, task 3 is added and processed using server 4 until second 7.
- At second 4, server 1 becomes free. Task 4 is added and processed using server 1 until second 9. 
- At second 5, task 5 is added and processed using server 3 until second 7.
- At second 6, task 6 is added and processed using server 2 until second 7.

 */

/*
   1882. Process Tasks Using Servers
   Explanation

   We have n servers, each with a weight servers[i]. We have m tasks arriving at times 0, 1, 2, .... At second j, task j arrives. We assign tasks to servers based on these rules:

   Priority for free servers: smallest weight, then smallest index.

   If no servers are free, wait until one becomes free (the one that becomes free earliest; ties broken by weight, then index).

   Multiple tasks may be assigned at the same second if multiple servers become free simultaneously.

   Key Data Structures

   Free servers min-heap: stores (weight, index) for currently free servers, ordered by weight then index.

   Busy servers min-heap: stores (free_time, weight, index) for busy servers, ordered by free_time, then weight, then index.

   Algorithm

   We iterate through tasks in order j = 0, 1, ..., m-1.

   At each second (which is j), we:

   Move any busy servers whose free_time <= current_time to the free heap.

   If the free heap is empty, we must wait until the earliest busy server becomes free. So set current_time = busy.top().free_time, and move all servers with that free_time to the free heap.

   Assign the current task to the top of the free heap: pop (weight, idx), assign it, push (current_time + tasks[j], weight, idx) into the busy heap.

   Record ans[j] = idx.

   Important Detail — Time Handling

   The "current time" is tricky because tasks arrive at j = 0, 1, 2, ..., but assignments may happen at later times if all servers are busy. We track current_time = max(j, current_time).

   Wait, let me re-read the problem: "At second j, the jth task is inserted into the queue". So task j is available at time j, but it might be assigned later if no server is free.

   The algorithm processes tasks in order j. At step j:

   The current time is at least j (the task can't be assigned before it arrives).

   We may need to advance time further if all servers are busy.

   Let me formalize:
   text

   current_time = 0
   for j = 0 to m-1:
# Advance time to at least j (task arrival)
if current_time < j: current_time = j

# Move all servers free by current_time to free heap
while busy is not empty and busy.top().free_time <= current_time:
move busy.top() to free

# If free heap is empty, we must wait
if free is empty:
current_time = busy.top().free_time
while busy is not empty and busy.top().free_time <= current_time:
move busy.top() to free

# Assign task j
(weight, idx) = free.pop()
ans[j] = idx
busy.push((current_time + tasks[j], weight, idx))

Wait, there's a subtlety. The current_time after assigning task j is the time we assigned it. But task j+1 arrives at time j+1, not at current_time+1. So we shouldn't increment current_time just because we processed a task — we should set it to max(j, current_time) at the start of each iteration.

Actually, the time at which task j is assigned is max(j, earliest_free_time). If a server was free before j, we can assign at time j. So current_time for task j should be max(j, current_time_from_previous). Since j increases by 1 each iteration, current_time = max(current_time, j) at the start.

But wait — after assigning task j at time t, current_time stays t. For task j+1, we set current_time = max(t, j+1). If t > j+1, we stay at t. If t <= j+1, we move to j+1.

Let me re-check with Example 1.
Example 1: servers = [3,3,2], tasks = [1,2,3,2,1,2]

Initial: free = [(2,2), (3,0), (3,1)] (sorted by weight, then index), busy = [].

j=0: current_time = max(0,0)=0. Free not empty. Pop (2,2) → ans[0]=2. Push (0+1, 2, 2) = (1,2,2) to busy. current_time stays 0.

j=1: current_time = max(0,1)=1. Move busy: (1,2,2) has free_time 1 <= 1 → push (2,2) to free. Free = [(2,2), (3,0), (3,1)]. Pop (2,2) → ans[1]=2. Push (1+2,2,2)=(3,2,2). current_time = 1.

j=2: current_time = max(1,2)=2. Move busy: (3,2,2) free_time 3 > 2 → no move. Free = [(3,0),(3,1)]. Pop (3,0) → ans[2]=0. Push (2+3,3,0)=(5,3,0). current_time=2.

j=3: current_time = max(2,3)=3. Move busy: (3,2,2) free_time 3 <= 3 → push (2,2). Free = [(2,2),(3,1)]. Pop (2,2) → ans[3]=2. Push (3+2,2,2)=(5,2,2). current_time=3.

j=4: current_time = max(3,4)=4. Move busy: none (min free_time is 5). Free = [(3,1)]. Pop (3,1) → ans[4]=1. Push (4+1,3,1)=(5,3,1). current_time=4.

j=5: current_time = max(4,5)=5. Move busy: (5,3,0),(5,2,2),(5,3,1) all free_time 5 <= 5 → push all to free. Free = [(2,2),(3,0),(3,1)]. Pop (2,2) → ans[5]=2. Push (5+2,2,2)=(7,2,2).

Result: [2,2,0,2,1,2] ✅
Example 2: servers = [5,1,4,3,2], tasks = [2,1,2,4,5,2,1]

Let me trust the algorithm and check with code.
Final Code
cpp
*/
#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
using namespace std;

// Helper to print a vector<string>
void printVec(const vector<int>& v)
{
	cout << "[";
	for (int i = 0; i < (int)v.size(); i++)
	{
		cout << "\"" << v[i] << "\"";
		if (i + 1 < (int)v.size()) cout << ", ";
	}
	cout << "]";
}

// Helper to compare and print checkmark
/*
   printVec — helper to format a vector of strings as ["a", "b", "c"].

   check — compares expected vs actual using == (vector equality), prints both, and appends ✔ if they match or ✘ if they don't.

   Each test case is wrapped in its own {} block to avoid variable name collisions.
 */
void check(const vector<int>& expected, const vector<int>& actual)
{
	cout << "Expected: ";
	printVec(expected);
	cout << "\nOutput:   ";
	printVec(actual);
	if (expected == actual)
	{
		cout << "  ✔\n" << endl;
	}
	else
	{
		cout << "  ✘\n" << endl;
	}
}



class Solution 
{
	public:
		vector<int> assignTasks(vector<int>& servers, vector<int>& tasks) 
	{
			int n = (int)servers.size();
			int m = (int)tasks.size();

			// Free servers: min-heap by (weight, index)
			priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> freeServers;

			// Busy servers: min-heap by (free_time, weight, index)
			priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, greater<tuple<int,int,int>>> busyServers;

			// Initialize all servers as free
			for (int i = 0; i < n; i++) {
				freeServers.push({servers[i], i});
			}

			vector<int> ans(m);
			long long currentTime = 0;

			for (int j = 0; j < m; j++) {
				// Advance time to at least j (task arrival time)
				currentTime = max(currentTime, (long long)j);

				// Move servers that became free by currentTime to free heap
				while (!busyServers.empty() && get<0>(busyServers.top()) <= currentTime) {
					auto [ft, weight, idx] = busyServers.top();
					busyServers.pop();
					freeServers.push({weight, idx});
				}

				// If no free server, wait until the earliest busy server becomes free
				if (freeServers.empty()) {
					currentTime = get<0>(busyServers.top());
					while (!busyServers.empty() && get<0>(busyServers.top()) <= currentTime) {
						auto [ft, weight, idx] = busyServers.top();
						busyServers.pop();
						freeServers.push({weight, idx});
					}
				}

				// Assign task j to the best free server
				auto [weight, idx] = freeServers.top();
				freeServers.pop();
				ans[j] = idx;

				// Server becomes busy until currentTime + tasks[j]
				busyServers.push({(int)(currentTime + tasks[j]), weight, idx});
			}

			return ans;
		}
};

int main() {
	Solution sol;

	// Test 1
	{
		vector<int> servers1 = {3,3,2};
		vector<int> tasks1 = {1,2,3,2,1,2};
		vector<int> ans1 = sol.assignTasks(servers1, tasks1);
		/*    cout << "Test 1: [";
		      for (int i = 0; i < (int)ans1.size(); i++) {
		      cout << ans1[i];
		      if (i + 1 < (int)ans1.size()) cout << ",";
		      }
		      cout << "] (expected [2,2,0,2,1,2])" << endl;
		 */
		vector<int> expected = {2,2,0,2,1,2};
		check(expected, sol.assignTasks(servers1, tasks1));
	}
	// Test 2
	{
	vector<int> servers2 = {5,1,4,3,2};
	vector<int> tasks2 = {2,1,2,4,5,2,1};
	vector<int> ans2 = sol.assignTasks(servers2, tasks2);
/*
	cout << "Test 2: [";
	for (int i = 0; i < (int)ans2.size(); i++) {
		cout << ans2[i];
		if (i + 1 < (int)ans2.size()) cout << ",";
	}
	cout << "] (expected [1,4,1,4,1,3,2])" << endl;
*/
	        vector<int> expected = {1,4,1,4,1,3,2};
                check(expected, sol.assignTasks(servers2, tasks2));

	}
	return 0;
}
/*
   Dry Run of Final Code
   Dry Run 1: servers = [3,3,2], tasks = [1,2,3,2,1,2]

Initial:

freeServers = [(2,2), (3,0), (3,1)]

busyServers = []

currentTime = 0

j=0:

currentTime = max(0,0) = 0.

Busy empty → no move.

Free not empty.

Pop (2,2) → ans[0]=2. Push (0+1, 2, 2) = (1,2,2) to busy.

j=1:

currentTime = max(0,1) = 1.

Move busy: (1,2,2) has free_time 1 ≤ 1 → push (2,2) to free. Busy empty.

Free = [(2,2),(3,0),(3,1)].

Pop (2,2) → ans[1]=2. Push (1+2, 2, 2) = (3,2,2).

j=2:

currentTime = max(1,2) = 2.

Move busy: (3,2,2) free_time 3 > 2 → no move.

Free = [(3,0),(3,1)].

Pop (3,0) → ans[2]=0. Push (2+3, 3, 0) = (5,3,0).

j=3:

currentTime = max(2,3) = 3.

Move busy: (3,2,2) free_time 3 ≤ 3 → push (2,2). Busy = [(5,3,0)].

Free = [(2,2),(3,1)].

Pop (2,2) → ans[3]=2. Push (3+2, 2, 2) = (5,2,2).

j=4:

currentTime = max(3,4) = 4.

Move busy: min free_time = 5 > 4 → no move.

Free = [(3,1)].

Pop (3,1) → ans[4]=1. Push (4+1, 3, 1) = (5,3,1).

j=5:

currentTime = max(4,5) = 5.

Move busy: (5,3,0), (5,2,2), (5,3,1) all free_time 5 ≤ 5 → push all to free.

Free = [(2,2),(3,0),(3,1)].

Pop (2,2) → ans[5]=2. Push (5+2, 2, 2) = (7,2,2).

Result: [2,2,0,2,1,2] ✅
Dry Run 2: servers = [5,1,4,3,2], tasks = [2,1,2,4,5,2,1]

Initial:

free = [(1,1),(2,4),(3,3),(4,2),(5,0)]

busy = []

currentTime = 0

j=0: currentTime=0. Pop (1,1) → ans[0]=1. Push (2,1,1).

j=1: currentTime=1. Busy (2,1,1) free_time 2 > 1. Free = [(2,4),(3,3),(4,2),(5,0)]. Pop (2,4) → ans[1]=4. Push (1+1, 2, 4) = (2,2,4).

j=2: currentTime=2. Move busy: (2,1,1) → push (1,1). (2,2,4) → push (2,4). Free = [(1,1),(2,4),(3,3),(4,2),(5,0)]. Pop (1,1) → ans[2]=1. Push (2+2, 1, 1) = (4,1,1).

j=3: currentTime=3. Move busy: (4,1,1) free_time 4 > 3. Free = [(2,4),(3,3),(4,2),(5,0)]. Pop (2,4) → ans[3]=4. Push (3+4, 2, 4) = (7,2,4).

j=4: currentTime=4. Move busy: (4,1,1) → push (1,1). Free = [(1,1),(3,3),(4,2),(5,0)]. Pop (1,1) → ans[4]=1. Push (4+5, 1, 1) = (9,1,1).

j=5: currentTime=5. Move busy: (7,2,4) free_time 7 > 5. Free = [(3,3),(4,2),(5,0)]. Pop (3,3) → ans[5]=3. Push (5+2, 3, 3) = (7,3,3).

j=6: currentTime=6. Move busy: (7,...) free_time 7 > 6. Free = [(4,2),(5,0)]. Pop (4,2) → ans[6]=2. Push (6+1, 4, 2) = (7,4,2).

Result: [1,4,1,4,1,3,2] ✅
Complexity
Metric	Value
Time	O((n + m) log(n + m)) — each server pushed/popped a constant number of times
Space	O(n + m) — for the two heaps and answer array

For n, m ≤ 10^5, this is efficient.
Summary
Aspect	Detail
Problem	Assign tasks to servers with weight/index priority, handling busy times
Key insight	Two min-heaps: free servers (by weight, index) and busy servers (by free time, weight, index)
	Time handling	Advance currentTime to at least j; if no free server, jump to earliest busy free time
	Time	O((n + m) log(n + m))
Space	O(n + m)
	The "Aha" Moment

	The problem is a simulation with priority queues. The key is maintaining two heaps:

	Free servers ordered by (weight, index) — to pick the best available server.

	Busy servers ordered by (free_time, weight, index) — to know when servers become available.

	The subtle part is time handling: tasks arrive at j = 0, 1, 2, ..., but if all servers are busy, we must jump forward to the earliest free_time. The condition currentTime = max(currentTime, j) ensures we never assign a task before it arrives.
Final Code (Copy-Paste)
	cpp

	class Solution {
		public:
			vector<int> assignTasks(vector<int>& servers, vector<int>& tasks) {
				int n = servers.size(), m = tasks.size();
				priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> freeQ;
				priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, greater<>> busyQ;
				for (int i = 0; i < n; i++) freeQ.push({servers[i], i});
				vector<int> ans(m);
				long long cur = 0;
				for (int j = 0; j < m; j++) {
					cur = max(cur, (long long)j);
					while (!busyQ.empty() && get<0>(busyQ.top()) <= cur) {
						auto [ft, w, idx] = busyQ.top(); busyQ.pop();
						freeQ.push({w, idx});
					}
					if (freeQ.empty()) {
						cur = get<0>(busyQ.top());
						while (!busyQ.empty() && get<0>(busyQ.top()) <= cur) {
							auto [ft, w, idx] = busyQ.top(); busyQ.pop();
							freeQ.push({w, idx});
						}
					}
					auto [w, idx] = freeQ.top(); freeQ.pop();
					ans[j] = idx;
					busyQ.push({(int)(cur + tasks[j]), w, idx});
				}
				return ans;
			}
	};

🎯 Time: O((n + m) log(n + m)) | Space: O(n + m)
*/
