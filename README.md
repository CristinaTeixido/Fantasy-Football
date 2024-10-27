This project aims to design a tool that determines the optimal player lineup for a football team based on specified constraints. The tool employs three different algorithms—exhaustive search, greedy, and heuristics—to evaluate and select the best lineup for maximizing performance and meeting the set criteria.

# **Algorithms**

### **Exhaustive Search:**

This algorithm evaluates every possible lineup combination to find the one that best meets the criteria. While it guarantees the best solution, it's computationally intensive and may be time-consuming for larger player pools.

### **Greedy Approach:**

The greedy algorithm builds the lineup by selecting players one at a time based on the highest value for a specific attribute (e.g., skill level, stamina). Though faster, it may not always reach the optimal solution.

### **Heuristics:**

This algorithm uses predefined rules or scoring functions to quickly generate a lineup that satisfies the constraints. It’s efficient and typically balances speed with reasonably good results, though it may not always be the optimal solution.
