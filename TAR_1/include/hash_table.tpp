#pragma once
#include <cstddef>
#include <functional>
#include <stdexcept>
#include "hash_table.hpp"

template <typename K, typename V>
HashTable<K, V>::HashTable() {
    cap = 8;
    count = 0;
    buckets = new Node*[cap];
    for (std::size_t i = 0; i < cap; i++) {
        buckets[i] = nullptr;          
    }
}

template <typename K, typename V>
HashTable<K, V>::~HashTable() {
    clear();                           // delete every node
    delete[] buckets;                  // delete the array of heads
}

template <typename K, typename V>
int HashTable<K, V>::hashing(const K& key) const {
    std::size_t h = std::hash<K>{}(key);
    return static_cast<int>(h % static_cast<std::size_t>(cap));
}

template <typename K, typename V>
void HashTable<K, V>::put(const K& key, const V& value) {
    int index = hashing(key);

    // Key already exists: update its value
    Node* node = buckets[index];
    while (node != nullptr) {
        if (node->key == key) {
            node->value = value;
            return;
        }
        node = node->next;
    }

    // New key: insert at the front of the bucket's list
    Node* newNode = new Node{key, value, buckets[index]};
    buckets[index] = newNode;
    count++;

    // Too full: grow and rehash
    if (static_cast<std::size_t>(count) * 4 > cap * 3) {
        rehash();
    }
}

template <typename K, typename V>
V HashTable<K, V>::get(const K& key) const {
    Node* node = buckets[hashing(key)];
    while (node != nullptr) {
        if (node->key == key) {
            return node->value;
        }
        node = node->next;
    }
    throw std::runtime_error("HashTable::get: key not found");
}

template <typename K, typename V>
bool HashTable<K, V>::remove(const K& key) {
    int index = hashing(key);
    Node* node = buckets[index];
    Node* previous = nullptr;

    while (node != nullptr) {
        if (node->key == key) {
            if (previous == nullptr) {
                buckets[index] = node->next;   // removing the first node
            } else {
                previous->next = node->next;   // skip over the node
            }
            delete node;
            count--;
            return true;
        }
        previous = node;
        node = node->next;
    }
    return false;                              // key wasn't there
}



template <typename K, typename V>
int HashTable<K, V>::size() const {
    return count;
}

template <typename K, typename V>
bool HashTable<K, V>::isEmpty() const {
    return count == 0;
}

template <typename K, typename V>
int HashTable<K, V>::capacity() const {
    return cap;
}

template <typename K, typename V>
void HashTable<K, V>::clear() {
    for (std::size_t i = 0; i < cap; i++) {
        Node* node = buckets[i];
        while (node != nullptr) {
            Node* nextNode = node->next;   // save before deleting
            delete node;
            node = nextNode;
        }
        buckets[i] = nullptr;
    }
    count = 0;
}

template <typename K, typename V>
void HashTable<K, V>::rehash() {
    Node** oldBuckets = buckets;
    std::size_t oldCap = cap;

    cap = cap * 2;
    buckets = new Node*[cap];
    for (std::size_t i = 0; i < cap; i++) {
        buckets[i] = nullptr;
    }

    // Move every node to its new bucket (indexes change because cap changed)
    for (std::size_t i = 0; i < oldCap; i++) {
        Node* node = oldBuckets[i];
        while (node != nullptr) {
            Node* nextNode = node->next;   // save before relinking
            int index = hashing(node->key);
            node->next = buckets[index];
            buckets[index] = node;
            node = nextNode;
        }
    }

    delete[] oldBuckets;                   // only the array; nodes were reused
}