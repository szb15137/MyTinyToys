#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <queue>

class Mythreadpool
{
private:
    /* data */
    std::vector<std::thread> workers;
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<std::function<void()>> tasks;
    bool stop = false;
public:
    Mythreadpool(int num_threads = 4);
    ~Mythreadpool();
    Mythreadpool(const Mythreadpool& other) = delete;
    Mythreadpool& operator=(const Mythreadpool& other) = delete;
    Mythreadpool(const Mythreadpool&& other) = delete;
    Mythreadpool& operator=(Mythreadpool&& other) = delete;

    // TODO: Update the class so that result can be returned from the thread
    template <class F, class... Args>
    void enqueue(F&& f, Args&&... args);
};

template <class F, class... Args>
void Mythreadpool::enqueue(F&& f, Args&&... args){
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (stop){
            return;
        }
        tasks.emplace(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    }
    cv.notify_one();
}