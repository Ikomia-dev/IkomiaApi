#ifndef CSINGLETON_H
#define CSINGLETON_H

/// @cond INTERNAL

template<class Type>
class CSingleton
{
    public:

        static Type& getInstance()
        {
            // Use static function scope variable to
            // correctly define lifespan of object.
            static Type instance;
            return instance;
        }
};

/// @endcond

#endif // CSINGLETON_HPP
