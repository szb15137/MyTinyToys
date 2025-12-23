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
                    std::unique_lock<std::mutex> lock(this->mtx);
                    this->cv.wait(lock, [this]{
                        return stop || !this->tasks.empty(); 
                    });
                    if (stop && this->tasks.empty()){
                        return;
                    }
                    task = this->tasks.front();
                    this->tasks.pop();
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