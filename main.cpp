#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <ctime>

#include "inc/Nat.h"

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    constexpr uint32_t NUMBER_OF_ADDRESSES = 5;
    constexpr uint32_t MAX_PER_ADDRESS = 3;

    Nat nat(NUMBER_OF_ADDRESSES, MAX_PER_ADDRESS);

    constexpr int TOTAL_OPERATIONS = 20;
    uint32_t total_allocs = 0;
    uint32_t total_frees = 0;
    uint32_t address_usage[NUMBER_OF_ADDRESSES] = {0};

    std::cout << "=== Nat Allocator Simulation ===\n";
    std::cout << "Total addresses: " << NUMBER_OF_ADDRESSES
              << ", Max per address: " << MAX_PER_ADDRESS << "\n";

    for (int i = 0; i < TOTAL_OPERATIONS; ++i) {
        if ((rand() % 100) < 70) {
            if (uint32_t addr; nat.addr_alloc(addr)) {
                std::cout << "Allocated address: " << addr << "\n";
                address_usage[addr]++;
                total_allocs++;
            } else {
                std::cout << "Allocation failed: no free address available\n";
            }
        } else {
            // Попробуем освободить случайный адрес
            if (const uint32_t random_addr = rand() % NUMBER_OF_ADDRESSES; nat.addr_free(random_addr)) {
                std::cout << "Freed address: " << random_addr << "\n";
                if (address_usage[random_addr] > 0)
                    address_usage[random_addr]--;
                total_frees++;
            } else {
                std::cout << "Free failed: address " << random_addr << " is empty or invalid\n";
            }
        }
    }

    std::cout << "\n=== Final Statistics ===\n";
    std::cout << "Total allocations: " << total_allocs << "\n";
    std::cout << "Total frees: " << total_frees << "\n";
    std::cout << "Current usage per address:\n";
    for (uint32_t i = 0; i < NUMBER_OF_ADDRESSES; ++i) {
        std::cout << "Address " << i << ": " << address_usage[i] << " abonent(s)\n";
    }

    return 0;
}