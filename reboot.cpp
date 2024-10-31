// reboot.cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/reboot.h>

int main() {
    printf("Rebooting the system...\n");
    if (reboot(RB_AUTOBOOT) == -1) {
        perror("reboot failed");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

