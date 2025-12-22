#include "mythreadpool.h"
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>

Mythreadpool::Mythreadpool(int num_threads)
{
    for (int i = 0; i < num_threads; i++){
        workers.emplace_back(std::thread([this](){
            while (true){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this]{
                        return stop || !tasks.empty(); 
                    });
                    if (stop && tasks.empty()){
                        return;
                    }
                    task = tasks.front();
                    tasks.pop();
                    // lock.unlock();
                }
                task();
            }
        }));
    }
}

Mythreadpool::~Mythreadpool()
{
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
    }
    cv.notify_all();
    for (auto& worker : workers){
        if (worker.joinable()){
            worker.join();
        }
    }
}