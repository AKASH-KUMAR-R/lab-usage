#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>  // For seeding the random number generator

#define NOF_PACKETS 10

// Function to generate random numbers in the range [1, a-1]
int rand_custom(int a)
{
    int rn = (rand() % 10) % a;
    return rn == 0 ? 1 : rn;  // Ensure a non-zero value is returned
}

int main()
{
    int packet_sz[NOF_PACKETS], i, clk, b_size, o_rate, p_sz_rm = 0, p_sz, p_time, op;

    // Seed the random number generator
    srand(time(0));

    // Generate random packet sizes
    for (i = 0; i < NOF_PACKETS; ++i) {
        packet_sz[i] = rand_custom(6) * 10;  // Multiplying by 10 for packet size
    }

    // Display generated packet sizes
    for (i = 0; i < NOF_PACKETS; ++i) {
        printf("\nPacket[%d]: %d bytes\t", i, packet_sz[i]);
    }

    // Get output rate and bucket size from user
    printf("\nEnter the Output rate: ");
    scanf("%d", &o_rate);
    printf("Enter the Bucket Size: ");
    scanf("%d", &b_size);

    // Process each packet
    for (i = 0; i < NOF_PACKETS; ++i) {
        if ((packet_sz[i] + p_sz_rm) > b_size) {
            if (packet_sz[i] > b_size) {  // Compare packet size with bucket size
                printf("\n\nIncoming packet size (%d bytes) is greater than bucket capacity (%d bytes) - PACKET REJECTED", packet_sz[i], b_size);
            } else {
                printf("\n\nBucket capacity exceeded - PACKET REJECTED!!");
            }
        } else {
            p_sz_rm += packet_sz[i];  // Add packet size to the remaining size to transmit
            printf("\n\nIncoming Packet size: %d bytes", packet_sz[i]);
            printf("\nBytes remaining to Transmit: %d bytes", p_sz_rm);

            // Generate random transmission time
            p_time = rand_custom(4) * 10;
            printf("\nTime left for transmission: %d units", p_time);

            // Simulate packet transmission
            for (clk = 10; clk <= p_time; clk += 10) {
                sleep(1);  // Sleep for 1 second to simulate time passage

                if (p_sz_rm) {
                    if (p_sz_rm <= o_rate) {  // If remaining packet size is less than or equal to output rate
                        op = p_sz_rm;
                        p_sz_rm = 0;
                    } else {
                        op = o_rate;
                        p_sz_rm -= o_rate;
                    }

                    printf("\nPacket of size %d bytes transmitted", op);
                    printf("----Bytes Remaining to Transmit: %d bytes", p_sz_rm);
                } else {
                    printf("\nTime left for transmission: %d units", p_time - clk);
                    printf("\nNo packets to transmit!!");
                }
            }
        }
    }

    return 0;
}