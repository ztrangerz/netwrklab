#include <stdio.h>

int main() {
    int incoming, outgoing, buck_size, n, store = 0;
    
    printf("Enter bucket size, outgoing rate, and the number of inputs: ");
    scanf("%d %d %d", &buck_size, &outgoing, &n);
    
    while (n != 0) {
        printf("Enter incoming packet size: ");
        scanf("%d", &incoming);
        
        // Handle incoming packets
        if (incoming + store <= buck_size) {
            store += incoming;
            printf("Bucket size %d out of %d\n", store, buck_size);
        } else {
            int overflow = incoming + store - buck_size;
            printf("Dropped %d packets\n", overflow);
            printf("Bucket buffer size %d out of %d\n", store, buck_size);
            store = buck_size;
        }
        
        // Handle outgoing packets
        if (outgoing <= store) {
            store -= outgoing;
        } else {
            int deficit = outgoing - store;
            printf("After outgoing %d packets, buffer underflow: %d packets\n", outgoing, deficit);
            store = 0;
        }
        
        printf("After outgoing %d packets left out of %d in buffer\n", outgoing, store);
        n--;
    }
    
    return 0;
}