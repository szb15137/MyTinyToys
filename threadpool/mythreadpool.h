#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <queue>
#include <future>
#include <type_traits>
#include <stdexcept>

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
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
};

template <class F, class... Args>
auto Mythreadpool::enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
    // typedef typename std::result_of<F(Args...)>::type return_type;
    using return_type = typename std::result_of<F(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (stop){
            throw std::runtime_error("Enqueue on stopped thread pool");
        }
        tasks.emplace([task](){(*task)();});
    }
    cv.notify_one();
    return res;
}