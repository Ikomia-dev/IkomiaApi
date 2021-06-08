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

#include "COcvDnnProcess.h"

//----------------------------//
//----- COcvDnnCoreParam -----//
//----------------------------//
COcvDnnCoreParam::COcvDnnCoreParam() : CWorkflowTaskParam()
{
}

void COcvDnnCoreParam::setParamMap(const UMapString& paramMap)
{
    m_backend = static_cast<cv::dnn::Backend>(std::stoi(paramMap.at("backend")));
    m_target = static_cast<cv::dnn::Target>(std::stoi(paramMap.at("target")));
}

UMapString COcvDnnCoreParam::getParamMap() const
{
    UMapString map;
    map.insert(std::make_pair("backend", std::to_string(m_backend)));
    map.insert(std::make_pair("target", std::to_string(m_target)));
    return map;
}

//-------------------------------//
//----- COcvDnnProcessParam -----//
//-------------------------------//
COcvDnnProcessParam::COcvDnnProcessParam() : COcvDnnCoreParam()
{
}

void COcvDnnProcessParam::setParamMap(const UMapString &paramMap)
{
    COcvDnnCoreParam::setParamMap(paramMap);
    m_framework = std::stoi(paramMap.at("framework"));
    m_inputSize = std::stoi(paramMap.at("inputSize"));
    m_modelName = paramMap.at("modelName");
    m_datasetName = paramMap.at("datasetName");
    m_modelFile = paramMap.at("modelFile");
    m_structureFile = paramMap.at("structureFile");
    m_labelsFile = paramMap.at("labelsFile");
}

UMapString COcvDnnProcessParam::getParamMap() const
{
    UMapString map = COcvDnnCoreParam::getParamMap();
    map.insert(std::make_pair("framework", std::to_string(m_framework)));
    map.insert(std::make_pair("inputSize", std::to_string(m_inputSize)));
    map.insert(std::make_pair("modelName", m_modelName));
    map.insert(std::make_pair("datasetName", m_datasetName));
    map.insert(std::make_pair("modelFile", m_modelFile));
    map.insert(std::make_pair("structureFile", m_structureFile));
    map.insert(std::make_pair("labelsFile", m_labelsFile));
    return map;
}

//--------------------------//
//----- COcvDnnProcess -----//
//--------------------------//
COcvDnnProcess::COcvDnnProcess() : C2dImageTask()
{
}

COcvDnnProcess::COcvDnnProcess(const std::string &name) : C2dImageTask(name)
{
}

int COcvDnnProcess::getNetworkInputSize() const
{
    return 224;
}

double COcvDnnProcess::getNetworkInputScaleFactor() const
{
    return 1.0;
}

cv::Scalar COcvDnnProcess::getNetworkInputMean() const
{
    return cv::Scalar();
}

std::vector<cv::String> COcvDnnProcess::getOutputsNames() const
{
    std::vector<cv::String> names;
    std::vector<int> outLayers = m_net.getUnconnectedOutLayers();
    std::vector<cv::String> layersNames = m_net.getLayerNames();
    names.resize(outLayers.size());

    for (size_t i = 0; i < outLayers.size(); ++i)
        names[i] = layersNames[outLayers[i] - 1];

    return names;
}

cv::dnn::Net COcvDnnProcess::readDnn()
{
    cv::dnn::Net net;
    auto pParam = std::dynamic_pointer_cast<COcvDnnProcessParam>(m_pParam);

    switch(pParam->m_framework)
    {
        case COcvDnnProcessParam::TENSORFLOW:
            net = cv::dnn::readNetFromTensorflow(pParam->m_modelFile, pParam->m_structureFile);
            break;
        case COcvDnnProcessParam::CAFFE:
            net = cv::dnn::readNetFromCaffe(pParam->m_structureFile, pParam->m_modelFile);
            break;
        case COcvDnnProcessParam::DARKNET:
            net = cv::dnn::readNetFromDarknet(pParam->m_structureFile, pParam->m_modelFile);
            break;
        case COcvDnnProcessParam::TORCH:
            net = cv::dnn::readNetFromTorch(pParam->m_modelFile, true);
            break;
        case COcvDnnProcessParam::ONNX:
            net = cv::dnn::readNetFromONNX(pParam->m_modelFile);
    }
    net.setPreferableBackend(pParam->m_backend);
    net.setPreferableTarget(pParam->m_target);
    //displayLayers(net);
    return net;
}

void COcvDnnProcess::readClassNames()
{
    auto pParam = std::dynamic_pointer_cast<COcvDnnProcessParam>(m_pParam);
    if(pParam->m_labelsFile.empty() == true)
        return;

    std::ifstream file(pParam->m_labelsFile);
    if(file.is_open() == false)
        throw CException(CoreExCode::INVALID_FILE, "Failed to open labels file", __func__, __FILE__, __LINE__);

    std::string name;
    m_classNames.clear();

    while(!file.eof())
    {
        std::getline(file, name);
        if(name.empty() == false)
            m_classNames.push_back(name);
    }
    file.close();
}

void COcvDnnProcess::displayLayers(const cv::dnn::Net& net)
{
    auto layerNames = net.getLayerNames();
    for(size_t i=0; i<layerNames.size(); ++i)
        qDebug() << QString(layerNames[i].c_str());
}
