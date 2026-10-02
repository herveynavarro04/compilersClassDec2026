#pragma once // specifies that this file will only be linked once during the whole compilation

class Queue{
    public: 
        Queue(); // constructor
        ~Queue(); // destructor

        // Prevents an existing Queue to be copied into a new one
        // Queue a = [0, 1, 2]
        // Queue b = a 
        // c = a
        // is not possible        
        Queue(const Queue&) = delete;
        Queue& operator = (const Queue&) = delete;

        void enqueue(int value); // adds a new value to the top of the array and also adds space if its full
        int dequeue(); // removes and returns the first element of the array 
        int front() const; // returns the front element of the array without removing it        
        bool isEmpty() const;  // checks if the Queue is empty       
        int size() const; // returns the size of the array
        int capacity() const; // returns de array capacity        

    private:
        void growth(); // increments the size of the dynamic array
        int* data; // pointer to the array in memory
        int count; // elements currently stored
        int cap; // size of the allocated array
};