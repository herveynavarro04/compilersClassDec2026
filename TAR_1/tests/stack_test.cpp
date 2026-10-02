#include <cassert>
#include <iostream>
#include <stdexcept>
#include "stack.hpp"

void testNewStackIsEmpty(){
    std::cout << "=== Test 1: Validate that the stack is empty ===\n";
    Stack stack;

    std::cout << "Is the stack empty? " << std::boolalpha << stack.isEmpty() << "\n";
    assert(stack.isEmpty());    
    std::cout << "PASSED\n\n";
}

void testPushAndPeek(){
    std::cout << "=== Test 2: Validate that we can push and peek the inserted element ===\n";
    Stack stack;
    stack.push(10);
    stack.push(20);
    stack.push(30);

    std::cout << "Values 10, 20 and 30 pushed into the stack\n";
    std::cout << "Stack current size: " << stack.size() << "\n";
    assert(stack.size() == 3);

    int top = stack.peek();
    std::cout << "Peek: " << top << "\n";
    assert(stack.peek() == 30);

    std::cout << "PASSED\n\n";
}

void testLifoOrder(){
    std::cout << "=== Test 3: Validate that the LIFO order works with the pop method ===\n";
    Stack stack;

    stack.push(10);
    stack.push(20);
    stack.push(30);
    std::cout << "Values 10, 20 and 30 pushed into the stack\n";

    int firstPop = stack.pop();
    std::cout << "First pop: " << firstPop << "\n";
    assert(firstPop == 30);

    int secondPop = stack.pop();
    std::cout << "Second pop: " << secondPop << "\n";
    assert(secondPop == 20);

    int thirdPop = stack.pop();
    std::cout << "Third pop: " << thirdPop << "\n";
    assert(thirdPop == 10);

    std::cout << "PASSED\n\n";
}

void testEmptyStackErrors(){
    std::cout << "=== Test 4: Validate pop and peek on an empty stack throws error ===\n";
    Stack stack;
    std::cout << "Is the stack empty? " << std::boolalpha << stack.isEmpty() << "\n";

    bool popError = false;
    std::cout << "Trying to pop on empty stack\n";

    try
    {
        stack.pop();
    }
    catch(const std::runtime_error& e)
    {
        std::cout << "Error caught: " << e.what() << "\n";
        popError = true;
    }
    assert(popError);

    bool peekError = false;
    std::cout << "Trying to peek on empty stack\n";

    try
    {
        stack.peek();
    }
    catch(const std::runtime_error& e)
    {
        std::cout << "Error caught: " << e.what() << "\n";
        peekError = true;
    }
    assert(peekError);

    std::cout << "PASSED\n\n";
}

void testManyElements(){
    std::cout << "=== Test 5: Validate many elements to test dynamic array growth ===\n";
    Stack stack;
    std::cout << "Stack size: " << stack.size() << "\n";
   
    for (int i = 0; i < 1000; i++){
        stack.push(i);
    }
    std::cout << "Pushed elements from 0 to 999 (1000 elements) to the stack\n";
    assert(stack.size() == 1000);
    std::cout << "Stack size after push operations: " << stack.size() << "\n";

    for (int i = 999; i >= 0; i--){
        assert(stack.pop() == i);
    }
    assert(stack.isEmpty());
    std::cout << "Pop all the elements in the stack in LIFO order\n";
    std::cout << "Is stack empty? " << std::boolalpha << stack.isEmpty() << "\n";

    std::cout << "PASSED\n\n";
}


int main(){
    testNewStackIsEmpty();
    testPushAndPeek();
    testLifoOrder();
    testEmptyStackErrors();        
    testManyElements();
    return 0;
}