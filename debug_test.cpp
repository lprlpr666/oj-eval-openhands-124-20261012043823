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
    // Test case 1: Original sample
    cout << "Test 1 - Original sample:" << endl;
    {
        int x = 4, k = 6, p = 4;
        LazySegmentTree st(x - 1);
        
        vector<pair<pair<int, int>, int>> orders = {
            {{1, 4}, 2},
            {{1, 3}, 2},
            {{2, 4}, 3},
            {{1, 2}, 3}
        };
        
        for (auto order : orders) {
            int u = order.first.first;
            int v = order.first.second;
            int n = order.second;
            
            int start_seg = u - 1;
            int end_seg = v - 2;
            
            int max_usage = st.queryRange(start_seg, end_seg);
            
            if (max_usage + n <= k) {
                st.updateRange(start_seg, end_seg, n);
                cout << "T" << endl;
            } else {
                cout << "N" << endl;
            }
        }
    }
    
    // Test case 2: Simple case
    cout << "\nTest 2 - Simple case:" << endl;
    {
        int x = 3, k = 5, p = 2;
        LazySegmentTree st(x - 1);
        
        // Order 1: 1->2, 3 seats (should pass)
        int max_usage = st.queryRange(0, 0);
        if (max_usage + 3 <= k) {
            st.updateRange(0, 0, 3);
            cout << "T" << endl;
        } else {
            cout << "N" << endl;
        }
        
        // Order 2: 2->3, 3 seats (should pass)
        max_usage = st.queryRange(1, 1);
        if (max_usage + 3 <= k) {
            st.updateRange(1, 1, 3);
            cout << "T" << endl;
        } else {
            cout << "N" << endl;
        }
    }
    
    // Test case 3: Edge case - single segment
    cout << "\nTest 3 - Single segment:" << endl;
    {
        int x = 2, k = 10, p = 3;
        LazySegmentTree st(x - 1);
        
        // Order 1: 1->2, 5 seats (should pass)
        int max_usage = st.queryRange(0, 0);
        if (max_usage + 5 <= k) {
            st.updateRange(0, 0, 5);
            cout << "T" << endl;
        } else {
            cout << "N" << endl;
        }
        
        // Order 2: 1->2, 3 seats (should pass)
        max_usage = st.queryRange(0, 0);
        if (max_usage + 3 <= k) {
            st.updateRange(0, 0, 3);
            cout << "T" << endl;
        } else {
            cout << "N" << endl;
        }
        
        // Order 3: 1->2, 3 seats (should fail)
        max_usage = st.queryRange(0, 0);
        if (max_usage + 3 <= k) {
            st.updateRange(0, 0, 3);
            cout << "T" << endl;
        } else {
            cout << "N" << endl;
        }
    }
    
    return 0;
}