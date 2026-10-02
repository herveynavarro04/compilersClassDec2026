#pragma once // specifies that this file will only be linked once during the whole compilation

class Stack {
    public:
        Stack(); // constructor
        ~Stack(); // destructor

        // Prevents an existing stack to be copied into a new one
        // Stack a = [0, 1, 2]
        // Stack b = a 
        // c = a
        // is not possible
        Stack(const Stack&) = delete; 
        Stack& operator = (const Stack&) = delete;
        
        void push(int value); // adds new values to the top of the dynamic array O(1)
        int pop(); // removes and returns the last inserted value on the array O(1)
        int peek() const; // returns the last
        bool isEmpty() const;  // checks if the stack is empty       
        int size() const; // returns the size of the array
        int capacity() const; // returns de array capacity

    private:
        void growth(); // increments the size of the dynamic array
        int* data; // pointer to the array in memory
        int count; // elements currently stored
        int cap; // size of the allocated array

};


