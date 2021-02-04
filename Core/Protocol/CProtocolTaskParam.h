#ifndef CPROTOCOLTASKPARAM_HPP
#define CPROTOCOLTASKPARAM_HPP

#include <memory>
#include <QString>
#include "Main/CoreGlobal.hpp"
#include "Main/CoreDefine.hpp"

/**
 * @ingroup groupCore
 * @brief
 * This class is the base class for the management of process and plugin parameters.
 * It consists of two virtual methods that should be overriden to fill or get the structure holding parameters value.
 * This structure is used internally by the system to save and load parameters value of process used in a workflow.
 */
class CORESHARED_EXPORT CProtocolTaskParam
{
    public:

        /**
        * @brief Default constructor
        */
        CProtocolTaskParam();
        virtual ~CProtocolTaskParam() = default;

        /**
         * @brief Sets process parameters values from the given map structure.
         * @param Parameters map (UMapString) holding two std::string. Key = parameter name, Value = parameter value.
         */
        virtual void        setParamMap(const UMapString& paramMap);

        /**
         * @brief Gets map structure filled with process parameters values.
         */
        virtual UMapString  getParamMap() const;

        /**
         * @brief Gets hash value from parameters values.
         * The system uses this method de determine if process configuration has changed.
         */
        virtual uint        getHashValue() const;
};

using ProtocolTaskParamPtr = std::shared_ptr<CProtocolTaskParam>;

#endif // CPROTOCOLTASKPARAM_HPP
