#include "shmutils.hpp"

int ShMem::open_shm(const char *memN, size_t size, const char *semN, int flags)
{
    shmName = memN;
    shmSize = size;
    semName = semN;
    switch (flags)
    {
    case MEM_READ:
        // Opens the shared memory segment for read
        fd = shm_open(shmName, O_RDWR, 0666);
        if (fd == -1)
        {
            std::cerr << "Failed to open shared memory segment: " << strerror(errno) << "\n";
            return -1;
        }
        // Opens the semaphore for synchronizing access to the shared memory segment
        sem = sem_open(semName, 0);
        if (sem == SEM_FAILED)
        {
            std::cerr << "Failed to open semaphore: " << strerror(errno) << "\n";
            return -1;
        }
        break;
    case MEM_CREATE | MEM_WRITE:
        // Creates and opens the shared memory segment
        fd = shm_open(shmName, O_CREAT | O_RDWR | O_EXCL, 0666);
        if (fd == -1)
        {
            std::cerr << "Failed to create shared memory segment: " << strerror(errno) << "\n";
            return errno;
        }
        // Truncate fd to shmSize length
        if (ftruncate(fd, shmSize) == -1)
        {
            std::cerr << "Failed to set size of shared memory segment: " << strerror(errno) << "\n";
            return -1;
        }
        // Creates a semaphore for synchronizing access to the shared memory segment
        sem = sem_open("/my_semaphore", O_CREAT | O_EXCL, 0666, 1);
        if (sem == SEM_FAILED)
        {
            std::cerr << "Failed to create semaphore: " << strerror(errno) << "\n";
            return -1;
        }
        break;

    default:
        break;
    }

    // Map the shared memory segment into our address space
    shmAddr = mmap(nullptr, shmSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shmAddr == MAP_FAILED)
    {
        std::cerr << "Failed to map shared memory segment: " << strerror(errno) << "\n";
        return -1;
    }
    return 0;
}

int ShMem::read_shm(char *buff)
{
    // Locks the semaphore
    if (sem_wait(sem) == -1)
    {
        std::cerr << "Failed to lock semaphore: " << strerror(errno) << "\n";
        return -1;
    }
    // Reads the content of shared memory and removes it after reading
    char *ch_ptr = static_cast<char *>(shmAddr);
    if (*ch_ptr)
    {
        memcpy(buff, shmAddr, shmSize);
        memset(shmAddr, 0, shmSize);
    }
    // Unlocks the semaphore
    if (sem_post(sem) == -1)
    {
        std::cerr << "Failed to unlock semaphore: " << strerror(errno) << "\n";
        return -1;
    }
    return 0;
}

int ShMem::write_shm(const char *buffer)
{
    // Locks the semaphore
    if (sem_wait(sem) == -1)
    {
        std::cerr << "Failed to lock semaphore: " << strerror(errno) << "\n";
        return -1;
    }
    // Writes to the shared memory segment with the contetnt of the buffer
    memcpy(shmAddr, buffer, strlen(buffer));
    // Unlocks the semaphore
    if (sem_post(sem) == -1)
    {
        std::cerr << "Failed to unlock semaphore: " << strerror(errno) << "\n";
        return -1;
    }
    return 0;
}

int ShMem::close_shm()
{
    // Deallocates the shared memory segment
    if (munmap(shmAddr, shmSize) == -1)
    {
        std::cerr << "Failed to deallocate shared memory: " << strerror(errno) << "\n";
        return -1;
    }
    // Closes the FD
    if (close(fd) == -1)
    {
        std::cerr << "Failed to close file descriptor: " << strerror(errno) << "\n";
        return -1;
    }
    // Closes the semaphore
    if (sem_close(sem) == -1)
    {
        std::cerr << "Failed to close descriptor for named semaphore: " << strerror(errno) << "\n";
        return -1;
    }
    return 0;
}

int ShMem::unlink_shm()
{
    // Removes the named semaphore
    if (sem_unlink(semName) == -1)
    {
        std::cerr << "Failed to remove named semaphore: " << strerror(errno) << "\n";
        return -1;
    }
    // Removes the named shared memory segment
    if (shm_unlink(shmName) == -1)
    {
        std::cerr << "Failed to remove the name of shared memory object: " << strerror(errno) << "\n";
        return -1;
    }
    return 0;
}
