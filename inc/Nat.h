//
// Created by Богдан on 31.05.2025.
//

#ifndef NAT_H
#define NAT_H

#include <memory>

class Nat
{
public:
    Nat(uint32_t NUMBER_OF_ADDRESSES, uint32_t MAX_NUMBER_ABONENT_PER_ADDRESS);
    ~Nat() = default;
    // ADDR - uint32_t [0..NUMBER_OF_ADDRESS-1]

    Nat(const Nat&) = delete;
    Nat& operator=(const Nat&) = delete;

    bool addr_alloc(uint32_t& placewheretowriteaddress);
    // true - если удачно выделила адресса
    // false - если нет свободных адрессов (все адресса уже содержат максимальное количество абонентов)

    bool addr_free(uint32_t address);
    // true - если успешно удалила
    // false - если адресс >= NUMBER_OF_ADDRESSES
    // false - если адреесс который мы возвращаем уже пустой (на нем нет ни одного абонента)

private:
    uint32_t NUMBER_OF_ADDRESSES;
    uint32_t MAX_NUMBER_ABONENT_PER_ADDRESS;

    std::unique_ptr<uint32_t[]> address_count;            // count[addr] — сколько абонентов на адресе
    std::unique_ptr<uint32_t*[]> level_arrays;           // level_arrays[k] — массив адресов с k абонентами
    std::unique_ptr<uint32_t*[]> address_to_index;       // address_to_index[k][addr] — индекс addr в level_arrays[k]
    std::unique_ptr<uint32_t[]> level_array_sizes;       // level_array_sizes[k] — сколько всего элементов в level_arrays[k]
    std::unique_ptr<uint32_t[]> level_array_heads;       // level_array_heads[k] — указатель на первый доступный адрес
    uint32_t first_non_full_level;                       // Уровень, где есть свободные адреса

};

#endif //NAT_H
