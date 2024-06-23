#include <string>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include <print>
#include <iostream>

char* get_shared_memory() {
     // creates unique key
    auto key = ftok("myfile", 42);

    // open shared memory - result is a handle to shared memory
    // 0x1000 - 4KB of memory
    // 0666 - open memory with read and write permissions
    // IPC_CREAT - create if it does not exist
    auto shm = shmget(key, 0x1000, 0666 | IPC_CREAT);

    // returns pointer to the shared memory
    return static_cast<char *>(shmat(shm, nullptr, 0));
}

void use_shared_memory() {
    constexpr std::int32_t token = 42;
    constexpr std::int32_t child{0};

    if (fork()!=child) {
        // sleep for a second
        sleep(10);
        std::cout << "parent\n";

        auto msg = get_shared_memory();
        msg[0] = 42;
        
        wait(nullptr);
    } else {
        auto msg = get_shared_memory();
        while(msg[0] != 42);
        std::cout << "child\n";
    }
}