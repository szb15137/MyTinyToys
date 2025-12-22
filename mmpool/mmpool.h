#include <vector>

struct PoolNode
{
    /* data */
    PoolNode *next;
};
class MyMmPool
{
public:
    explicit MyMmPool(int num, int size = 4096);
    ~MyMmPool();
    MyMmPool(const MyMmPool &other) = delete;
    MyMmPool(MyMmPool &&other) = delete;
    MyMmPool &operator=(const MyMmPool &other) = delete;
    MyMmPool &operator=(MyMmPool &&other) = delete;
    void *allocate();
    void deallocate(void* ptr);

private:
    static inline int align_size(int size, int alignment);
    void expand();

private:
    int pool_size;
    int block_num;
    PoolNode* free_list;
    std::vector<char*> mm_pool_list;
};