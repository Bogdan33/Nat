//
// Created by Богдан on 31.05.2025.
//

#include "../inc/Nat.h"

Nat::Nat(uint32_t num_addresses, uint32_t max_per_address)
     : NUMBER_OF_ADDRESSES(num_addresses),
       MAX_NUMBER_ABONENT_PER_ADDRESS(max_per_address)
{

    if (num_addresses == 0 || max_per_address == 0)
        throw;

    address_count = new uint32_t[num_addresses];

    const uint32_t ARRAY_COUNT = max_per_address + 1;
    array = new uint32_t*[ARRAY_COUNT];
    array_size = new uint32_t[ARRAY_COUNT];
    array_head = new uint32_t[ARRAY_COUNT];

    for (uint32_t k = 0; k < ARRAY_COUNT; ++k) {
        array[k] = new uint32_t[num_addresses]; // В каждом масиве может быть максимум N адресов
        array_size[k] = 0;
        array_head[k] = 0;
    }

    // Инициализируем mas 0 (нулевая нагрузка)
    for (uint32_t i = 0; i < num_addresses; ++i) {
        array[0][i] = i;
        address_count[i] = 0;
    }
    array_size[0] = num_addresses;
    first_non_full_array = 0;
}

Nat::~Nat()
{
    const uint32_t ARRAY_COUNT = MAX_NUMBER_ABONENT_PER_ADDRESS + 1;

    for (uint32_t k = 0; k < ARRAY_COUNT; ++k)
        delete[] array[k];

    delete[] array;
    delete[] array_size;
    delete[] array_head;
    delete[] address_count;
}

bool Nat::addr_alloc(uint32_t& placewheretowriteaddress)
{
    if (first_non_full_array >= MAX_NUMBER_ABONENT_PER_ADDRESS)
        return false;

    uint32_t k = first_non_full_array;

    // Берём первый элемент из нужного масива
    const uint32_t addr = array[k][array_head[k]];
    array_head[k]++;

    // Если этот масив опустел, ищем следующий непустой
    if (array_head[k] >= array_size[k])
    {
        do {
            ++k;
        } while (k <= MAX_NUMBER_ABONENT_PER_ADDRESS && array_head[k] >= array_size[k]);

        first_non_full_array = (k <= MAX_NUMBER_ABONENT_PER_ADDRESS) ? k : MAX_NUMBER_ABONENT_PER_ADDRESS + 1;
    }

    // Перемещаем адрес в следующий масив
    const uint32_t new_k = address_count[addr] + 1;
    address_count[addr] = new_k;

    uint32_t& new_head = array_head[new_k];
    uint32_t& new_size = array_size[new_k];

    array[new_k][new_size] = addr;
    new_size++;

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

    // Удаляем из старого масива (ищем адрес)
    uint32_t* old_array = array[old_k];
    const uint32_t old_head = array_head[old_k];
    const uint32_t old_size = array_size[old_k];

    bool found = false;
    for (uint32_t i = old_head; i < old_size; ++i)
    {
        if (old_array[i] == address)
        {
            // Сдвигаем последний элемент на место найденного
            old_array[i] = old_array[old_size - 1];
            array_size[old_k]--;
            found = true;
            break;
        }
    }

    if (!found)
        return false;

    // Добавляем в новый масив
    uint32_t& new_head = array_head[new_k];
    uint32_t& new_size = array_size[new_k];

    array[new_k][new_size] = address;
    new_size++;

    address_count[address] = new_k;

    // Обновляем first_non_full_mas, если нужно
    if (new_k < first_non_full_array)
        first_non_full_array = new_k;

    return true;
}