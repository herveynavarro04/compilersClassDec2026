#include "queue.hpp"
#include <stdexcept>

Queue::Queue(){
    cap = 4; // we intialize the capacity at a small number 
    count = 0; // the count starts at 0 cause the array is empty
    data = new int[cap]; // we separate the memory for the capacity for the 4 ints (16 bytes)
}

Queue::~Queue(){
    delete[] data; // destructor to clean the memory when the program stops running
}

void Queue::enqueue(int value){
    if (cap == count){ // if the queue reaches its max capacity, we call the growth function to duplicate its size
        growth();       // before moving on
    }
    data[count] = value; // we assing the new value at the end of the queue
    count++; // increment the count variable because of the new value
}

int Queue::dequeue(){
    if (isEmpty()){
        throw std::runtime_error("Queue underflow: Tried to dequeue on empty queue"); // if the Queue is empty, it calls the runtime 
    }                                                                               // error function (Queue underflow)
    int value = data[0]; // we stored the value in data[0] before rewritting it                                                             
    for (int i = 1; i < count; i++){ // this loop assigns the index value to the previous index up to the lenght of the array
        data[i - 1] = data[i];
    }
    count --; // we reduce the count cause we took out the first element 
    return value; // return the previousley stored value
}

int Queue::front() const{
    if (isEmpty()){
        throw std::runtime_error("Queue is empty: Tried to front on empty queue"); // if the Queue is empty, it calls the runtime 
    }                                                                            // error 
    return data[0]; // return the first value of the array
}

bool Queue::isEmpty() const{ // checks if the array is empty by conditioning the count
    return count == 0;
}

int Queue::size() const{ // returns the size of the array via de count
    return count;
}

int Queue::capacity() const{ // returns de array capacity
    return cap;
}

void Queue::growth(){ // doubles the size of the dynamic array 
    int newCap = cap * 2; // temp variable that dups capacity
    int* newData = new int[newCap]; // separates double of the memory we had before 
    for (int i = 0; i < count; i++){ 
        newData[i] = data[i]; // / copies each memory adress from data into the new data array
    }
    delete[] data; // deletes previous memory used
    data = newData; // inserts the newData array into the previous data array
    cap = newCap; // capacity is now equal to the previous ammount used 
}