#include <stdio.h>

#include "platform.h"
#include "rnbench.h"

void progress_callback(int percent) {
    if (percent == RNBENCH_PROGRESS_FINISHED) {
        return;
    }
    
    if (percent == 0) {
        printf("Benchmarking");
    }

    if (percent % 3 == 0) {
        putchar('.');
        fflush(stdout);
    }
}

int main(int argc, char* argv[]) {
    printf("Retro Nano Bench v%s\n\n", RNBENCH_VERSION);
    
    unsigned int final_result = rnbench_run(progress_callback);

    printf("\n\nResult: %u\n", final_result);
    return 0;
}