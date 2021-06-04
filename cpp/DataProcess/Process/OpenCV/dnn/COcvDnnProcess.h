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

#ifndef CDNNPROCESS_H
#define CDNNPROCESS_H

#include "Core/C2dImageTask.h"
#include "opencv2/dnn.hpp"

//----------------------------//
//----- COcvDnnCoreParam -----//
//----------------------------//
class DATAPROCESSSHARED_EXPORT COcvDnnCoreParam: public CWorkflowTaskParam
{
    public:

        COcvDnnCoreParam();
        virtual ~COcvDnnCoreParam() = default;

        virtual void        setParamMap(const UMapString& paramMap) override;

        virtual UMapString  getParamMap() const override;

    public:

        cv::dnn::Backend    m_backend = cv::dnn::DNN_BACKEND_DEFAULT;
        cv::dnn::Target     m_target = cv::dnn::DNN_TARGET_CPU;
        bool                m_bUpdate = false;
};

//-------------------------------//
//----- COcvDnnProcessParam -----//
//-------------------------------//
class DATAPROCESSSHARED_EXPORT COcvDnnProcessParam: public COcvDnnCoreParam
{
    public:

        enum Framework {TENSORFLOW, CAFFE, DARKNET, TORCH, ONNX};

        COcvDnnProcessParam();
        virtual ~COcvDnnProcessParam() = default;

        virtual void        setParamMap(const UMapString& paramMap) override;

        virtual UMapString  getParamMap() const override;

    public:

        int         m_framework = Framework::CAFFE;
        int         m_inputSize = 416;
        std::string m_modelName;
        std::string m_datasetName;
        std::string m_modelFile;
        std::string m_structureFile;
        std::string m_labelsFile;
};

//--------------------------//
//----- COcvDnnProcess -----//
//--------------------------//
class DATAPROCESSSHARED_EXPORT COcvDnnProcess : public C2dImageTask
{
    public:

        COcvDnnProcess();
        COcvDnnProcess(const std::string& name);

        virtual ~COcvDnnProcess() = default;

        cv::dnn::Net                    readDnn();
        void                            readClassNames();

    protected:

        virtual int                     getNetworkInputSize() const;
        virtual double                  getNetworkInputScaleFactor() const;
        virtual cv::Scalar              getNetworkInputMean() const;
        virtual std::vector<cv::String> getOutputsNames() const;

        void                            displayLayers(const cv::dnn::Net& net);

    protected:

        std::string                 m_outputLayerName = "";
        std::vector<std::string>    m_classNames;
        cv::dnn::Net                m_net;
};

#endif // CDNNPROCESS_H
