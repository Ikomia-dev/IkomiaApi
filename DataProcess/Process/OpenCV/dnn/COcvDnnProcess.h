#ifndef CDNNPROCESS_H
#define CDNNPROCESS_H

#include "Core/CImageProcess2d.h"
#include "opencv2/dnn.hpp"

//----------------------------//
//----- COcvDnnCoreParam -----//
//----------------------------//
class DATAPROCESSSHARED_EXPORT COcvDnnCoreParam: public CProtocolTaskParam
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
class DATAPROCESSSHARED_EXPORT COcvDnnProcess : public CImageProcess2d
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
