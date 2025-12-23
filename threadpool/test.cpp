#include "mythreadpool.h"
#include <iostream>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <future>

void printHello(int i) {
    std::cout << "Hello from task " << i << std::endl;
}

void printHello(char c){
    std::cout << "Hello from char task" << c << std::endl;
}

void printHi(int i) {
    std::cout << "Hi from task " << i << std::endl;
}

int multiplys(int a, int b) {
    return a * b;
}

int main(){
    Mythreadpool pool;
    for (int i = 0; i < 10; i++)
    {
        auto fut1 = pool.enqueue([](char c)
                                 { printHello(c); }, 'A');
        fut1.get();
        auto fut2 = pool.enqueue(printHi, i);
        fut2.get();
        auto fut3 = pool.enqueue([i]()
                                 { std::cout << "Hello from lambda task " << i << std::endl; });
        fut3.get();
        auto fut4 = pool.enqueue(multiplys, i, i + 1);
        std::cout << "Result from multiplys task: " << fut4.get() << std::endl;
    }
}