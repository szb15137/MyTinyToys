#include "mmpool.h"

MyMmPool::MyMmPool(int num, int size) : block_num(num), free_list(nullptr) {
    pool_size = size > sizeof(void*) ? size : sizeof(void*);
    pool_size = align_size(pool_size, sizeof(void*));
    expand();
}
MyMmPool::~MyMmPool() {
    for (char* pool : mm_pool_list){
        ::operator delete(pool);
    }
}

void MyMmPool::expand(){
    // Will only be called when free_list is empty
    char* new_pool = static_cast<char*>(::operator new (pool_size * block_num));
    mm_pool_list.push_back(new_pool);
    for (int i = 0; i < block_num; i++){
        PoolNode* ptr = reinterpret_cast<PoolNode*>(new_pool);
        ptr -> next = free_list;
        free_list = ptr;
        new_pool += pool_size;
    }
}

void* MyMmPool::allocate() {
    //
    if (!free_list){
        expand();
    }
    void* node = static_cast<void*>(free_list);
    free_list = free_list -> next;
    return node;
}

void MyMmPool::deallocate(void* ptr) {
    //
    PoolNode* node = static_cast<PoolNode*>(ptr);
    node -> next = free_list;
    free_list = node;
}

inline int MyMmPool::align_size(int size, int alignment){
    return (size + (alignment - 1)) & ~ (alignment - 1);
}