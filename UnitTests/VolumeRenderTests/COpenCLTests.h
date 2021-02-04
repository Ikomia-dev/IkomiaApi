#ifndef COPENCLTESTS_H
#define COPENCLTESTS_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <memory>
#include "Data/CMat.hpp"
#include "VolumeRenderDefine.h"
#include "ocl.hpp"

class CRender
{
    public:
        CRender() {}

        RenderError  initCL()
        {
            try
            {
                m_pOclObj = std::make_unique<ocl::COpenCL>(CL_DEVICE_TYPE_GPU, m_bInteropGLCL);
                m_pOclObj->init();
            }
            catch(cl::Error& e)
            {
                std::cout << e.what() << "(" << ocl::utils::getOpenCLErrorCodeStr(e.err()) << ")" << std::endl;
                return RenderError::INIT_FAILED;
            }
            return RenderError::SUCCESS;
        }

        void    setInputData(CMat inputData)
        {
            m_inputData = inputData;
        }
        void    setCLGLInterop(bool bEnable)
        {
            m_bInteropGLCL = bEnable;
        }

        void    execute(GLuint& pbo);

    private:
        std::unique_ptr<ocl::COpenCL> m_pOclObj = nullptr;
        bool m_bInteropGLCL = false;
        CMat m_inputData;
};

class QGLTestWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    public:
        QGLTestWidget(QWidget* pParent = nullptr) : QOpenGLWidget(pParent) {}

        void setImage(CMat image);

    protected:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;

        void initCLGL();
        void perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

        void smoothOpencvImg();

    private:
        std::unique_ptr<CRender> m_pRender = nullptr;
        int m_width = 0;
        int m_height = 0;
        CMat m_image;
        GLuint m_pbo;
        GLuint m_tex;
};

class COpenCLTests : public QObject
{
    Q_OBJECT
    public:
        COpenCLTests(QObject *parent = nullptr);

    private slots:
        void onInitOpenCLOnly();
        void onInitOpenCLOpenGL();
        void onPerformHelloWorld();
        void onSmoothOpencvImg();
        void onSmoothOpencvImgGL();

    private:
        CMat loadSampleImage();

    private:
        CMat m_image;
};

#endif // COPENCLTESTS_H
