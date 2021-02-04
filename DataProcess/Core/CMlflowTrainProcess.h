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
