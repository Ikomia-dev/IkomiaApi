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

#ifndef CDNNTRAINMLFLOWPROCESS_H
#define CDNNTRAINMLFLOWPROCESS_H

#include "CDnnTrainProcess.h"


class DATAPROCESSSHARED_EXPORT CMlflowTrainProcess: public CDnnTrainProcess
{
    public:

        CMlflowTrainProcess();
        CMlflowTrainProcess(const std::string& name);
        CMlflowTrainProcess(const std::string& name, const std::shared_ptr<CDnnTrainProcessParam>& pParam);

        void    beginTaskRun() override;
        void    endTaskRun() override;

        void    logParam(const std::string& key, int value);
        void    logParam(const std::string& key, float value);
        void    logParam(const std::string& key, const std::string& value);
        void    logParams(const std::map<std::string, std::string> &params);
        void    logMetric(const std::string& key, float value, int step=0);
        void    logMetrics(const std::map<std::string, float>& metrics, int step=0);
        void    logArtifact(const std::string& path);
        void    logArtifacts(const std::string& folderPath);

    private:

        void    initMlFlow();

    private:

        int     m_experimentId = -1;
};

#endif // CDNNTRAINMLFLOWPROCESS_H
