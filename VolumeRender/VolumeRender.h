#ifndef VOLUMERENDER_H
#define VOLUMERENDER_H

#include <QtOpenGL>
#include <memory>
#include <QMatrix4x4>
#include "Data/CMat.hpp"
#include "ocl.hpp"
#include "renderUtils.hpp"
#include "CTextureRender.h"
#include "VolumeRenderDefine.h"

class CProgressSignalHandler;

class VOLUMERENDERSHARED_EXPORT CRayParam
{
    public:

        void setParam(RenderParamType type, float value)
        {
            switch (type)
            {
                case RenderParamType::DENSITY: m_density = value; break;
                case RenderParamType::BRIGHTNESS: m_brightness = value; break;
                case RenderParamType::OFFSET: m_transferOffset = value; break;
                case RenderParamType::SCALE: m_transferScale = value; break;
                case RenderParamType::CHR_MIN: m_chR[0] = value; break;
                case RenderParamType::CHR_MAX: m_chR[1] = value; break;
                case RenderParamType::CHG_MIN: m_chG[0] = value; break;
                case RenderParamType::CHG_MAX: m_chG[1] = value; break;
                case RenderParamType::CHB_MIN: m_chB[0] = value; break;
                case RenderParamType::CHB_MAX: m_chB[1] = value; break;
                case RenderParamType::ISOVALUE_R: m_isoValue[0] = value; break;
                case RenderParamType::ISOVALUE_G: m_isoValue[1] = value; break;
                case RenderParamType::ISOVALUE_B: m_isoValue[2] = value; break;
                case RenderParamType::ISOVALUE_A: m_isoValue[3] = value; break;
                default:break;
            }
        }

        float m_density;
        float m_brightness;
        float m_transferOffset;
        float m_transferScale;
        float m_chR[2];
        float m_chG[2];
        float m_chB[2];
        float m_clipPlaneCenter[6];
        float m_K;
        float m_ratioX;
        float m_ratioY;
        float m_focalLength;
        float m_wndWidth;
        float m_wndHeight;
        float m_isoValue[4];
        float m_padding[2];
};

class VOLUMERENDERSHARED_EXPORT CVolumeRender
{
    public:

        using Img2DPtr = std::unique_ptr<cl::Image2D>;
        using Img3DPtr = std::unique_ptr<cl::Image3D>;
        using BufPtr = std::unique_ptr<cl::Buffer>;
        using SamplerPtr = std::unique_ptr<cl::Sampler>;

        CVolumeRender();
        ~CVolumeRender();

        RenderError initCL(const std::string name="");
        void        init();
        void        initDeviceBuffer();
        void        initHostBuffer();
        void        initStandardHostMemory();
        void        initPinnedHostMemory();
        void        initProgram();
        void        initKernel();
        void        initKernelArg();
        void        initPBO(GLuint pbo, GLuint pbo_depth);

        void        updateWndParameters();

        void        fillSegmentationStack();
        void        fillGrayStack();
        void        fillColorStack();
        void        fillHostBuffer();
        void        transferHostToDevice();
        void        convertTo8bits(CMat& src, CMat& dst);

        void        setParameters();
        void        setInputData(CMat inputData);
        void        setBinaryData(CMat binaryData);
        void        setCLGLInterop(bool bEnable);
        void        setMatrix(QMatrix4x4& model, QMatrix4x4& view, QMatrix4x4& projection);
        void        setWndSize(size_t width, size_t height);
        void        setLocalGlobalWorkSize();
        void        setRenderMode(RenderMode mode);
        void        setColormap(RenderColormap colormap);
        void        setRayParam(RenderParamType type, float value);
        void        setProgressSignalHandler(CProgressSignalHandler* pHandler);

        void        execute();

    private:

        void        emitSetProgressSteps(size_t steps);
        void        emitSetProgressMsg(const std::string &msg);
        void        emitDoProgress();

    private:

        std::unique_ptr<ocl::COpenCL>   m_pOclObj = nullptr;
        bool                            m_bInteropGLCL = false;
        CMat                            m_inputData;
        CMat                            m_inputData8bit;
        CMat                            m_binaryData;
        // CL Objects
        Img3DPtr                        m_bufInput = nullptr;
        Img3DPtr                        m_bufPinnedInput = nullptr;
        Img2DPtr                        m_bufTransfertFunction = nullptr;
        BufPtr                          m_bufInvViewMatrix = nullptr;
        BufPtr                          m_bufPinnedInvViewMatrixMap = nullptr;
        BufPtr                          m_bufModelViewProjMatrix = nullptr;
        BufPtr                          m_bufPinnedModelViewProjMatrixMap = nullptr;
        BufPtr                          m_bufRayParam = nullptr;
        SamplerPtr                      m_volumeSamplerLinear = nullptr;
        SamplerPtr                      m_volumeSamplerNearest = nullptr;
        SamplerPtr                      m_transferFuncSampler = nullptr;
        std::vector<cl::Memory>         m_pbo_cl;
        QMatrix4x4                      m_viewMatrix;
        QMatrix4x4                      m_modelMatrix;
        QMatrix4x4                      m_projMatrix;
        size_t                          m_kernelId = 0;
        std::unique_ptr<CRayParam>      m_pRayParam = nullptr;
        uchar*                          m_pHostBuffer = nullptr;
        QMatrix4x4                      m_invViewMatrix;
        QMatrix4x4                      m_modelViewProj;
        float*                          m_pInvViewMatrix = nullptr;
        float*                          m_pModelViewProjMatrix = nullptr;
        long                            m_localSizeX = 16;
        long                            m_localSizeY = 16;
        size_t                          m_gridSize[2];
        bool                            m_bColorMap = false;
        bool                            m_bPinned =false;
        bool                            m_bLinearFiltering = true;
        size_t                          m_nbBand = 1;
        size_t                          m_textureWidth;
        size_t                          m_textureHeight;
        size_t                          m_nbImages;
        size_t                          m_wndWidth = 0;
        size_t                          m_wndHeight = 0;
        bool                            m_bIsInit = false;
        RenderMode                      m_mode = RenderMode::RAYCAST;
        void*                           m_transferFuncData = nullptr;
        //Progress bar
        CProgressSignalHandler*         m_pProgressSignalHandler = nullptr;
};

#endif // VOLUMERENDER_H
