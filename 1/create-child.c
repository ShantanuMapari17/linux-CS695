#include <unistd.h>
#include <stdio.h>
#include <stdio.h>
int main() {
    pid_t child_pid;

    printf("Parent Process PID: %d\n", getpid());

    // Create two child processes
    for (int i = 0; i < 4; ++i) {
        child_pid = fork();

        if (child_pid == 0) {
            // Child process
            printf("Child Process PID: %d\n", getpid());
            sleep(60);  // Sleep for a while to keep the child processes alive
            return 0;
        } else if (child_pid < 0) {
            // Fork failed
            printf("Fork failed.\n");
            return -1;
        }
    }

    // Wait for child processes to complete
    for (int i = 0; i < 4; ++i) {
        wait(NULL);
    }

    return 0;
}
