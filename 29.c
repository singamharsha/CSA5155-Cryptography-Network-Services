#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define STATE_SIZE 25 

void initialize_state(uint64_t state[STATE_SIZE]) {
    memset(state, 0, sizeof(uint64_t) * STATE_SIZE);
}

void print_state(uint64_t state[STATE_SIZE]) {
    int i;
    for (i = 0; i < STATE_SIZE; ++i) {
        printf("%016llx ", state[i]);
        if ((i + 1) % 5 == 0) printf("\n");
    }
}

void set_lane(uint64_t state[STATE_SIZE], int lane_index, uint64_t value) {
    state[lane_index] = value;
}

int count_nonzero_capacity(uint64_t state[STATE_SIZE], int capacity_start, int capacity_end) {
    int i;
    int count = 0;
    for (i = capacity_start; i < capacity_end; ++i) {
        if (state[i] != 0) {
            count++;
        }
    }
    return count;
}

int main() {
    uint64_t state[STATE_SIZE];
    int i;
    int num_steps = 0;
    
    initialize_state(state);

    for (i = 0; i < 5; ++i) {
        set_lane(state, i, 0x0101010101010101ULL);
    }

    printf("Initial state:\n");
    print_state(state);
    
    int capacity_start = 12;
    int capacity_end = 25;

    while (count_nonzero_capacity(state, capacity_start, capacity_end) < (capacity_end - capacity_start)) {
        num_steps++;
        set_lane(state, (capacity_start + num_steps) % STATE_SIZE, 0x0202020202020202ULL);
        printf("Step %d:\n", num_steps);
        print_state(state);
    }

    printf("Total steps to have all non-zero lanes in the capacity portion: %d\n", num_steps);

    return 0;
}

