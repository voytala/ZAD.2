#include "hash.h"

#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <iostream>

namespace {
    struct VectorEquals {
    public:
    bool operator()( std::pair<const uint64_t *, const size_t> const seq1,
                     std::pair<const uint64_t *, const size_t> const seq2)
                     const {
        if (seq1.second != seq2.second) 
            return false;
        for (size_t it = 0; it < seq1.second; it++) {
            if (seq1.first[it] != seq2.first[it])
                return false;
        }
        return true;
    }
    };

    struct HashingObject {
    public:
    hash_function_t function;
    HashingObject(hash_function_t function) {
        this->function = function;
    }
    uint64_t operator()(  std::pair<
                            const uint64_t *, 
                            const size_t
                            > const seq1)
                        const {
        return function(seq1.first, seq1.second);
    }
    };
}

using hash_array = std::unordered_set<
                    std::pair<const uint64_t *, size_t>,
                    HashingObject, 
                    VectorEquals>;
using hash_array_map = std::unordered_map<unsigned long, hash_array *>;

unsigned long next_usable_identifier = 0;
hash_array_map all_arrays;

// Tworzy tablicę haszującą i zwraca jej identyfikator. Parametr
// hash_function jest wskaźnikiem na funkcję haszującą, która daje w wyniku
// liczbę uint64_t i ma kolejno parametry uint64_t const * oraz size_t.

unsigned long hash_create(hash_function_t hash_function) {
    HashingObject Hashing = HashingObject(hash_function);
    hash_array *array = new hash_array(0, Hashing);
    all_arrays.insert({next_usable_identifier, array});
    next_usable_identifier++;
    return next_usable_identifier - 1;
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
        return ((*(all_arrays.at(id)))).size();
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
        hash_array *to_insert_to = all_arrays.at(id);
        uint64_t * seqcopy = new uint64_t[size];
        for (size_t it = 0; it < size; it++) {
            seqcopy[it] = seq[it];
        }
        return ((*to_insert_to).insert(std::make_pair(seqcopy, size))).second;
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
        hash_array *to_insert_to = all_arrays.at(id);
        return ((*to_insert_to).erase(std::make_pair(seq, size)) == 1);
    } catch (std::out_of_range) {
        return false;
    }
}

// Jeśli tablica haszująca o identyfikatorze id istnieje i nie jest pusta,
// to usuwa z niej wszystkie elementy. W przeciwnym przypadku nic nie robi.

void hash_clear(unsigned long id) {
    try {
        hash_array *to_insert_to = all_arrays.at(id);
        (*to_insert_to).clear();
    } catch (std::out_of_range) {}
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
        hash_array *to_insert_to = all_arrays.at(id);
        return (
                (*to_insert_to).find(std::make_pair(seq, size)) != 
                (*to_insert_to).end()
            );
    } catch (std::out_of_range) {
        return false;
    }
}