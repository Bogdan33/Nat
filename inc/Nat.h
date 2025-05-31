//
// Created by Богдан on 31.05.2025.
//

#ifndef NAT_H
#define NAT_H

#include <cstdint>

class Nat
{
public:
    Nat(uint32_t NUMBER_OF_ADDRESSES, uint32_t MAX_NUMBER_ABONENT_PER_ADDRESS);
    ~Nat();
    // ADDR - uint32_t [0..NUMBER_OF_ADDRESS-1]

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

    uint32_t* address_count;

    uint32_t** array;
    uint32_t* array_size; // сколько элементов в каждом масиве
    uint32_t* array_head; // индекс следующего элемента для извлечения

    uint32_t first_non_full_array; // первый неполный уровень (k), где есть свободные адреса
};



#endif //NAT_H
