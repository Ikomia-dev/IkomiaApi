#ifndef CDATASETINFO_H
#define CDATASETINFO_H

#include "Main/CoreDefine.hpp"
#include "Main/CoreGlobal.hpp"
#include <string>

/** @cond INTERNAL */

class CORESHARED_EXPORT CDatasetInfo
{
    public:

        //Constructors
        CDatasetInfo();
        /**
         * @brief
         *
         * @param info
         */
        CDatasetInfo(const CDatasetInfo& info);
        /**
         * @brief
         *
         * @param info
         */
        CDatasetInfo(CDatasetInfo&& info);

        //Operators
        /**
         * @brief
         *
         * @param info
         * @return CDatasetInfo &&operator
         */
        CDatasetInfo&   operator=(const CDatasetInfo& info);
        /**
         * @brief
         *
         * @param info
         * @return CDatasetInfo &&operator
         */
        CDatasetInfo&   operator=(CDatasetInfo&& info);

        bool            operator==(const CDatasetInfo& other) const;

        //Getters
        /**
         * @brief
         *
         * @return std::string
         */
        std::string     getName() const;

        //Setters
        /**
         * @brief
         *
         * @param name
         */
        void            setName(const std::string &getName);

    private:

        std::string m_name;
};

/** @endcond */

#endif // CDATASETINFO_H
