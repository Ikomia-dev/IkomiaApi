#ifndef CQUEUE_HPP
#define CQUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class CQueue
{
    public:

        struct cancelled {};

        T       pop()
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            while (queue_.empty())
            {
                if (cancelled_)
                    throw cancelled();

                if(cond_.wait_for(mlock, std::chrono::milliseconds(timeout_)) == std::cv_status::timeout)
                    throw cancelled();

                if (cancelled_)
                    throw cancelled();
            }
            auto item = queue_.front();
            queue_.pop();
            return item;
        }
        void    pop(T& item)
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            while (queue_.empty())
            {
                if (cancelled_)
                    throw cancelled();

                if(cond_.wait_for(mlock, std::chrono::milliseconds(timeout_)) == std::cv_status::timeout)
                    throw cancelled();

                if (cancelled_)
                    throw cancelled();
            }
            item = queue_.front();
            queue_.pop();
        }
        void    push(T const& item)
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            queue_.push(item);
            mlock.unlock();
            cond_.notify_one();
        }
        void    push(T&& item)
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            queue_.push(std::move(item));
            mlock.unlock();
            cond_.notify_one();
        }

        size_t  size() const
        {
            return queue_.size();;
        }
        void    clear()
        {
            std::queue<T> empty;
            std::unique_lock<std::mutex> mlock(mutex_);
            std::swap( queue_, empty );
            mlock.unlock();
            cond_.notify_one();
        }
        void    cancel()
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            cancelled_ = true;
            cond_.notify_all();
        }
        void    activate()
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            cancelled_ = false;
            cond_.notify_all();
        }

    private:

        std::queue<T>           queue_;
        std::mutex              mutex_;
        std::condition_variable cond_;
        bool                    cancelled_ = false;
        const int               timeout_ = 5000; // In milliseconds
};

#endif // CQUEUE_HPP
