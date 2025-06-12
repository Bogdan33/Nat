//
// Created by Богдан on 31.05.2025.
//

#include "../inc/Nat.h"

#include <cstdint>
#include <stdexcept>

Nat::Nat(uint32_t num_addresses, uint32_t max_per_address)
    : NUMBER_OF_ADDRESSES(num_addresses),
      MAX_NUMBER_ABONENT_PER_ADDRESS(max_per_address),
      first_non_full_level(0)
{

    if (num_addresses == 0 || max_per_address == 0)
        throw std::invalid_argument("Invalid parameters");

    const uint32_t LEVEL_COUNT = max_per_address + 1;

    // Выделяем одномерные массивы
    address_count = std::make_unique<uint32_t[]>(num_addresses);
    level_array_sizes = std::make_unique<uint32_t[]>(LEVEL_COUNT);
    level_array_heads = std::make_unique<uint32_t[]>(LEVEL_COUNT);

    // Выделяем двумерные структуры
    level_arrays = std::make_unique<uint32_t*[]>(LEVEL_COUNT);
    address_to_index = std::make_unique<uint32_t*[]>(LEVEL_COUNT);

    for (uint32_t k = 0; k < LEVEL_COUNT; ++k)
    {
        level_arrays[k] = new uint32_t[num_addresses];
        address_to_index[k] = new uint32_t[num_addresses];
        level_array_sizes[k] = 0;
        level_array_heads[k] = 0;
    }

    // Инициализируем начальное состояние: все адреса в level 0
    for (uint32_t i = 0; i < num_addresses; ++i)
    {
        level_arrays[0][i] = i;
        address_to_index[0][i] = i;
        address_count[i] = 0;
    }
    level_array_sizes[0] = num_addresses;
}

bool Nat::addr_alloc(uint32_t& placewheretowriteaddress) {
    if (first_non_full_level >= MAX_NUMBER_ABONENT_PER_ADDRESS)
        return false;

    const uint32_t k = first_non_full_level;

    const uint32_t addr = level_arrays[k][level_array_heads[k]];
    level_array_heads[k]++;

    if (level_array_heads[k] >= level_array_sizes[k])
    {
        uint32_t next_level = k + 1;
        while (next_level <= MAX_NUMBER_ABONENT_PER_ADDRESS &&
               level_array_heads[next_level] >= level_array_sizes[next_level])
        {
            ++next_level;
        }

        first_non_full_level = (next_level <= MAX_NUMBER_ABONENT_PER_ADDRESS)
            ? next_level
            : MAX_NUMBER_ABONENT_PER_ADDRESS + 1;
    }

    const uint32_t old_k = address_count[addr];
    const uint32_t new_k = old_k + 1;
    address_count[addr] = new_k;

    // Перемещаем адрес в новый уровень
    const uint32_t new_size = level_array_sizes[new_k];
    level_arrays[new_k][new_size] = addr;
    address_to_index[new_k][addr] = new_size;
    level_array_sizes[new_k]++;

    placewheretowriteaddress = addr;
    return true;
}

bool Nat::addr_free(const uint32_t address)
{
    if (address >= NUMBER_OF_ADDRESSES)
        return false;

    const uint32_t old_k = address_count[address];
    if (old_k == 0)
        return false;

    const uint32_t new_k = old_k - 1;

    // Получаем текущую позицию адреса в массиве уровня
    const uint32_t pos = address_to_index[old_k][address];

    // Получаем последний адрес в старом уровне
    const uint32_t last_pos = level_array_sizes[old_k] - 1;
    const uint32_t last_addr = level_arrays[old_k][last_pos];

    // Заменяем текущий адрес последним
    level_arrays[old_k][pos] = last_addr;
    level_array_sizes[old_k]--;

    // Обновляем индекс последнего адреса
    address_to_index[old_k][last_addr] = pos;

    // Перемещаем адрес в новый уровень
    const uint32_t new_size = level_array_sizes[new_k];
    level_arrays[new_k][new_size] = address;
    address_to_index[new_k][address] = new_size;
    level_array_sizes[new_k]++;

    address_count[address] = new_k;

    // Обновляем first_non_full_level, если нужно
    if (new_k < first_non_full_level)
        first_non_full_level = new_k;

    return true;
}