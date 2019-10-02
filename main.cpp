#include <iostream>
#include <random>
#include <cassert>
#include <unordered_map>

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
    // We will generate a set of random numbers for keys and values

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, maxSize);

    std::unordered_map<size_t, size_t> keysAndVals;

    for (size_t i = 0; i < maxSize; ++i) {
        const size_t newKey = dist(rng);
        const size_t newVal = dist(rng);
        hbst.insertValue(newKey, newVal);
        keysAndVals[newKey] = newVal;
    }

    const size_t numUniqueKeys = keysAndVals.size();

    std::cout << "Inserted keys: " << numUniqueKeys << "\n";

    hbst.printTree();

    // 2. Search

    for (const auto& kv : keysAndVals) {
        size_t val = 0;
        bool found = hbst.searchValue(kv.first, val);
        // Check that all inserted keys are presented in the tree
        assert (found && val == kv.second);
    }

    // 3. Delete half of the keys

    size_t i = 0;
    std::vector<size_t> deletedKeys;

    for (const auto& kv : keysAndVals) {
        if (i == numUniqueKeys / 2) {
            break;
        }

        hbst.deleteValue(kv.first);
        size_t val = 0;
        bool found = hbst.searchValue(kv.first, val);
        // Check that key was actually deleted
        assert (!found);

        deletedKeys.push_back(kv.first);
        ++i;
    }

    std::cout << "Deleted keys: " << deletedKeys.size() << "\n";

    for (const auto& deletedKey : deletedKeys) {
        keysAndVals.erase(deletedKey);
    }

    std::cout << "Keys exist after deletion: " << keysAndVals.size() << "\n";

    // Check that the rest of the keys are still presented in the tree
    for (const auto& kv : keysAndVals) {
        size_t val = 0;
        bool found = hbst.searchValue(kv.first, val);
        assert (!found || val == kv.second);
    }

    hbst.printTree();

    return 0;
}