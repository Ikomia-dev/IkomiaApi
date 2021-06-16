#ifndef UTILSDEFINE_HPP
#define UTILSDEFINE_HPP

namespace Ikomia
{
    enum class PluginState : size_t
    {
        VALID,
        DEPRECATED,
        UPDATED,
        INVALID
    };

    /**
     * @enum OS
     * @brief The OS enum defines the possible operating systems.
     */
    enum OSType
    {
        ALL,    /**< Cross-platform process, implemented in Python */
        LINUX,  /**< Linux */
        WIN,    /**< Windows 10 */
        OSX     /**< Mac OS X 10.13 or higher */
    };

    /**
     * @enum Language
     * @brief The Language enum defines the possible programming languages.
     */
    enum ApiLanguage
    {
        CPP,    /**< C++ */
        PYTHON  /**< Python */
    };
}

#endif // UTILSDEFINE_HPP
