#include <mutex>
#include <condition_variable>
#include <deque>

template <typename T>
class BlockingQueue
{
private:
    std::mutex              mutex;
    std::condition_variable condition;
    std::deque<T>           queue;
public:

    std::deque<T> getDeque() { return queue; }

    void push(T const& value) {
        {
            std::unique_lock<std::mutex> lock(this->mutex);
            queue.push_front(value);
        }
        this->condition.notify_one();
    }
    T pop() {
        std::unique_lock<std::mutex> lock(this->mutex);
        this->condition.wait(lock, [=] { return !this->queue.empty(); });
        T rc(std::move(this->queue.back()));
        this->queue.pop_back();
        return rc;
    }

   /* bool empty() {
        std::unique_lock<std::mutex> lock(this->d_mutex);
        bool check = this->d_queue.empty();
        return check;
    }*/
};