#include "hash.h"

using hash_array = std::pair<   std::unordered_set<uint64_t> *, 
                                const hash_function_t> *;
using hash_array_map = std::unordered_map<unsigned long, hash_array>;

unsigned long next_usable_identifier = 0;
hash_array_map all_arrays;

// Tworzy tablicę haszującą i zwraca jej identyfikator. Parametr
// hash_function jest wskaźnikiem na funkcję haszującą, która daje w wyniku
// liczbę uint64_t i ma kolejno parametry uint64_t const * oraz size_t.

unsigned long hash_create(hash_function_t hash_function) {
    std::unordered_set<uint64_t> array;
    const hash_function_t function_copy = hash_function;
    std::pair<  std::unordered_set<uint64_t> *, 
                const hash_function_t> array_with_function = 
                {&array, hash_function};
    all_arrays.insert({next_usable_identifier, &array_with_function});
    return next_usable_identifier++;
}

// Usuwa tablicę haszującą o identyfikatorze id, o ile ona istnieje.
// W przeciwnym przypadku nic nie robi.

void hash_delete(unsigned long id) {
    all_arrays.erase(id);
}

// Daje liczbę ciągów przechowywanych w tablicy haszującej
// o identyfikatorze id lub 0, jeśli taka tablica nie istnieje.

size_t hash_size(unsigned long id) {
    try {
        return (*((*(all_arrays.at(id))).first)).size();
    } catch (std::out_of_range) {
        return 0;
    }
}

// Wstawia do tablicy haszującej o identyfikatorze id ciąg liczb
// całkowitych seq o długości size. Wynikiem jest informacja, czy operacja
// się powiodła. Operacja się nie powiedzie, jeśli nie ma takiej tablicy
// haszującej, jeśli tablica haszująca zawiera już taki ciąg, jeśli
// parametr seq ma wartość NULL lub parametr size ma wartość 0.

bool hash_insert(unsigned long id, uint64_t const * seq, size_t size) {
    if (seq == NULL || size == 0) {
        return false;
    }
    try {
        hash_array to_insert_to = all_arrays.at(id);
        uint64_t hash = (*((*to_insert_to).second))(seq, size);
        return ((*((*to_insert_to).first)).insert(hash)).second;
    } catch (std::out_of_range) {
        return false;
    }
}

// Usuwa z tablicy haszującej o identyfikatorze id ciąg liczb całkowitych
// seq o długości size. Wynikiem jest informacja, czy operacja się
// powiodła. Operacja się nie powiedzie, jeśli nie ma takiej tablicy
// haszującej, jeśli tablica haszująca nie zawiera takiego ciągu,
// jeśli parametr seq ma wartość NULL lub parametr size ma wartość 0.

bool hash_remove(unsigned long id, uint64_t const * seq, size_t size) {
    if (seq == NULL || size == 0) {
        return false;
    }
    try {
        hash_array to_insert_to = all_arrays.at(id);
        uint64_t hash = (*((*to_insert_to).second))(seq, size);
        return ((*((*to_insert_to).first)).erase(hash));
    } catch (std::out_of_range) {
        return false;
    }
}

// Jeśli tablica haszująca o identyfikatorze id istnieje i nie jest pusta,
// to usuwa z niej wszystkie elementy. W przeciwnym przypadku nic nie robi.

void hash_clear(unsigned long id) {
    try {
        hash_array to_insert_to = all_arrays.at(id);
        ((*((*to_insert_to).first)).clear());
    } catch (std::out_of_range) {

    }
}

// Daje wynik true, jeśli istnieje tablica haszująca o identyfikatorze id
// i zawiera ona ciąg liczb całkowitych seq o długości size. Daje wynik
// false w przeciwnym przypadku oraz gdy parametr seq ma wartość NULL lub
// parametr size ma wartość 0.

bool hash_test(unsigned long id, uint64_t const * seq, size_t size) {
    if (seq == NULL || size == 0) {
        return false;
    }
    try {
        hash_array to_insert_to = all_arrays.at(id);
        uint64_t hash = (*((*to_insert_to).second))(seq, size);
        return (
                (*((*to_insert_to).first)).find(hash) != 
                (*((*to_insert_to).first)).end()
            );
    } catch (std::out_of_range) {
        return false;
    }
}