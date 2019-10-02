#include <iostream>
#include <random>
#include <chrono>

#include "HBST.hpp"

struct KeyHasher {
    static size_t toSizeT(size_t key) {
        return key;
    }
};

int main() {
    const size_t maxSize = 64;

    HBST<size_t, size_t, KeyHasher> hbst(maxSize);

    // 1. Insertion

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, maxSize);

    std::vector<size_t> keys(maxSize);

    for (size_t i = 0; i < maxSize; ++i) {
        keys[i] = dist(rng);
        hbst.insertValue(keys[i], i);
    }

    hbst.printTree();

    // 2. Search

    for (size_t i = 0; i < maxSize; ++i) {
        size_t val = 0;

        if (hbst.searchValue(keys[i], val)) {
            std::cout << val;
        }
    }

    // 3. Delete

    for (size_t i = 0; i < maxSize / 2; ++i) {
        hbst.deleteValue(keys[i]);
    }

    hbst.printTree();

    return 0;
}