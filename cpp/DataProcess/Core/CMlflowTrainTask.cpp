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

#include "CMlflowTrainTask.h"
#include <QDateTime>
#include "UtilsTools.hpp"

using namespace boost::python;

CMlflowTrainTask::CMlflowTrainTask(): CDnnTrainTask()
{
    m_pParam = std::make_shared<CDnnTrainTaskParam>();
    initMlFlow();
}

CMlflowTrainTask::CMlflowTrainTask(const std::string &name): CDnnTrainTask(name)
{
    m_pParam = std::make_shared<CDnnTrainTaskParam>();
    initMlFlow();
}

CMlflowTrainTask::CMlflowTrainTask(const std::string &name, const std::shared_ptr<CDnnTrainTaskParam>& pParam): CDnnTrainTask(name, pParam)
{
    initMlFlow();
}

void CMlflowTrainTask::beginTaskRun()
{
    CDnnTrainTask::beginTaskRun();

    CPyEnsureGIL gil;
    object mlflow = import("mlflow");
    mlflow.attr("end_run")();

    if(m_experimentId == -1)
        return;

    try
    {
        object none;
        str strTaskName(m_name);
        str strId(m_experimentId);
        mlflow.attr("start_run")(none, strId, strTaskName, false);

        auto paramPtr = std::static_pointer_cast<CDnnTrainTaskParam>(m_pParam);
        logParams(paramPtr->m_cfg);
    }
    catch(error_already_set&)
    {
        //Not a critical error
        qWarning().noquote() << QString::fromStdString(Utils::Python::handlePythonException());
    }
}

void CMlflowTrainTask::endTaskRun()
{
    CDnnTrainTask::endTaskRun();

    try
    {
        CPyEnsureGIL gil;
        object mlflow = import("mlflow");
        mlflow.attr("end_run")();
    }
    catch(error_already_set&)
    {
        //Not a critical error
        qWarning().noquote() << QString::fromStdString(Utils::Python::handlePythonException());
    }
}

void CMlflowTrainTask::logParam(const std::string &key, int value)
{
    if(m_experimentId == -1)
        return;

    try
    {
        CPyEnsureGIL gil;
        str strKey(key);
        object mlflow = import("mlflow");
        mlflow.attr("log_param")(strKey, value);
    }
    catch(error_already_set&)
    {
        //Not a critical error
        qWarning().noquote() << QString::fromStdString(Utils::Python::handlePythonException());
    }
}

void CMlflowTrainTask::logParam(const std::string &key, float value)
{
    if(m_experimentId == -1)
        return;

    try
    {
        CPyEnsureGIL gil;
        str strKey(key);
        object mlflow = import("mlflow");
        mlflow.attr("log_param")(strKey, value);
    }
    catch(error_already_set&)
    {
        //Not a critical error
        qWarning().noquote() << QString::fromStdString(Utils::Python::handlePythonException());
    }
}

void CMlflowTrainTask::logParam(const std::string &key, const std::string &value)
{
    if(m_experimentId == -1)
        return;

    try
    {
        CPyEnsureGIL gil;
        str strKey(key);
        str strValue(value);
        object mlflow = import("mlflow");
        mlflow.attr("log_param")(strKey, strValue);
    }
    catch(error_already_set&)
    {
        //Not a critical error
        qWarning().noquote() << QString::fromStdString(Utils::Python::handlePythonException());
    }
}

void CMlflowTrainTask::logParams(const std::map<std::string, std::string> &params)
{
    if(m_experimentId == -1)
        return;

    try
    {
        CPyEnsureGIL gil;
        dict pyParams;

        for(auto it=params.begin(); it!=params.end(); ++it)
        {
            str key(it->first);
            str value(it->second);
            pyParams[key] = value;
        }

        object mlflow = import("mlflow");
        mlflow.attr("log_params")(pyParams);
    }
    catch(error_already_set&)
    {
        //Not a critical error
        qWarning().noquote() << QString::fromStdString(Utils::Python::handlePythonException());
    }
}

void CMlflowTrainTask::logMetric(const std::string &key, float value, int step)
{
    if(m_experimentId == -1)
        return;

    try
    {
        CPyEnsureGIL gil;
        str strKey(key);
        object mlflow = import("mlflow");
        mlflow.attr("log_metric")(strKey, value, step);
    }
    catch(error_already_set&)
    {
        //Not a critical error
        qWarning().noquote() << QString::fromStdString(Utils::Python::handlePythonException());
    }
}

void CMlflowTrainTask::logMetrics(const std::map<std::string, float> &metrics, int step)
{
    if(m_experimentId == -1)
        return;

    try
    {
        CPyEnsureGIL gil;
        dict pyMetrics;

        for(auto it=metrics.begin(); it!=metrics.end(); ++it)
        {
            str key(it->first);
            pyMetrics[key] = it->second;
        }

        object mlflow = import("mlflow");
        mlflow.attr("log_metrics")(pyMetrics, step);
    }
    catch(error_already_set&)
    {
        //Not a critical error
        qWarning().noquote() << QString::fromStdString(Utils::Python::handlePythonException());
    }
}

void CMlflowTrainTask::logArtifact(const std::string &path)
{
    if(m_experimentId == -1)
        return;

    try
    {
        CPyEnsureGIL gil;
        str strPath(path);
        object mlflow = import("mlflow");
        mlflow.attr("log_artifact")(strPath);
    }
    catch(error_already_set&)
    {
        //Not a critical error
        qWarning().noquote() << QString::fromStdString(Utils::Python::handlePythonException());
    }
}

void CMlflowTrainTask::logArtifacts(const std::string &folderPath)
{
    if(m_experimentId == -1)
        return;

    try
    {
        CPyEnsureGIL gil;
        str strPath(folderPath);
        object mlflow = import("mlflow");
        mlflow.attr("log_artifacts")(strPath);
    }
    catch(error_already_set&)
    {
        //Not a critical error
        qWarning().noquote() << QString::fromStdString(Utils::Python::handlePythonException());
    }
}

void CMlflowTrainTask::initMlFlow()
{
    try
    {
        CPyEnsureGIL gil;

        // Create experiment
        auto date = "experiment_" + QDateTime::currentDateTime().toString(Qt::ISODate).toStdString();
        str strDate(date);

        object mlflow = import("mlflow");
        mlflow.attr("set_tracking_uri")(Utils::MLflow::getTrackingURI());
        m_experimentId = std::stoi(extract<std::string>(mlflow.attr("create_experiment")(strDate)));
    }
    catch(error_already_set&)
    {
        //Not a critical error
        qWarning().noquote() << QString::fromStdString(Utils::Python::handlePythonException());
    }
}
