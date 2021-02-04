#ifndef CDATAINFO_H
#define CDATAINFO_H

#include <string>
#include <memory>
#include "Main/CoreGlobal.hpp"
#include "Main/CoreDefine.hpp"

/** @cond INTERNAL */

class CORESHARED_EXPORT CDataInfo
{
    public:

        //Constructors
        CDataInfo(IODataType type);
        CDataInfo(IODataType type, const std::string& fileName);
        CDataInfo(const CDataInfo& data);
        CDataInfo(CDataInfo&& data);

        //Destructor
        virtual ~CDataInfo();

        //Operators
        CDataInfo&                  operator=(const CDataInfo& data);
        CDataInfo&                  operator=(CDataInfo&& data);

        //Setters
        void                        setFileName(const std::string& fileName);
        void                        setElementSize(int size);

        //Getters
        std::string                 getFileName() const;
        IODataType                  getType() const;
        int                         getElementSize() const;
        UMapString                  getMetadata() const;
        UMapString&                 metadata();
        virtual VectorPairString    getStringList() const;

    private:

        IODataType      m_type;
        std::string     m_fileName;
        UMapString      m_metadata;
        int             m_elementSize = 1;
};

using CDataInfoPtr = std::shared_ptr<CDataInfo>;

/** @endcond */

#endif // CDATAINFO_H
