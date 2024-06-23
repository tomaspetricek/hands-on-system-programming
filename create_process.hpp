#include <unistd.h>

#include <print>
#include <span>
#include <array>

constexpr pid_t child_id = 0;

constexpr std::size_t process_count{4};

std::array<pid_t, 2> children0 = {0, 0};
std::array<pid_t, 1> children1 = {0};
std::array<pid_t, 0> children2 = {};
std::array<pid_t, 0> children3 = {};

std::array<std::span<pid_t>, process_count> children_ids = {
    std::span<pid_t>(children0),
    std::span<pid_t>(children1),
    std::span<pid_t>(children2),
    std::span<pid_t>(children3)
};

std::array<std::size_t, process_count> children_count{0, 0, 0, 0};
std::array<pid_t, process_count> process_ids{0, 0, 0, 0};

pid_t get_current_id() {
    return getpid();
}

pid_t get_parent_id() {
    return getppid();
}

std::size_t get_current_index(const pid_t origin_id, const pid_t current_id) {
    return current_id-origin_id;
}

bool is_parent(const pid_t forked) {
    return forked!=child_id;
}

void try_register_child(const std::size_t process_index, const pid_t forked) {
    if (is_parent(forked)) {
        auto& child_index = children_count[process_index];
        children_ids[process_index][child_index++] = forked;
    }
}

void register_process_id(const std::size_t process_index, const pid_t process_id) {
    process_ids[process_index] = process_id;
}

void create_processes() {
    std::int32_t origin_id{getpid()}, current_id;
    pid_t forked;
    std::size_t current_index{0};

    forked = fork();
    current_id = get_current_id();
    current_index = get_current_index(origin_id, current_id);
    try_register_child(current_index, forked);

    forked = fork();
    current_id = get_current_id();
    current_index = get_current_index(origin_id, current_id);
    try_register_child(current_index, forked);

    register_process_id(current_index, current_id);
        
    std::print("parent: {}, children: ", process_ids[current_index]);
    for (const auto &child_id : children_ids[current_index]) {
        std::print("{} ", child_id);
    }
    std::print("\n");
}