#pragma once
#include <cstddef>

template <typename K, typename V> // we use templates cause we dont know the value of our keys and values (can be ints, floats, strings, etc...)
class HashTable{
    public:
        HashTable(); // constructor
        ~HashTable(); // destructor

        // Prevents an existing hashtable to be copied into a new one
        // hashtable a = [0, 1, 2]
        // hashtable b = a 
        // c = a
        // is not possible
        HashTable(const HashTable&) = delete; 
        HashTable& operator = (const HashTable&) = delete;

        void put(const K &key, const V &value); // adds/updates a key, value pair in the hashtable, if the max capacity is reached here, we need to rehash and double capacity, key and value are passed by reference cause we are using templates in here
        V get(const K& key) const; // returns a value in the hash table depending on the key
        bool remove(const K &key); // removes a key from the hashtable
        bool isEmpty() const;  // checks if the hashtable is empty       
        int size() const; // returns the size of the array
        int capacity() const; // returns de array capacity
        void clear(); // clears each node inside the link-list (collitions) and also each bucket
    
    private:
        void rehash(); // if the max capacity of the dyanmic array is reach, we need to double it, since hashing is made with '% cap', we need to rehash the table with the new cap variable
    
        struct Node { // this is the structure of each node of our link list, we need a key and value that comply with our template types and a next pointer
            K key;    // that chains nodes with the same bucket, this handles collisions
            V value;
            Node* next;
        };

        Node** buckets; // buckets is an array of pointers, buckets[i] points to the first node of buckets[i] linked-list
        std::size_t cap; // max capacity of the buckets (we use size_t so we dont have to cast when hashing)
        int count; // total count of keys

        int hashing(const K &key) const; // hashes the key using the 'std::hash' method
};

#include "hash_table.tpp" // template definitions must be visible wherever the template is used
