
#include <iostream>
#include <atomic>
#include <csignal>
#include "../../shmlib/include/shmutils.hpp"

#define SHARED_MEMORY_NAME "/my_shared_memory"
#define SHARED_MEMORY_SEM_NAME "/my_semaphore"
#define SHARED_MEMORY_SIZE 1024

// Loop control flag
std::atomic<bool> running(true);

void signalHandler(int signum)
{
    std::cout << "\nInterrupt signal (" << signum << ") received.\n";
    running = false;
}

int main()
{   
    // Declares structure
    struct sigaction action;
    // Sets handler
    action.sa_handler = signalHandler;
    // Clears signal mask
    sigemptyset(&action.sa_mask);
    // Sets flags
    action.sa_flags = 0;

    // Signal handler registration
    if (sigaction(SIGINT, &action, nullptr) == -1)
    {
        std::cerr << "Error setting up signal handler." << std::endl;
        return 1;
    }

    ShMem shmObj;

    // Check for existing names
    if (shmObj.open_shm(SHARED_MEMORY_NAME, SHARED_MEMORY_SIZE, SHARED_MEMORY_SEM_NAME, MEM_CREATE | MEM_WRITE) == EEXIST)
    {
        shmObj.close_shm();
        shmObj.unlink_shm();
    }

    while (running)
    {
        // Write to the shared memory segment
        char buf[SHARED_MEMORY_SIZE];
        std::cout << "Write Data: ";
        std::cin.getline(buf, 1024);
        if (!strcmp("exit", buf))
            break;
        shmObj.write_shm(buf);
        std::cout << "Length: " << strlen(buf) << "\n";
    }

    // Clean up
    shmObj.close_shm();
    shmObj.unlink_shm();

    return 0;
}
