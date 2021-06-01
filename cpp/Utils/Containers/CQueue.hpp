// Copyright (C) 2021 Ikomia SAS
// Contact: https://www.ikomia.com
//
// This file is part of the Ikomia API libraries.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef CQUEUE_HPP
#define CQUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include "CException.h"

template <typename T>
class CQueue
{
    public:

        T       pop()
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            while (queue_.empty())
            {
                if (cancelled_)
                    throw CException(CoreExCode::PROCESS_CANCELLED, "Threaded queue cancelled.");

                if(cond_.wait_for(mlock, std::chrono::milliseconds(timeout_)) == std::cv_status::timeout)
                    throw CException(CoreExCode::TIMEOUT_REACHED, "Threaded queue timeout.");

                if (cancelled_)
                    throw CException(CoreExCode::PROCESS_CANCELLED, "Threaded queue cancelled");
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
                    throw CException(CoreExCode::PROCESS_CANCELLED, "Threaded queue cancelled.");

                if(cond_.wait_for(mlock, std::chrono::milliseconds(timeout_)) == std::cv_status::timeout)
                    throw CException(CoreExCode::TIMEOUT_REACHED, "Threaded queue timeout.");

                if (cancelled_)
                    throw CException(CoreExCode::PROCESS_CANCELLED, "Threaded queue cancelled.");
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
        void    setTimeout(int ms)
        {
            timeout_ = ms;
        }

    private:

        std::queue<T>           queue_;
        std::mutex              mutex_;
        std::condition_variable cond_;
        bool                    cancelled_ = false;
        int                     timeout_ = 5000; // In milliseconds
};

#endif // CQUEUE_HPP
