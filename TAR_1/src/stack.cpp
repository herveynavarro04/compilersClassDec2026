#include "stack.hpp"
#include <stdexcept>

Stack::Stack(){
    cap = 4; // we define the capacity of the dynamic array at 4 to start
    count = 0; // initialized at 0 cause we dont have any values yet
    data = new int[cap]; // we reserve space for 4 ints = 16 bytes
};

Stack::~Stack(){
    delete[] data; // we implement the desctuctor to clear memory when the program stops running
}

void Stack::push(int value){ // adds a new value to the last place of the array
    if (count == cap) { // if the current count is greater than the capacity of the array, we call the growth function to dup its size
        growth();
    }
    data[count] = value; // we insert the value into the last index, which, will always be equal to the latest count
    count++; // we increment count by one unit
}

int Stack::pop(){
    if (isEmpty()){
        throw std::runtime_error("Stack underflow: Tried to pop on empty stack"); // if the stack is empty, it calls the runtime 
    }                                                                            // error function (stack underflow)
    count--; // here count is doing two jobs, first it reduces the index by one, so it will return the last 
            // valid index (from 0 to 1), also count is giving us the actual size of the array after "removing" the last element   
            // we are not really remvoing the last elemnt, just getting it out of the stack, this elemnt will be rewrited in memory on
            // the next push operaton
    return data[count];
}

int Stack::peek() const{
    if(isEmpty()){
        throw std::runtime_error("Stack is empty: Tried to peek on empty stack"); // if the stack is empty, we cant peek and we return
    }                                                                             // error 
    return data[count - 1]; // we look for the current count - 1
}

bool Stack::isEmpty() const{ // checks if the array is empty by conditioning the count
    return count == 0;
}

int Stack::size() const{ // returns the size of the array via de count
    return count;
}

int Stack::capacity() const{ // returns de array capacity
    return cap;
}

void Stack::growth(){ // doubles the size of the dynamic array 
    int newCap = cap * 2; // temp variable that dups capacity
    int* newData = new int[newCap]; // separates double of the memory we had before 
    for (int i = 0; i < count; i++){ 
        newData[i] = data[i]; // / copies each memory adress from data into the new data array
    }
    delete[] data; // deletes previous memory used
    data = newData; // inserts the newData array into the previous data array
    cap = newCap; // capacity is now equal to the previous ammount used 
}