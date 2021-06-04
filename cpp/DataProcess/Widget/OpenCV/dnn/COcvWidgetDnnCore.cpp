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

#include "COcvWidgetDnnCore.h"

COcvWidgetDnnCore::COcvWidgetDnnCore(QWidget *parent) : CWorkflowTaskWidget(parent)
{
    init();
    initConnections();
}

COcvWidgetDnnCore::COcvWidgetDnnCore(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent) : CWorkflowTaskWidget(parent)
{
    m_pParam = std::dynamic_pointer_cast<COcvDnnCoreParam>(pParam);
    init();
    initConnections();
}

void COcvWidgetDnnCore::init()
{
    if(m_pParam == nullptr)
        m_pParam = std::make_shared<COcvDnnCoreParam>();

    //DNN Backends
    m_pComboBackend = addCombo(0, "DNN Backend");
    m_pComboBackend->addItem("Default", cv::dnn::DNN_BACKEND_DEFAULT);
    m_pComboBackend->addItem("Halide", cv::dnn::DNN_BACKEND_HALIDE);
    m_pComboBackend->addItem("Inference engine", cv::dnn::DNN_BACKEND_INFERENCE_ENGINE);
    m_pComboBackend->addItem("OpenCV", cv::dnn::DNN_BACKEND_OPENCV);
    m_pComboBackend->addItem("VKCOM", cv::dnn::DNN_BACKEND_VKCOM);
    m_pComboBackend->addItem("CUDA", cv::dnn::DNN_BACKEND_CUDA);
    m_pComboBackend->setCurrentIndex(m_pComboBackend->findData(m_pParam->m_backend));

    //DNN targets
    m_pComboTarget = addCombo(1, "DNN Target");
    fillComboTarget(m_pParam->m_backend);
    m_pComboTarget->setCurrentIndex(m_pComboTarget->findData(m_pParam->m_target));
}

void COcvWidgetDnnCore::initConnections()
{
    connect(m_pComboBackend, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index)
    {
        Q_UNUSED(index);

        auto backend = static_cast<cv::dnn::Backend>(m_pComboBackend->currentData().toInt());
        if(backend != m_pParam->m_backend)
        {
            m_pParam->m_bUpdate = true;
            m_pParam->m_backend = backend;
            fillComboTarget(backend);
        }
    });

    connect(m_pComboTarget, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index)
    {
        Q_UNUSED(index);
        auto target = static_cast<cv::dnn::Target>(m_pComboTarget->currentData().toInt());
        if(target != m_pParam->m_target)
        {
            m_pParam->m_bUpdate = true;
            m_pParam->m_target = target;
        }
    });
}

void COcvWidgetDnnCore::fillComboTarget(cv::dnn::Backend backend)
{
    auto targetList = cv::dnn::getAvailableTargets(backend);
    m_pComboTarget->clear();

    for(auto it : targetList)
        m_pComboTarget->addItem(getTargetName(it), it);

    m_pComboTarget->setCurrentIndex(0);
}

QString COcvWidgetDnnCore::getTargetName(const cv::dnn::Target &target) const
{
    switch(target)
    {
        case cv::dnn::DNN_TARGET_CPU: return "CPU";
        case cv::dnn::DNN_TARGET_OPENCL: return "OpenCL FP32";
        case cv::dnn::DNN_TARGET_OPENCL_FP16: return "OpenCL FP16";
        case cv::dnn::DNN_TARGET_MYRIAD: return "MYRIAD";
        case cv::dnn::DNN_TARGET_VULKAN: return "VULKAN";
        case cv::dnn::DNN_TARGET_FPGA: return "FPGA";
        case cv::dnn::DNN_TARGET_CUDA: return "CUDA FP32";
        case cv::dnn::DNN_TARGET_CUDA_FP16: return "CUDA FP16";
    }
    return "";
}
