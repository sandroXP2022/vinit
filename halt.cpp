#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/reboot.h>

int main() {
    printf("Halting the system...\n");
    if (reboot(RB_HALT_SYSTEM) == -1) {
        perror("halt failed");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

