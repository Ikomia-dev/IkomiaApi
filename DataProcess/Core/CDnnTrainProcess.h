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

        virtual UMapString  getParamMap() const override;
        virtual uint        getHashValue() const override;

    public:

        std::map<std::string,std::string>   m_cfg;
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

        ~CDnnTrainProcess() = default;

        std::string     getTensorboardLogDir() const;

        void            enableMlflow(bool bEnable);
        void            enableTensorboard(bool bEnable);

        /**
         * @brief See CProtocolTask::beginTaskRun.
         */
        virtual void    beginTaskRun() override;
        /**
         * @brief See CProtocolTask::endTaskRun.
         */
        virtual void    endTaskRun() override;

    private:

        bool    m_bOpenMlflow = true;
        bool    m_bOpenTensorboad = true;
};

#endif // CDNNTRAINPROCESS_H
