#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class LazySegmentTree {
private:
    vector<int> tree;
    vector<int> lazy;
    int n;
    
    void push(int node, int start, int end) {
        if (lazy[node] != 0) {
            tree[node] += lazy[node];
            if (start != end) {
                lazy[2 * node] += lazy[node];
                lazy[2 * node + 1] += lazy[node];
            }
            lazy[node] = 0;
        }
    }
    
    void updateRange(int node, int start, int end, int l, int r, int val) {
        push(node, start, end);
        if (start > r || end < l) {
            return;
        }
        if (start >= l && end <= r) {
            lazy[node] += val;
            push(node, start, end);
            return;
        }
        int mid = (start + end) / 2;
        updateRange(2 * node, start, mid, l, r, val);
        updateRange(2 * node + 1, mid + 1, end, l, r, val);
        push(2 * node, start, mid);
        push(2 * node + 1, mid + 1, end);
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }
    
    int queryRange(int node, int start, int end, int l, int r) {
        if (start > r || end < l) {
            return 0;
        }
        push(node, start, end);
        if (start >= l && end <= r) {
            return tree[node];
        }
        int mid = (start + end) / 2;
        int left_max = queryRange(2 * node, start, mid, l, r);
        int right_max = queryRange(2 * node + 1, mid + 1, end, l, r);
        return max(left_max, right_max);
    }
    
public:
    LazySegmentTree(int size) {
        n = size;
        tree.resize(4 * n, 0);
        lazy.resize(4 * n, 0);
    }
    
    void updateRange(int l, int r, int val) {
        updateRange(1, 0, n - 1, l, r, val);
    }
    
    int queryRange(int l, int r) {
        return queryRange(1, 0, n - 1, l, r);
    }
};

int main() {
    int x, k, p;
    scanf("%d %d %d", &x, &k, &p);
    
    // We only need x-1 segments since there are x stations
    LazySegmentTree st(x - 1);
    
    for (int i = 0; i < p; i++) {
        int u, v, n;
        scanf("%d %d %d", &u, &v, &n);
        
        // Convert to 0-based indexing for segments
        // Journey from station u to v covers segments u-1 to v-2
        int start_seg = u - 1;
        int end_seg = v - 2;
        
        // Check maximum seat usage in the range
        int max_usage = st.queryRange(start_seg, end_seg);
        
        if (max_usage + n <= k) {
            // Order can be fulfilled, update all segments
            st.updateRange(start_seg, end_seg, n);
            printf("T\n");
        } else {
            printf("N\n");
        }
    }
    
    return 0;
}