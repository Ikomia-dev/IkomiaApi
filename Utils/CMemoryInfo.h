#ifndef CMEMORYINFO_H
#define CMEMORYINFO_H

#include "UtilsGlobal.hpp"

/**
 * @brief
 * This class offers useful functions to query memory information.
 * These functions are tested on Windows 10, Linux and Mac OS X.
 * @ingroup groupUtils
 */
class UTILSSHARED_EXPORT CMemoryInfo
{
    public:

        /**
         * @brief Default constructor
         */
        CMemoryInfo();

        /**
         * @brief This function gives the total amount of memory installed on the machine.
         * @return Total amount of memory in MB.
         */
        unsigned long long  totalMemory() const;
        /**
         * @brief This function gives the amount of available memory.
         * @return Available amount of memory in MB.
         */
        unsigned long long  availableMemory() const;
        /**
         * @brief This functions gives the memory load.
         * @return Memory load in percentage.
         */
        int                 memoryLoad() const;

    private:

        /// @cond INTERNAL
        void                getWindowsMemoryInfo();
        void                getLinuxMemoryInfo();
        void                getAppleMemoryInfo();
        unsigned long long  getNumberFromQString(const QString &xString);

    private:

        int                 m_memoryLoad = 0;
        unsigned long long  m_total = 0;
        unsigned long long  m_available = 0;
#ifndef Q_OS_MAC
        unsigned long long  m_totalPageFile = 0;
        unsigned long long  m_availablePageFile = 0;
#endif

        /// @endcond
};

#endif // CMEMORYINFO_H
