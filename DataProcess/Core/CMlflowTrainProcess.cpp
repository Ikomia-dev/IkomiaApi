#include "CMlflowTrainProcess.h"
#include <QDateTime>
#include "UtilsTools.hpp"

using namespace boost::python;

CMlflowTrainProcess::CMlflowTrainProcess(): CDnnTrainProcess()
{
    m_pParam = std::make_shared<CDnnTrainProcessParam>();
    initMlFlow();
}

CMlflowTrainProcess::CMlflowTrainProcess(const std::string &name): CDnnTrainProcess(name)
{
    m_pParam = std::make_shared<CDnnTrainProcessParam>();
    initMlFlow();
}

CMlflowTrainProcess::CMlflowTrainProcess(const std::string &name, const std::shared_ptr<CDnnTrainProcessParam>& pParam): CDnnTrainProcess(name, pParam)
{
    initMlFlow();
}

void CMlflowTrainProcess::beginTaskRun()
{
    CDnnTrainProcess::beginTaskRun();

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
    }
    catch(error_already_set&)
    {
        //Not a critical error
        qWarning().noquote() << QString::fromStdString(Utils::Python::handlePythonException());
    }
}

void CMlflowTrainProcess::endTaskRun()
{
    CDnnTrainProcess::endTaskRun();

    try
    {
        CPyEnsureGIL gil;
        object mlflow = import("mlflow");
        mlflow.attr("end_run")();
        m_experimentId = -1;
    }
    catch(error_already_set&)
    {
        //Not a critical error
        qWarning().noquote() << QString::fromStdString(Utils::Python::handlePythonException());
    }
}

void CMlflowTrainProcess::logParam(const std::string &key, int value)
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

void CMlflowTrainProcess::logParam(const std::string &key, float value)
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

void CMlflowTrainProcess::logParam(const std::string &key, const std::string &value)
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

void CMlflowTrainProcess::logParams(const std::map<std::string, std::string> &params)
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

void CMlflowTrainProcess::logMetric(const std::string &key, float value, int step)
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

void CMlflowTrainProcess::logMetrics(const std::map<std::string, float> &metrics, int step)
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

void CMlflowTrainProcess::logArtifact(const std::string &path)
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

void CMlflowTrainProcess::logArtifacts(const std::string &folderPath)
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

void CMlflowTrainProcess::initMlFlow()
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
