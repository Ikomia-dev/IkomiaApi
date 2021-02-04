#ifndef CPROCESSFACTORY_H
#define CPROCESSFACTORY_H

#include "DesignPattern/CAbstractFactory.hpp"
#include "Core/CProcessInfo.h"
#include "Protocol/CProtocolTaskParam.h"
#include "Protocol/CProtocolTask.h"

//---------------------------//
//----- Process factory -----//
//---------------------------//
/**
 * @ingroup groupDataProcess
 * @brief The CProcessFactory class is an abstract class defining the core structure of the process factory.
 * The system extensibility for the process library is based on the well known factory design pattern.
 * Each process task must implement a factory class derived from this class.
 * Then the system is able to instantiate dynamically a process object (even user-defined).
 */
class CProcessFactory
{
    public:

        virtual ~CProcessFactory() = default;

        /**
         * @brief Gets the metadata structure associated with the process task.
         * @return CProcessInfo reference.
         */
        CProcessInfo&               getInfo(){ return m_info; }
        /**
         * @brief Gets the metadata structure associated with the process task.
         * @return CProcessInfo object.
         */
        CProcessInfo                getInfo() const { return m_info; }

        /**
         * @brief Sets the metadata structure associated with the process task.
         * @param info: see CProcessInfo for more details.
         */
        void                        setInfo(const CProcessInfo& info){ m_info = info; }

        /**
         * @brief Pure virtual method to create new process task instance with the given parameters.
         * @param pParam: CProtocolTaskParam based shared pointer.
         * @return CProtocolTask based shared pointer.
         */
        virtual ProtocolTaskPtr     create(const ProtocolTaskParamPtr& pParam) = 0;
        /**
         * @brief Pure virtual method to create new process task instance with default parameters.
         * @return CProtocolTask based shared pointer.
         */
        virtual ProtocolTaskPtr     create() = 0;

    protected:

        CProcessInfo                m_info;
};

using ProcessFactoryPtr = std::shared_ptr<CProcessFactory>;
using ProcessFactories = std::vector<ProcessFactoryPtr>;

//----- Process abstract factory -----//
class CProcessAbstractFactory: public CAbstractFactory<std::string, ProtocolTaskPtr, const ProtocolTaskParamPtr>
{
    public:

        ProcessFactories&   getList()
        {
            return m_factories;
        }

        ProcessFactoryPtr   getFactory(const std::string& name)
        {
            auto it = std::find_if(m_factories.begin(),
                                   m_factories.end(),
                                   [&name](const ProcessFactoryPtr& factoryPtr){ return factoryPtr->getInfo().getName() == name;});
            return *it;
        }

        void                remove(const std::string& name)
        {
            m_factories.erase(std::remove_if(m_factories.begin(),
                                             m_factories.end(),
                                             [name](const ProcessFactoryPtr& factoryPtr){return factoryPtr->getInfo().getName() == name;}),
                              m_factories.end());
        }

    private:

        ProcessFactories m_factories;
};

#endif // CPROCESSFACTORY_H
