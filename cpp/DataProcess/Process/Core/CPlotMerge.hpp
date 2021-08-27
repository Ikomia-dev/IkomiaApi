// Copyright (C) 2021 Ikomia SAS
// Contact: https://www.ikomia.com
//
// This file is part of the Ikomia API libraries.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef CPLOTPROCESS_H
#define CPLOTPROCESS_H

#include "Core/CTaskFactory.hpp"
#include "IO/CImageIO.h"
#include "IO/CBlobMeasureIO.h"
#include "IO/CNumericIO.hpp"

//-----------------------------//
//----- CPlotProcessParam -----//
//-----------------------------//
class CPlotMergeParam: public CWorkflowTaskParam
{
    public:

        CPlotMergeParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_inputCount = std::stoi(paramMap.at("inputCount"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("inputCount", std::to_string(m_inputCount)));
            return map;
        }

    public:

        int m_inputCount = 1;
};

//------------------------//
//----- CPlotProcess -----//
//------------------------//
class CPlotMerge : public CWorkflowTask
{
    public:

        CPlotMerge() : CWorkflowTask()
        {
            addInput(std::make_shared<CImageIO>(IODataType::IMAGE));
            addOutput(std::make_shared<CImageIO>(IODataType::IMAGE));
            addOutput(std::make_shared<CNumericIO<double>>());
        }
        CPlotMerge(const std::string name, const std::shared_ptr<CPlotMergeParam>& pParam) : CWorkflowTask(name)
        {
            m_pParam = std::make_shared<CPlotMergeParam>(*pParam);
            addInput(std::make_shared<CImageIO>(IODataType::IMAGE));

            for(int i=0; i<pParam->m_inputCount; ++i)
                addInput(std::make_shared<CNumericIO<double>>());

            addOutput(std::make_shared<CImageIO>(IODataType::IMAGE));
            addOutput(std::make_shared<CNumericIO<double>>());
        }

        void    parametersModified() override
        {
            auto pParamTmp = std::dynamic_pointer_cast<CPlotMergeParam>(m_pParam);
            size_t inOldCount = getInputCount()-1;

            if(pParamTmp->m_inputCount > (int)inOldCount)
            {
                for(size_t i=0; i<pParamTmp->m_inputCount-inOldCount; ++i)
                    addInput(std::make_shared<CNumericIO<double>>());
            }
            else
            {
                for(size_t i=0; i<inOldCount-pParamTmp->m_inputCount; ++i)
                    removeInput(getInputCount() - 1);
            }
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<CPlotMergeParam>(m_pParam);

            if(pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            std::vector<std::shared_ptr<CNumericIO<double>>> inputs;
            int index = 1;
            for(int i=0; i<pParam->m_inputCount; ++i)
            {
                inputs.push_back(std::dynamic_pointer_cast<CNumericIO<double>>(getInput(index++)));
                if(inputs.back() == nullptr)
                    throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);
            }
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CNumericIO<double>>(getOutput(1));
            if(pOutput)
            {
                pOutput->clearData();

                for(auto&& it : inputs)
                {
                    auto valueLists = it->getAllValues();
                    auto labelLists = it->getAllValueLabels();
                    assert(valueLists.size() == labelLists.size());

                    for(size_t i=0; i<valueLists.size(); ++i)
                        pOutput->addValueList(valueLists[i], labelLists[i]);
                }
                pOutput->setOutputType(NumericOutputType::PLOT);
                pOutput->setPlotType(PlotType::HISTOGRAM);
            }
            emit m_signalHandler->doProgress();

            forwardInputImage();
            endTaskRun();
            emit m_signalHandler->doProgress();
        }

    private:

        void    forwardInputImage()
        {
            if(getOutputCount() == 0)
                throw CException(CoreExCode::INVALID_SIZE, "Wrong outputs count", __func__, __FILE__, __LINE__);

            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));

            if(pOutput)
                pOutput->setImage(pInput->getImage());
        }
};

class CPlotMergeFactory : public CTaskFactory
{
    public:

        CPlotMergeFactory()
        {
            m_info.m_name = QObject::tr("Plots merge").toStdString();
            m_info.m_description = QObject::tr("This process mutualizes data for plotting.").toStdString();
            m_info.m_path = QObject::tr("Core/Plots").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/default-process.png").toStdString();
            m_info.m_keywords = "Plot, histogram, chart";
        }
        ~CPlotMergeFactory() {}

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CPlotMergeParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CPlotMerge>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<CPlotMergeParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<CPlotMerge>(m_info.m_name, pDerivedParam);
        }
};

#endif // CPLOTPROCESS_H
