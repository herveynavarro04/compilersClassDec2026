#include <cassert>
#include <iostream>
#include <stdexcept>
#include "queue.hpp"

void testNewQueueIsEmpty(){
    std::cout << "=== Test 1: Validate that the queue is empty ===\n";
    Queue queue;

    std::cout << "Is the queue empty? " << std::boolalpha << queue.isEmpty() << "\n";
    assert(queue.isEmpty());
    std::cout << "PASSED\n\n";
}

void testEnqueueAndFront(){
    std::cout << "=== Test 2: Validate that we can enqueue and see the front element ===\n";
    Queue queue;
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);

    std::cout << "Values 10, 20 and 30 enqueued into the queue\n";
    std::cout << "Queue current size: " << queue.size() << "\n";
    assert(queue.size() == 3);

    int first = queue.front();
    std::cout << "Front: " << first << "\n";
    assert(queue.front() == 10);

    std::cout << "PASSED\n\n";
}

void testFifoOrder(){
    std::cout << "=== Test 3: Validate that the FIFO order works with the dequeue method ===\n";
    Queue queue;

    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);
    std::cout << "Values 10, 20 and 30 enqueued into the queue\n";

    int firstDequeue = queue.dequeue();
    std::cout << "First dequeue: " << firstDequeue << "\n";
    assert(firstDequeue == 10);

    int secondDequeue = queue.dequeue();
    std::cout << "Second dequeue: " << secondDequeue << "\n";
    assert(secondDequeue == 20);

    int thirdDequeue = queue.dequeue();
    std::cout << "Third dequeue: " << thirdDequeue << "\n";
    assert(thirdDequeue == 30);

    std::cout << "PASSED\n\n";
}

void testEmptyQueueErrors(){
    std::cout << "=== Test 4: Validate dequeue and front on an empty queue throws error ===\n";
    Queue queue;
    std::cout << "Is the queue empty? " << std::boolalpha << queue.isEmpty() << "\n";

    bool dequeueError = false;
    std::cout << "Trying to dequeue on empty queue\n";

    try
    {
        queue.dequeue();
    }
    catch(const std::runtime_error& e)
    {
        std::cout << "Error caught: " << e.what() << "\n";
        dequeueError = true;
    }
    assert(dequeueError);

    bool frontError = false;
    std::cout << "Trying to front on empty queue\n";

    try
    {
        queue.front();
    }
    catch(const std::runtime_error& e)
    {
        std::cout << "Error caught: " << e.what() << "\n";
        frontError = true;
    }
    assert(frontError);

    std::cout << "PASSED\n\n";
}

void testManyElements(){
    std::cout << "=== Test 5: Validate many elements to test dynamic array growth ===\n";
    Queue queue;
    std::cout << "Queue size: " << queue.size() << "\n";

    for (int i = 0; i < 1000; i++){
        queue.enqueue(i);
    }
    std::cout << "Enqueued elements from 0 to 999 (1000 elements) to the queue\n";
    assert(queue.size() == 1000);
    std::cout << "Queue size after enqueue operations: " << queue.size() << "\n";

    for (int i = 0; i < 1000; i++){
        assert(queue.dequeue() == i);
    }
    assert(queue.isEmpty());
    std::cout << "Dequeue all the elements in the queue in FIFO order\n";
    std::cout << "Is queue empty? " << std::boolalpha << queue.isEmpty() << "\n";

    std::cout << "PASSED\n\n";
}


int main(){
    testNewQueueIsEmpty();
    testEnqueueAndFront();
    testFifoOrder();
    testEmptyQueueErrors();
    testManyElements();
    return 0;
}
