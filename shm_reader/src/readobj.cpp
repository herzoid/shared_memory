
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

    // Create and open the shared memory object
    ShMem shmObj;
    shmObj.open_shm(SHARED_MEMORY_NAME, SHARED_MEMORY_SIZE, SHARED_MEMORY_SEM_NAME, MEM_READ);
    
    const char *mrk = "[READ]";
    while (running)
    {
        // Read from the shared memory segment
        char buf[SHARED_MEMORY_SIZE];
        shmObj.read_shm(buf);
        // print to console
        int ibuf = static_cast<int>(*buf);
        !strcmp(mrk, "[READ]") ? mrk = "[    ]" : mrk = "[READ]";
        std::wcout << mrk << " Shared memory content:\n"
                   << buf << '\n';
        sleep(1);
        system("clear");
    }

    // Clean up
    shmObj.close_shm();

    return 0;
}