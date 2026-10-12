#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class FenwickTree {
private:
    vector<int> tree;
    int n;
    
    void update(int idx, int val) {
        for (; idx < n; idx |= idx + 1) {
            tree[idx] += val;
        }
    }
    
    int query(int idx) {
        int result = 0;
        for (; idx >= 0; idx = (idx & (idx + 1)) - 1) {
            result += tree[idx];
        }
        return result;
    }
    
public:
    FenwickTree(int size) {
        n = size;
        tree.resize(n, 0);
    }
    
    void rangeAdd(int l, int r, int val) {
        update(l, val);
        if (r + 1 < n) {
            update(r + 1, -val);
        }
    }
    
    int pointQuery(int idx) {
        return query(idx);
    }
};

int main() {
    int x, k, p;
    scanf("%d %d %d", &x, &k, &p);
    
    // Special case: if there's only 1 station, no journeys are possible
    if (x == 1) {
        for (int i = 0; i < p; i++) {
            int u, v, n;
            scanf("%d %d %d", &u, &v, &n);
            // Any journey from station 1 to 1 has no segments, so it's always possible
            printf("T\n");
        }
        return 0;
    }
    
    // We'll use a simple array approach since Fenwick tree gives us point queries
    // but we need range maximum queries
    vector<int> seats(x - 1, 0);
    
    for (int i = 0; i < p; i++) {
        int u, v, n;
        scanf("%d %d %d", &u, &v, &n);
        
        // Convert to 0-based indexing for segments
        // Journey from station u to v covers segments u-1 to v-2
        int start_seg = u - 1;
        int end_seg = v - 2;
        
        // Check if order can be fulfilled
        bool can_fulfill = true;
        for (int seg = start_seg; seg <= end_seg; seg++) {
            if (seats[seg] + n > k) {
                can_fulfill = false;
                break;
            }
        }
        
        if (can_fulfill) {
            // Update seat usage for all segments in this journey
            for (int seg = start_seg; seg <= end_seg; seg++) {
                seats[seg] += n;
            }
            printf("T\n");
        } else {
            printf("N\n");
        }
    }
    
    return 0;
}