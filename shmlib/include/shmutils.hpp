#pragma once
#include <sys/mman.h>
#include <semaphore.h>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#define MEM_CREATE 01
#define MEM_READ 02
#define MEM_WRITE 04

class ShMem
{
private:
    /// @brief File descriptor
    int fd;
    /// @brief Shared memory segment address
    void *shmAddr;
    /// @brief Size of mshared memory segment
    size_t shmSize;
    /// @brief Semaphore name
    const char *semName;
    /// @brief Shared memory segment name
    const char *shmName;
    /// @brief Semaphore
    sem_t *sem;
public:
    /// @brief Opens shared memory segment with semaphore to perform actions depends of passed flags
    /// @param memName Shared memory segment name
    /// @param size Shared memory segment size
    /// @param semName Semaphore name
    /// @param flags Flags to pass
    /// @return 0 on success, -1 on error
    int open_shm(const char *memName, size_t size, const char *semName, int flags);

    /// @brief Reads shared memory segment content to a buffer
    /// @param buffer Buffer to write content
    /// @return 0 on success, -1 on error
    int read_shm(char *buffer);

    /// @brief Writes to the shared memory with the content of the buffer
    /// @param buffer 
    /// @return 0 on success, -1 on error
    int write_shm(const char *buffer);

    /// @brief Closes shared memory segment
    /// @return 0 on success, -1 on error
    int close_shm();

    /// @brief Removes the name of shared memory object and the name of the semaphore
    /// @return 0 on success, -1 on error
    int unlink_shm();
};
