#include <queue>
#include <mutex>
#include <condition_variable>
template <class T>
class Cola {
    std::mutex m;
    std::mutex queue_mutex;
    std::queue<T> cola;
    std::condition_variable var;
    public:
    void encolar(T& t){
        std::unique_lock<std::mutex> uniqueLock(this->m);
        this->cola.push(t);
        this->var.notify_all();
    }

    T desencolar(){
        std::unique_lock<std::mutex> uniqueLock(this->m);
        while (this->cola.empty()){
        this->var.wait(uniqueLock);
        }
        this->queue_mutex.lock();
        T t_desencolado = this->cola.front();
        this->cola.pop();
        this->queue_mutex.unlock();
        return std::move(t_desencolado);
    }


};
