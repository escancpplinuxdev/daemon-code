/*
3360. Stone Removal Game
Easy
Topics
premium lock iconCompanies
Hint

Alice and Bob are playing a game where they take turns removing stones from a pile, with Alice going first.

    Alice starts by removing exactly 10 stones on her first turn.
    For each subsequent turn, each player removes exactly 1 fewer stone than the previous opponent.

The player who cannot make a move loses the game.

Given a positive integer n, return true if Alice wins the game and false otherwise.

 

Example 1:

Input: n = 12

Output: true

Explanation:

    Alice removes 10 stones on her first turn, leaving 2 stones for Bob.
    Bob cannot remove 9 stones, so Alice wins.

Example 2:

Input: n = 1

Output: false

Explanation:

    Alice cannot remove 10 stones, so Alice loses.

class Solution {
public:
    bool canAliceWin(int n) {
        
    }
};

*/

#include <iostream>
using namespace std;

class Solution {
public:
    bool canAliceWin(int n) {
        int removal = 10;
        int turn = 0;  // 0 = Alice, 1 = Bob
        
        while (n >= removal) {
            n -= removal;
            removal -= 1;
            turn = 1 - turn;
        }
        
        // If it's Bob's turn when the loop exits, Bob cannot move → Alice wins
        return turn == 1;
    }
};

int main() {
    Solution sol;
    
    cout << "Test 1: n=12 -> " << (sol.canAliceWin(12) ? "true" : "false") << " (expected true)" << endl;
    cout << "Test 2: n=1 -> " << (sol.canAliceWin(1) ? "true" : "false") << " (expected false)" << endl;
    cout << "Test 3: n=10 -> " << (sol.canAliceWin(10) ? "true" : "false") << " (expected true)" << endl;
    cout << "Test 4: n=9 -> " << (sol.canAliceWin(9) ? "true" : "false") << " (expected false)" << endl;
    cout << "Test 5: n=20 -> " << (sol.canAliceWin(20) ? "true" : "false") << endl;
    
    return 0;
}

