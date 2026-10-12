#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class LazySegmentTree {
private:
    vector<int> max_val;
    vector<int> lazy;
    int n;
    
    void push(int node, int node_left, int node_right) {
        if (lazy[node] != 0) {
            max_val[node] += lazy[node];
            if (node_left != node_right) {
                int mid = (node_left + node_right) / 2;
                lazy[2 * node] += lazy[node];
                lazy[2 * node + 1] += lazy[node];
            }
            lazy[node] = 0;
        }
    }
    
    void range_add(int node, int node_left, int node_right, int query_left, int query_right, int value) {
        push(node, node_left, node_right);
        
        if (query_left > node_right || query_right < node_left) {
            return;
        }
        
        if (query_left <= node_left && node_right <= query_right) {
            lazy[node] += value;
            push(node, node_left, node_right);
            return;
        }
        
        int mid = (node_left + node_right) / 2;
        range_add(2 * node, node_left, mid, query_left, query_right, value);
        range_add(2 * node + 1, mid + 1, node_right, query_left, query_right, value);
        
        push(2 * node, node_left, mid);
        push(2 * node + 1, mid + 1, node_right);
        max_val[node] = max(max_val[2 * node], max_val[2 * node + 1]);
    }
    
    int range_max(int node, int node_left, int node_right, int query_left, int query_right) {
        push(node, node_left, node_right);
        
        if (query_left > node_right || query_right < node_left) {
            return 0;
        }
        
        if (query_left <= node_left && node_right <= query_right) {
            return max_val[node];
        }
        
        int mid = (node_left + node_right) / 2;
        int left_result = range_max(2 * node, node_left, mid, query_left, query_right);
        int right_result = range_max(2 * node + 1, mid + 1, node_right, query_left, query_right);
        return max(left_result, right_result);
    }
    
public:
    LazySegmentTree(int size) {
        n = size;
        max_val.resize(4 * n, 0);
        lazy.resize(4 * n, 0);
    }
    
    void add_range(int left, int right, int value) {
        if (left <= right) {
            range_add(1, 0, n - 1, left, right, value);
        }
    }
    
    int get_max(int left, int right) {
        if (left <= right) {
            return range_max(1, 0, n - 1, left, right);
        }
        return 0;
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
    
    // We only need x-1 segments since there are x stations
    LazySegmentTree st(x - 1);
    
    for (int i = 0; i < p; i++) {
        int u, v, n;
        scanf("%d %d %d", &u, &v, &n);
        
        // Handle the case where u > v (reverse journey)
        if (u > v) {
            swap(u, v);
        }
        
        // Convert to 0-based indexing for segments
        // Journey from station u to v covers segments u-1 to v-2
        int start_seg = u - 1;
        int end_seg = v - 2;
        
        // Check maximum seat usage in the range
        int max_usage = st.get_max(start_seg, end_seg);
        
        if (max_usage + n <= k) {
            // Order can be fulfilled, update all segments
            st.add_range(start_seg, end_seg, n);
            printf("T\n");
        } else {
            printf("N\n");
        }
    }
    
    return 0;
}