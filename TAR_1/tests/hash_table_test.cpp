#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include "hash_table.hpp"

void testNewHashTableIsEmpty(){
    std::cout << "=== Test 1: Validate that the hash table is empty ===\n";
    HashTable<std::string, int> table;

    std::cout << "Is the hash table empty? " << std::boolalpha << table.isEmpty() << "\n";
    assert(table.isEmpty());
    std::cout << "Hash table size: " << table.size() << "\n";
    assert(table.size() == 0);
    std::cout << "PASSED\n\n";
}

void testPutAndGet(){
    std::cout << "=== Test 2: Validate that we can put key-value pairs and get them back ===\n";
    HashTable<std::string, int> table;
    table.put("one", 1);
    table.put("two", 2);
    table.put("three", 3);

    std::cout << "Pairs (one, 1), (two, 2) and (three, 3) put into the hash table\n";
    std::cout << "Hash table current size: " << table.size() << "\n";
    assert(table.size() == 3);

    int one = table.get("one");
    std::cout << "Get 'one': " << one << "\n";
    assert(one == 1);

    int two = table.get("two");
    std::cout << "Get 'two': " << two << "\n";
    assert(two == 2);

    int three = table.get("three");
    std::cout << "Get 'three': " << three << "\n";
    assert(three == 3);

    std::cout << "PASSED\n\n";
}

void testUpdateAndRemove(){
    std::cout << "=== Test 3: Validate that put updates existing keys and remove deletes them ===\n";
    HashTable<std::string, int> table;

    table.put("apple", 10);
    table.put("banana", 20);
    std::cout << "Pairs (apple, 10) and (banana, 20) put into the hash table\n";

    table.put("apple", 99);
    std::cout << "Updated 'apple' to 99\n";
    std::cout << "Get 'apple': " << table.get("apple") << "\n";
    assert(table.get("apple") == 99);
    std::cout << "Hash table size (should not change on update): " << table.size() << "\n";
    assert(table.size() == 2);

    bool removed = table.remove("apple");
    std::cout << "Removed 'apple'? " << std::boolalpha << removed << "\n";
    assert(removed);
    std::cout << "Hash table size after remove: " << table.size() << "\n";
    assert(table.size() == 1);

    std::cout << "Get 'banana' is still there: " << table.get("banana") << "\n";
    assert(table.get("banana") == 20);

    std::cout << "PASSED\n\n";
}

void testMissingKeyErrors(){
    std::cout << "=== Test 4: Validate get on a missing key throws error and remove returns false ===\n";
    HashTable<std::string, int> table;
    std::cout << "Is the hash table empty? " << std::boolalpha << table.isEmpty() << "\n";

    bool getError = false;
    std::cout << "Trying to get a missing key\n";

    try
    {
        table.get("ghost");
    }
    catch(const std::runtime_error& e)
    {
        std::cout << "Error caught: " << e.what() << "\n";
        getError = true;
    }
    assert(getError);

    std::cout << "Trying to remove a missing key\n";
    bool removed = table.remove("ghost");
    std::cout << "Removed 'ghost'? " << std::boolalpha << removed << "\n";
    assert(!removed);

    std::cout << "PASSED\n\n";
}

void testManyElements(){
    std::cout << "=== Test 5: Validate many elements to test rehashing and clear ===\n";
    HashTable<int, int> table;
    std::cout << "Hash table size: " << table.size() << "\n";
    std::cout << "Hash table initial capacity: " << table.capacity() << "\n";
    int initialCapacity = table.capacity();

    for (int i = 0; i < 1000; i++){
        table.put(i, i * 10);
    }
    std::cout << "Put keys from 0 to 999 (1000 elements) with value key * 10 into the hash table\n";
    assert(table.size() == 1000);
    std::cout << "Hash table size after put operations: " << table.size() << "\n";
    std::cout << "Hash table capacity after rehashing: " << table.capacity() << "\n";
    assert(table.capacity() > initialCapacity);

    for (int i = 0; i < 1000; i++){
        assert(table.get(i) == i * 10);
    }
    std::cout << "Get all the keys and validate their values after rehashing\n";

    table.clear();
    assert(table.isEmpty());
    std::cout << "Cleared the hash table\n";
    std::cout << "Is hash table empty? " << std::boolalpha << table.isEmpty() << "\n";

    std::cout << "PASSED\n\n";
}


int main(){
    testNewHashTableIsEmpty();
    testPutAndGet();
    testUpdateAndRemove();
    testMissingKeyErrors();
    testManyElements();
    return 0;
}
