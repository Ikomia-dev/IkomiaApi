#ifndef CFACTORYREGISTER_HPP
#define CFACTORYREGISTER_HPP

#include "CSingleton.hpp"

/// @cond INTERNAL

template<class Type>
class CFactoryRegister : public CSingleton<Type>
{
    public:

        // Specific function for process/widget registration (must implement registration() otherwise undefined behavior)
        static bool registration()
        {
            auto& instance = CSingleton<Type>::getInstance();
            instance.registration();
            return true;
        }
};

/// @endcond

#endif // CFACTORYREGISTER_HPP
