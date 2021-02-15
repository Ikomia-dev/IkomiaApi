/*
 * Copyright (C) 2021 Ikomia SAS
 * Contact: https://www.ikomia.com
 *
 * This file is part of the Ikomia API libraries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef CDNNTRAINPROCESS_H
#define CDNNTRAINPROCESS_H

#include "DataProcessGlobal.hpp"
#include "Protocol/CProtocolTask.h"
#include "CException.h"

//---------------------------------//
//----- CDnnTrainProcessParam -----//
//---------------------------------//
class DATAPROCESSSHARED_EXPORT CDnnTrainProcessParam: public CProtocolTaskParam
{
    public:

        CDnnTrainProcessParam();

        virtual ~CDnnTrainProcessParam() = default;

        virtual void        setParamMap(const UMapString& paramMap) override;
        void                setBatchSize(int size);
        void                setEpochCount(int nb);
        void                setClassCount(int nb);
        void                setLearningRate(float value);
        void                setMomentum(float value);
        void                setWeightDecay(float decay);
        void                setModelName(const std::string& name);

        virtual UMapString  getParamMap() const override;
        virtual uint        getHashValue() const override;
        int                 getBatchSize() const;
        int                 getEpochCount() const;
        int                 getClassCount() const;
        float               getLearningRate() const;
        float               getMomentum() const;
        float               getWeightDecay() const;
        std::string         getModelName() const;

    public:

        int         m_batchSize = 32;
        int         m_epochs = 1;
        int         m_classes = 1;
        float       m_learningRate = 0.001;
        float       m_momentum = 0.9;
        float       m_weightDecay = 0.0005;
        std::string m_modelName = "";
};

//----------------------------//
//----- CDnnTrainProcess -----//
//----------------------------//
class DATAPROCESSSHARED_EXPORT CDnnTrainProcess : public CProtocolTask
{
    public:

        CDnnTrainProcess();
        CDnnTrainProcess(const std::string& name);
        CDnnTrainProcess(const std::string& name, const std::shared_ptr<CDnnTrainProcessParam>& pParam);

        virtual ~CDnnTrainProcess() = default;

        /**
         * @brief See CProtocolTask::beginTaskRun.
         */
        virtual void    beginTaskRun() override;
        /**
         * @brief See CProtocolTask::endTaskRun.
         */
        virtual void    endTaskRun() override;
};

#endif // CDNNTRAINPROCESS_H
