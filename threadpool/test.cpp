#include "mythreadpool.h"
#include <iostream>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>

void printHello(int i) {
    std::cout << "Hello from task " << i << std::endl;
}

int main(){
    Mythreadpool pool;
    for (int i = 0; i < 10; i++)
    {
        pool.enqueue(printHello, i);
        pool.enqueue([i](){
            std::cout << "Hello from lambda task " << i << std::endl;
        });
    }
}