/*

3360. Stone Removal Game
Explanation

This is a simple turn-simulation game with a fixed rule:

    Alice starts, removing exactly 10 stones.

    Each subsequent turn, the player removes 1 fewer stone than the previous opponent.

    A player loses if they cannot remove the required number of stones (i.e., the pile has fewer stones than needed).

    Alice goes first.

Key Observations

    The sequence of required removals is: 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1, ...

    Alice takes turns 1, 3, 5, 7, ... (odd turns)

    Bob takes turns 2, 4, 6, 8, ... (even turns)

    Once the required removal becomes ≤ 0, the game would theoretically continue forever — but actually, since the pile is finite, the game must end before that. Specifically, after the 10th turn (required removal = 1), the next requirement would be 0, which is trivially satisfiable by any pile (including 0). But removing 0 stones doesn't change the pile, so the game would loop forever. However, in practice, the pile will run out before we reach 0, because the total stones removed after 10 turns is 10 + 9 + ... + 1 = 55. If n > 55, the game would theoretically continue with 0-stone removals. But wait — the problem says "The player who cannot make a move loses." Removing 0 stones is a valid move (they can remove 0 stones if required), but it doesn't reduce the pile. This would cause an infinite loop. So the problem must guarantee that n is small enough that the game ends before the required removal reaches 0. Or, more likely, the rule is that each player removes exactly 1 fewer stone than the previous opponent, and if the required amount is 0 or negative, the player cannot make a move (since removing 0 or negative stones is not allowed — stones are positive). Let me re-read.

    "For each subsequent turn, each player removes exactly 1 fewer stone than the previous opponent. The player who cannot make a move loses the game."

If the required removal becomes 0 (after the 10th turn), the next player would need to remove 0 stones. Is that allowed? "Cannot make a move" typically means "cannot remove the required number of stones because there aren't enough." Removing 0 stones when 0 are required is technically possible, but it doesn't change the pile. This would lead to infinite play. So the problem likely guarantees that n is small enough (specifically, n ≤ 55 so the game ends before the requirement reaches 0). Let me check the constraints: 1 ≤ n ≤ 50 is typical for this problem. Given the problem is Easy and the examples use n = 12 and n = 1, I'll assume n ≤ 50 (so the game ends within 10 turns).

Wait — even if n = 50, the sum 10+9+8+7+6+5+4+3+2+1 = 55 > 50, so the game ends before turn 10. So the required removal never reaches 0. Good.
Simulation

We simulate the game:

    removal = 10 (Alice's first required removal)

    turn = 0 (0 = Alice, 1 = Bob)

    While n >= removal:

        n -= removal

        removal -= 1

        turn = 1 - turn

    The player whose turn it is when n < removal loses. So if turn == 0 (Alice's turn) when the loop ends, Alice loses → return false. If turn == 1 (Bob's turn), Alice wins → return true.

Wait, let me re-think. The loop continues while the current player can move (i.e., n >= removal). When the loop exits, the current player cannot move → they lose. So:

    If turn == 0 (Alice's turn) when loop exits → Alice loses → return false.

    If turn == 1 (Bob's turn) when loop exits → Bob loses → Alice wins → return true.

Example 1: n = 12
Turn	Player	removal	n before	n >= removal?	n after	turn after
1	Alice	10	12	✅ (12 >= 10)	2	Bob
2	Bob	9	2	❌ (2 < 9)	—	—

Loop exits with turn = 1 (Bob's turn). Bob cannot remove 9 → Bob loses → Alice wins → true. ✅
Example 2: n = 1
Turn	Player	removal	n before	n >= removal?
1	Alice	10	1	❌ (1 < 10)

Loop doesn't run. turn = 0 (Alice's turn). Alice cannot remove 10 → Alice loses → false. ✅
Final Code
cpp

#include <iostream>
using namespace std;

class Solution {
public:
    bool canAliceWin(int n) {
        int removal = 10;
        int turn = 0;  // 0 = Alice, 1 = Bob
        
        while (n >= removal) {
            n -= removal;
            removal -= 1;
            turn = 1 - turn;
        }
        
        // If it's Bob's turn when the loop exits, Bob cannot move → Alice wins
        return turn == 1;
    }
};

int main() {
    Solution sol;
    
    cout << "Test 1: n=12 -> " << (sol.canAliceWin(12) ? "true" : "false") << " (expected true)" << endl;
    cout << "Test 2: n=1 -> " << (sol.canAliceWin(1) ? "true" : "false") << " (expected false)" << endl;
    cout << "Test 3: n=10 -> " << (sol.canAliceWin(10) ? "true" : "false") << " (expected true)" << endl;
    cout << "Test 4: n=9 -> " << (sol.canAliceWin(9) ? "true" : "false") << " (expected false)" << endl;
    cout << "Test 5: n=20 -> " << (sol.canAliceWin(20) ? "true" : "false") << endl;
    
    return 0;
}

Dry Run of Final Code
Dry Run 1: n = 12

Initial: n = 12, removal = 10, turn = 0 (Alice).
Iteration	n	removal	n >= removal?	Action	n after	removal after	turn after
1	12	10	✅	n -= 10	2	9	1 (Bob)
—	2	9	❌	loop exits	—	—	1

turn = 1 (Bob's turn) → Bob loses → true. ✅
Dry Run 2: n = 1

Initial: n = 1, removal = 10, turn = 0.
Iteration	n	removal	n >= removal?
—	1	10	❌

Loop doesn't run. turn = 0 (Alice's turn) → Alice loses → false. ✅
Dry Run 3: n = 10

Initial: n = 10, removal = 10, turn = 0.
Iteration	n	removal	n >= removal?	Action	n after	removal after	turn after
1	10	10	✅	n -= 10	0	9	1 (Bob)
—	0	9	❌	loop exits	—	—	1

turn = 1 → Bob loses → true. ✅
Dry Run 4: n = 9

Initial: n = 9, removal = 10, turn = 0.
Iteration	n	removal	n >= removal?
—	9	10	❌

Loop doesn't run. turn = 0 → Alice loses → false. ✅
Dry Run 5: n = 20

Initial: n = 20, removal = 10, turn = 0.
Iteration	n	removal	n >= removal?	n after	removal after	turn after
1	20	10	✅	10	9	1 (Bob)
2	10	9	✅	1	8	0 (Alice)
3	1	8	❌	—	—	0

turn = 0 → Alice loses → false. ✅

Verification: Alice removes 10 (n=10). Bob removes 9 (n=1). Alice needs to remove 8 but only 1 left → Alice loses. ✅
Complexity
Time Complexity
Metric	Value	Reasoning
Time	O(1)	At most 10 iterations (since removal decreases from 10 to 1, and n ≤ 55 for the game to end within turns). Even for larger n, the loop runs at most 10 times before removal hits 0.
Space Complexity
Metric	Value	Reasoning
Space	O(1)	Only two integer variables.
How to Explain to an Interviewer

    Problem restatement: "Alice removes 10, then each player removes 1 less than the previous. A player loses if they can't remove the required amount. Return true if Alice wins."

    Approach: "Simulate the game. Start with removal = 10, turn = 0 (Alice). While the pile has at least removal stones, subtract removal from n, decrement removal, and switch turns. When the loop exits, the current player cannot move — they lose. If it's Bob's turn, Alice wins."

    Time complexity: "At most 10 iterations because removal decreases from 10 to 1. So O(1)."

    Space complexity: "Only a few integer variables — O(1)."

Summary
Aspect	Detail
Problem	Simulate stone-removal game; return true if Alice wins
Approach	Direct simulation of turns
Key insight	At most 10 turns; simulate directly
Time	O(1) — at most 10 iterations
Space	O(1)
Winner logic	After loop, if turn == 1 (Bob's turn), Bob loses → Alice wins
The "Aha" Moment

The game has a fixed, deterministic sequence of required removals: 10, 9, 8, ..., 1. There's no strategy — just simulate. Since there are at most 10 turns (because removal decreases by 1 each time starting from 10), the simulation is O(1) time and space. The only subtlety is correctly tracking whose turn it is when the pile runs out: if it's Bob's turn and he can't move, Alice wins.
Final Code (Copy-Paste)
cpp

class Solution {
public:
    bool canAliceWin(int n) {
        int removal = 10;
        int turn = 0;  // 0 = Alice, 1 = Bob
        while (n >= removal) {
            n -= removal;
            removal--;
            turn = 1 - turn;
        }
        return turn == 1;
    }
};

🎯 Time: O(1) | Space: O(1)

*/
