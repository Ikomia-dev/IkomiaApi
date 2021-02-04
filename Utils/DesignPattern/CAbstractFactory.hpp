#ifndef CABSTRACTFACTORY_HPP
#define CABSTRACTFACTORY_HPP

#include <mutex>
#include <memory>
#include <cassert>
#include <unordered_map>
#include <vector>
#include <functional>

/// @cond INTERNAL

//Abstract factory
template <class Key, class Object, class... Args>
class CAbstractFactory
{
    public:

        using Creator = std::function<Object(Args...)>;

        void    registerCreator(const Key& key, const Creator& creator)
        {
            assert(creator);
            if(m_creators.count(key) == 0)
                m_creators.emplace(key, creator);
        }

        void    unregisterCreator(const Key& key)
        {
            auto it = m_creators.find(key);
            if(it != m_creators.end())
                m_creators.erase(it);
        }

        Object  createObject(const Key& key, Args&&... args)
        {
            if(m_creators.find(key) == m_creators.end())
                return Object();
            else
                return m_creators[key](std::forward<Args>(args)...);
        }

        std::unordered_map<Key, Creator>& getCreators()
        {
            return m_creators;
        }

        bool    isCreatorExists(const Key& key) const
        {
            return m_creators.find(key) != m_creators.end();
        }

    protected:

        std::unordered_map<Key, Creator> m_creators;
};

/// @endcond

#endif // CABSTRACTFACTORY_HPP
