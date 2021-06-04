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

#ifndef CDNNTRAINTASK_H
#define CDNNTRAINTASK_H

#include "DataProcessGlobal.hpp"
#include "Workflow/CWorkflowTask.h"
#include "CException.h"

//------------------------------//
//----- CDnnTrainTaskParam -----//
//------------------------------//
class DATAPROCESSSHARED_EXPORT CDnnTrainTaskParam: public CWorkflowTaskParam
{
    public:

        CDnnTrainTaskParam();

        virtual ~CDnnTrainTaskParam() = default;

        virtual void        setParamMap(const UMapString& paramMap) override;

        virtual UMapString  getParamMap() const override;
        virtual uint        getHashValue() const override;

    public:

        std::map<std::string,std::string>   m_cfg;
};

//-------------------------//
//----- CDnnTrainTask -----//
//-------------------------//
class DATAPROCESSSHARED_EXPORT CDnnTrainTask : public CWorkflowTask
{
    public:

        CDnnTrainTask();
        CDnnTrainTask(const std::string& name);
        CDnnTrainTask(const std::string& name, const std::shared_ptr<CDnnTrainTaskParam>& pParam);

        ~CDnnTrainTask() = default;

        std::string     getTensorboardLogDir() const;

        void            enableMlflow(bool bEnable);
        void            enableTensorboard(bool bEnable);

        /**
         * @brief See CWorkflowTask::beginTaskRun.
         */
        virtual void    beginTaskRun() override;
        /**
         * @brief See CWorkflowTask::endTaskRun.
         */
        virtual void    endTaskRun() override;

    private:

        bool    m_bOpenMlflow = true;
        bool    m_bOpenTensorboad = true;
};

#endif // CDNNTRAINTASK_H
