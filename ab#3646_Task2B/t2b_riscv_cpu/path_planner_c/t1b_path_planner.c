#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define PATH_PLANNED_BASE_ADDRESS 0x02000010
#define PATH_PLANNED(index) (*(volatile uint32_t*)(PATH_PLANNED_BASE_ADDRESS + (index * 4)))

#define CONNECTED_NODES_ADDRESS 0x02000110
#define CONNECTED_NODES(index) (*(volatile uint32_t*)(CONNECTED_NODES_ADDRESS + (index * 4)))

int removeRepeats(int path[], int length, int currentIndex) {
    if (currentIndex >= length) {
        return currentIndex;
    }

    // Check for repetition
    if (currentIndex >= 2 && path[currentIndex] == path[currentIndex - 2]) {
        // Remove the repeated segment by left-shifting the array
        for (int i = currentIndex - 1; i < length - 2; ++i) {
            path[i] = path[i + 2];
        }
        return removeRepeats(path, length - 2, currentIndex - 2);
    } else {
        // If no repetition, continue to the next element
        return removeRepeats(path, length, currentIndex + 1);
    }
}

int main(int argc, char const *argv[]) {

#ifdef __linux__

    const uint8_t START_POINT   = atoi(argv[1]);
    const uint8_t END_POINT     = atoi(argv[2]);
    uint8_t NODE_POINT          = 0;
    uint8_t CPU_DONE            = 0;
    uint8_t NODE                = 0;

#else
    // Address value of variables are updated for RISC-V Implementation
    #define START_POINT         (* (volatile uint8_t * ) 0x02000000)
    #define END_POINT           (* (volatile uint8_t * ) 0x02000004)
    #define NODE_POINT          (* (volatile uint8_t * ) 0x02000008)
    #define CPU_DONE            (* (volatile uint8_t * ) 0x0200000c)
    #define NODE                (* (volatile uint8_t * ) 0x0200026C)

#endif


    // Populate the path_planned array
    PATH_PLANNED(0)=0;
    PATH_PLANNED(1)=1;
    PATH_PLANNED(2)=2;
    PATH_PLANNED(3)=3;
    PATH_PLANNED(4)=4;
    PATH_PLANNED(5)=5;
    PATH_PLANNED(6)=4;
    PATH_PLANNED(7)=6;
    PATH_PLANNED(8)=7;
    PATH_PLANNED(9)=8;
    PATH_PLANNED(10)=9;
    PATH_PLANNED(11)=10;
    PATH_PLANNED(12)=9;
    PATH_PLANNED(13)=11;
    PATH_PLANNED(14)=9;
    PATH_PLANNED(15)=8;
    PATH_PLANNED(16)=12;
    PATH_PLANNED(17)=13;
    PATH_PLANNED(18)=14;
    PATH_PLANNED(19)=15;
    PATH_PLANNED(20)=14;
    PATH_PLANNED(21)=16;
    PATH_PLANNED(22)=17;
    PATH_PLANNED(23)=16;
    PATH_PLANNED(24)=18;
    PATH_PLANNED(25)=19;
    PATH_PLANNED(26)=20;
    PATH_PLANNED(27)=21;
    PATH_PLANNED(28)=22;
    PATH_PLANNED(29)=21;
    PATH_PLANNED(30)=23;
    PATH_PLANNED(31)=21;
    PATH_PLANNED(32)=20;
    PATH_PLANNED(32)=24;
    PATH_PLANNED(33)=25;
    PATH_PLANNED(34)=26;
    PATH_PLANNED(35)=27;
    PATH_PLANNED(36)=26;
    PATH_PLANNED(37)=28;
    PATH_PLANNED(38)=29;

    int startIdx = -1;
    int endIdx = -1;

    for (int i = 0; i < 40; ++i) {
        if (PATH_PLANNED(i) == START_POINT && startIdx == -1) {
            startIdx = i;
        }
        if (PATH_PLANNED(i) == END_POINT) {
            endIdx = i;
            break;  // No need to continue once END_POINT is found
        }
    }

    // Buffer to store connected node values
    int connectedIndex = 0;

    // Iterate through the path and add connected nodes to the buffer
    int path[40]; // Assuming a maximum path length of 40

if (START_POINT < END_POINT) {
    // Trace the path from start to end
    for (int i = startIdx, j = 0; i <= endIdx; ++i, ++j) {
        path[j] = PATH_PLANNED(i);
    }
} else {
    // Trace the path from end to start
    for (int i = endIdx, j = 0; i >= startIdx; --i, ++j) {
        path[j] = PATH_PLANNED(i);
    }
}

    // Remove repeated segments from the path
    connectedIndex = removeRepeats(path, endIdx - startIdx + 1, 0);

    // Store the final connected nodes back to NODE_POINT
    for (int i = 0; i < connectedIndex; ++i) {
        CONNECTED_NODES(i) = path[i];
        NODE_POINT = CONNECTED_NODES(i);
    }

    // Now NODE_POINT contains the final connected nodes
    // You can use NODE_POINT as needed in the rest of your code

    CPU_DONE = 1;

    return 0;
}