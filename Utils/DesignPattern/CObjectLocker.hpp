#ifndef COBJECTLOCKER_H
#define COBJECTLOCKER_H


template<typename ResourceObj>
class CObjectLocker
{
    public:

        CObjectLocker(ResourceObj& obj) : m_obj(obj)
        {
            m_obj.m_mutex.lock();
        }

        ~CObjectLocker()
        {
            m_obj.m_mutex.unlock();
        }

    private:

        ResourceObj& m_obj;
};

#endif // COBJECTLOCKER_H
