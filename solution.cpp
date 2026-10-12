#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

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
    
    // Array to track current seat usage between stations
    // seats[i] represents seats used between station i+1 and i+2 (0-based)
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