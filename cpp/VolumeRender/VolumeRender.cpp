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

#include <stdlib.h>
#include "VolumeRender.h"
#include "Kernels/raycastGray.h"
#include "Kernels/raycastColor.h"
#include "CProgressSignalHandler.h"

constexpr double pi = 3.14159265358979323846;

// create transfer function texture in BGRA format
uchar transferFuncClassic[36] = {
                      0,   0,   0,   0,
                     76,  76,  76,  30,
                    102, 102, 102,  60,
                    127, 127, 127,  90,
                    153, 153, 153, 120,
                    178, 178, 178, 150,
                    204, 204, 204, 180,
                    229, 229, 229, 210,
                    255, 255, 255, 255
                    };

uchar transferFuncGraySkin[36] = {
                      0,   0,   0,   0,
                     70,  93, 147,  30,
                    155, 178, 232,  60,
                      0,   0,   0,  90,
                     42,  71,  71, 120,
                     85, 114, 114, 150,
                    190, 228, 228, 180,
                    217, 255, 255, 210,
                    217, 255, 255, 255,
                    };

uchar transferFuncGraySkin_[36] = {
                      0,   0,   0,   0,
                      0,   0, 128,  30,
                     41,  56, 159,  60,
                     70,  90, 255,  90,
                      0,   0, 255, 120,
                      0, 255, 255, 150,
                    217, 255, 255, 180,
                    217, 255, 255, 210,
                    255, 255, 255, 255,
                    };

uchar transferFuncGrayJet[36] = {
                      0,   0,   0,   0,
                    255,   0,   0,  30,
                    255,  51,   0,  60,
                    255, 127,   0,  90,
                    255, 255,   0, 120,
                    127, 255, 127, 150,
                      0, 255, 255, 180,
                      0, 127, 255, 210,
                      0, 127, 255, 255
                    };

uchar transferFuncGrayColor[36] = {
                      0,   0,   0,   0,
                      0,   0, 255,  30,
                      0, 127, 255,  60,
                      0, 255, 255,  90,
                      0, 255,   0, 120,
                    255, 255,   0, 150,
                    255,   0,   0, 180,
                    255,   0, 255, 210,
                    255,   0, 255, 255
                    };

CVolumeRender::CVolumeRender()
{
    setParameters();
}

CVolumeRender::~CVolumeRender()
{
    if(m_pHostBuffer)
        ocl::utils::aligned_free(m_pHostBuffer);
}

RenderError CVolumeRender::initCL(const std::string name)
{
    try
    {
        m_pOclObj = std::make_unique<ocl::COpenCL>(CL_DEVICE_TYPE_GPU, m_bInteropGLCL);
        m_pOclObj->init(name);
    }
    catch(cl::Error& e)
    {
        Utils::print(e.what() + std::string("(") + ocl::utils::getOpenCLErrorCodeStr(e.err()) + std::string(")"), QtCriticalMsg);
        return RenderError::INIT_FAILED;
    }
    return RenderError::SUCCESS;
}

void CVolumeRender::init()
{
    assert(m_pOclObj);

    m_bIsInit = false;
    // Clean memory
    m_pOclObj->cleanProgramAndKernel();
    // Init host buffer and fill it with source volume and segmentation if needed
    initHostBuffer();
    fillHostBuffer();
    fillSegmentationStack();
    // Init GPU memory
    initStandardHostMemory();
    initDeviceBuffer();
    // Init OpenCL program
    initProgram();
    // Init all OpenCL kernels with args
    initKernel();
    initKernelArg();
    // Transfer memory to GPU
    transferHostToDevice();
    m_bIsInit = true;
}

void CVolumeRender::initDeviceBuffer()
{
    assert(m_pOclObj);

    cl::ImageFormat transferFunc_format;
    transferFunc_format.image_channel_order = CL_BGRA;
    transferFunc_format.image_channel_data_type = CL_UNORM_INT8;

    // Create samplers for transfer function, linear interpolation and nearest interpolation
    m_transferFuncSampler	= std::make_unique<cl::Sampler>(m_pOclObj->getContext(), true, CL_ADDRESS_CLAMP_TO_EDGE, CL_FILTER_LINEAR);
    m_volumeSamplerLinear	= std::make_unique<cl::Sampler>(m_pOclObj->getContext(), true, CL_ADDRESS_CLAMP_TO_EDGE, CL_FILTER_LINEAR);
    m_volumeSamplerNearest	= std::make_unique<cl::Sampler>(m_pOclObj->getContext(), true, CL_ADDRESS_CLAMP_TO_EDGE, CL_FILTER_NEAREST);

    // init invViewMatrix
    m_bufInvViewMatrix		= std::make_unique<cl::Buffer>(m_pOclObj->getContext(), CL_MEM_READ_ONLY, 16 * sizeof(float));
    // init modelview proj matrix
    m_bufModelViewProjMatrix = std::make_unique<cl::Buffer>(m_pOclObj->getContext(), CL_MEM_READ_ONLY, 16 * sizeof(float));

    m_bufRayParam = std::make_unique<cl::Buffer>(m_pOclObj->getContext(), CL_MEM_READ_ONLY, sizeof(CRayParam));

    m_bufTransfertFunction = std::make_unique<cl::Image2D>(m_pOclObj->getContext(), CL_MEM_READ_ONLY, transferFunc_format, 9, 1);

    /* Commented - if(m_bPinned)
    {
        m_bufPinnedInvViewMatrixMap  = std::make_unique<BufPtr(m_pOclObj->getContext(), CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR, 16 * sizeof(float), 0);
        m_pInvViewMatrix = (float*)m_pRayRender->getQueue().enqueueMapBuffer(*m_bufPinnedInvViewMatrixMap, CL_FALSE, CL_MAP_WRITE, 0, 16*sizeof(float));

        m_bufPinnedModelViewProjMatrixMap  = std::make_unique<BufPtr(m_pOclObj->getContext(), CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR, 16 * sizeof(float), 0);
        m_pModelViewProjMatrix = (float*)m_pOclObj->getQueue().enqueueMapBuffer(*m_bufPinnedModelViewProjMatrixMap, CL_FALSE, CL_MAP_WRITE, 0, 16*sizeof(float));
    }*/
}

void CVolumeRender::initHostBuffer()
{
    assert(m_inputData.data);

    m_textureHeight = m_inputData.getNbRows();
    m_textureWidth = m_inputData.getNbCols();
    m_nbImages = m_inputData.getNbStacks();
    m_nbBand = m_inputData.channels();

    convertTo8bits(m_inputData, m_inputData8bit);

    size_t size = 4 * m_textureWidth * m_textureHeight * m_nbImages * sizeof(uchar);
    m_pHostBuffer = (uchar*)ocl::utils::aligned_malloc(size, 16);
    memset(m_pHostBuffer, 0, size);

    //Progress initialization
    emitSetProgressSteps(m_nbImages);
}

void CVolumeRender::initStandardHostMemory()
{
    assert(m_pOclObj);

    // Init buffers in host memory (pageable, non-pinned)
    cl::ImageFormat volume_format;

    volume_format.image_channel_order = CL_BGRA;
    volume_format.image_channel_data_type = CL_UNORM_INT8;

    m_bufInput = std::make_unique<cl::Image3D>(m_pOclObj->getContext(),
                    CL_MEM_READ_ONLY,
                    volume_format,
                    m_textureWidth,
                    m_textureHeight,
                    m_nbImages
                    );
}

void CVolumeRender::initProgram()
{
    assert(m_pOclObj);

    float realMaxXYZ = std::max<float>(m_textureWidth, std::max<float>(m_textureHeight, m_nbImages));
    float alpha  = 2.0f * m_textureWidth  / realMaxXYZ;
    float beta = 2.0f * m_textureHeight / realMaxXYZ;
    float delta  = 2.0f * m_nbImages  / realMaxXYZ;

    // Set compiler options parameters
    //OCLoptions = (std::string)"-cl-nv-verbose "; // Enables debug information
    //OCLoptions += (std::string)"-cl-kernel-arg-info ";
    std::string OCLoptions = (std::string)"-cl-mad-enable "; // Enables mad(x,y,z) = x + y*z
    OCLoptions += (std::string)"-cl-fast-relaxed-math "; // Enables fast maths optimization
    OCLoptions += (std::string)"-Ddelta=" + std::to_string((float)delta/2.0f);
    OCLoptions += (std::string)" -Dalpha=" + std::to_string((float)alpha/2.0f);
    OCLoptions += (std::string)" -Dbeta=" + std::to_string((float)beta/2.0f);
    OCLoptions += (std::string)" -Dsampling=" + std::to_string(std::max<size_t>(std::max<size_t>(m_textureWidth, m_textureHeight), m_nbImages));
    OCLoptions += (std::string)" -DTSTEP=" + std::to_string(1.0f / 2048.0f);

    // Optimization using compiler options
    std::replace(OCLoptions.begin(), OCLoptions.end(), ',', '.');

    ocl::buildProgramData buildData;
    if(m_nbBand == 1)
        buildData.source = kernelRaycastGray;
    else
        buildData.source = kernelRaycastColor;

    buildData.flagsStr = OCLoptions;
    m_pOclObj->initProgram(buildData);
}

void CVolumeRender::initKernel()
{
    assert(m_pOclObj);

    if(m_nbBand == 1)
    {
        m_pOclObj->initKernelList(static_cast<size_t>(RenderMode::RAYCAST), "renderGray_F2B"); // id = 0
        m_pOclObj->initKernelList(static_cast<size_t>(RenderMode::MIP), "renderMIPGray_F2B"); // id = 1
        m_pOclObj->initKernelList(static_cast<size_t>(RenderMode::ISOSURFACE), "renderIso_F2B"); // id = 2
        m_pOclObj->initKernelList(static_cast<size_t>(RenderMode::MinIP), "renderMinIPGray_F2B"); // id = 3
    }
    else
    {
        m_pOclObj->initKernelList(static_cast<size_t>(RenderMode::RAYCAST), "renderColor_F2B"); // id = 0
        m_pOclObj->initKernelList(static_cast<size_t>(RenderMode::MIP), "renderMIPColor_F2B"); // id = 1
        m_pOclObj->initKernelList(static_cast<size_t>(RenderMode::ISOSURFACE), "renderIso_F2B"); // id = 2
        m_pOclObj->initKernelList(static_cast<size_t>(RenderMode::MinIP), "renderMinIPColor_F2B"); // id = 3
    }
    setRenderMode(m_mode);
}

void CVolumeRender::initKernelArg()
{
    assert(m_pOclObj);

    if(m_pbo_cl.size()<2)
        throw cl::Error(CL_INVALID_KERNEL_ARGS, "Invalid PBO's");

    // set image and sampler args
    for(size_t id=0; id<m_pOclObj->getNbKernel(); ++id)
    {
        m_pOclObj->getKernel(id).setArg(0, sizeof(cl::Memory), (void*)&m_pbo_cl.front());
        m_pOclObj->getKernel(id).setArg(1, sizeof(cl::Memory), (void*)m_bufInvViewMatrix.get());
        m_pOclObj->getKernel(id).setArg(2, sizeof(cl::Memory), (void*)m_bufInput.get());
        m_pOclObj->getKernel(id).setArg(3, sizeof(cl::Memory), (void*)m_bufTransfertFunction.get());
        m_pOclObj->getKernel(id).setArg(4, sizeof(cl::Sampler), m_bLinearFiltering ? m_volumeSamplerLinear.get() : m_volumeSamplerNearest.get());
        m_pOclObj->getKernel(id).setArg(5, sizeof(cl::Sampler), (void*)m_transferFuncSampler.get());
        m_pOclObj->getKernel(id).setArg(6, sizeof(cl::Memory), (void*)&m_pbo_cl.back());
        m_pOclObj->getKernel(id).setArg(7, sizeof(cl::Memory), (void*)m_bufModelViewProjMatrix.get());
        m_pOclObj->getKernel(id).setArg(8, sizeof(cl::Memory), (void*)m_bufRayParam.get());
    }
}

void CVolumeRender::initPBO(GLuint pbo, GLuint pbo_depth)
{
    assert(m_pOclObj);

    m_pbo_cl.clear();

    // create OpenCL buffer from GL PBO
    m_pbo_cl.push_back(cl::BufferGL(m_pOclObj->getContext(), CL_MEM_WRITE_ONLY, pbo));
    m_pbo_cl.push_back(cl::BufferGL(m_pOclObj->getContext(), CL_MEM_WRITE_ONLY, pbo_depth));
}

void CVolumeRender::setParameters()
{
    //Ray parameters
    m_pRayParam = std::make_unique<CRayParam>();
    m_pRayParam->m_density              = 0.05f;
    m_pRayParam->m_brightness			= 1.0f;
    m_pRayParam->m_transferOffset		= 0.0f;
    m_pRayParam->m_transferScale		= 1.0f;
    m_pRayParam->m_chR[0]				= 0.0f;
    m_pRayParam->m_chG[0]				= 0.0f;
    m_pRayParam->m_chB[0]				= 0.0f;
    m_pRayParam->m_chR[1]				= 1.0f;
    m_pRayParam->m_chG[1]				= 1.0f;
    m_pRayParam->m_chB[1]               = 1.0f;
    m_pRayParam->m_clipPlaneCenter[0]   = 0.0f;
    m_pRayParam->m_clipPlaneCenter[1]   = 1.0f;
    m_pRayParam->m_clipPlaneCenter[2]   = 0.0f;
    m_pRayParam->m_clipPlaneCenter[3]   = 1.0f;
    m_pRayParam->m_clipPlaneCenter[4]   = 0.0f;
    m_pRayParam->m_clipPlaneCenter[5]   = 1.0f;
    m_pRayParam->m_isoValue[0]          = 0.0f;
    m_pRayParam->m_isoValue[1]          = 1.0f;
    m_pRayParam->m_isoValue[2]          = 0.0f;
    m_pRayParam->m_isoValue[3]          = 1.0f;

    updateWndParameters();

    //Default color map
    m_transferFuncData = transferFuncClassic;
}

void CVolumeRender::updateWndParameters()
{
    if(m_pRayParam)
    {
        size_t maxWH;
        float ratioX, ratioY;

        // Adapting OCL display to the window size ratio
        if (m_wndWidth>m_wndHeight)
        {
            ratioX = 1.0f;
            ratioY = (float)m_wndHeight/(float)m_wndWidth;
            maxWH = m_wndWidth;
        }
        else
        {
            ratioX = (float)m_wndWidth/(float)m_wndHeight;
            ratioY = 1.0f;
            maxWH = m_wndHeight;
        }
        m_pRayParam->m_wndHeight = (float)m_wndHeight;
        m_pRayParam->m_wndWidth = (float)m_wndWidth;
        m_pRayParam->m_K = 2.0f/(float)maxWH;
        m_pRayParam->m_ratioX = ratioX;
        m_pRayParam->m_ratioY = ratioY;
        m_pRayParam->m_focalLength = ratioY/std::tan((float)pi/8.0f);
    }
}

void CVolumeRender::fillSegmentationStack()
{
    if(m_binaryData.empty())
        return;

    if(m_nbBand == 1)
    {
        for(size_t z=0; z<m_nbImages; ++z)
        {
            CMat binZ = m_binaryData.getPlane(z);

            for(size_t y=0; y<m_textureHeight; ++y)
            {
                auto binY = binZ.ptr<uchar>((int)y);

                for(size_t x=0; x<m_textureWidth; ++x)
                {
                    auto index = ((z*m_textureHeight + y)*m_textureWidth + x) * 4;
                    m_pHostBuffer[index+2] = static_cast<uchar>(binY[x]);
                }
            }
            emitDoProgress();
        }
    }
    else
    {
        for(size_t z=0; z<m_nbImages; ++z)
        {
            CMat binZ = m_binaryData.getPlane(z);

            for(size_t y=0; y<m_textureHeight; ++y)
            {
                auto binY = binZ.ptr<uchar>((int)y);

                for(size_t x=0; x<m_textureWidth; ++x)
                {
                    auto index = ((z*m_textureHeight + y)*m_textureWidth + x) * 4;
                    m_pHostBuffer[index+3] = static_cast<uchar>(binY[x]);
                }
            }
            emitDoProgress();
        }
    }

}

void CVolumeRender::fillGrayStack()
{
    for(size_t z=0; z<m_nbImages; ++z)
    {
        CMat tmpZ = m_inputData8bit.getPlane(z);
        for(size_t y=0; y<m_textureHeight; ++y)
        {
            auto ptrY = tmpZ.ptr<uchar>((int)y);
            for(size_t x=0; x<m_textureWidth; ++x)
            {
                auto index = ((z*m_textureHeight + y)*m_textureWidth + x) * 4;
                m_pHostBuffer[index+3] = static_cast<uchar>(ptrY[x]);
            }
        }
        emitDoProgress();
    }
}

void CVolumeRender::fillColorStack()
{
    for(size_t z=0; z<m_nbImages; ++z)
    {
        CMat tmpZ = m_inputData8bit.getPlane(z);
        for(size_t y=0; y<m_textureHeight; ++y)
        {
            auto ptrY = tmpZ.ptr<uchar>((int)y);
            for(size_t x=0; x<m_textureWidth; ++x)
            {
                auto index = ((z*m_textureHeight + y)*m_textureWidth + x) * 4;
                // CMat image is CV_8UC3 (3 channels) and is BGR
                m_pHostBuffer[index] = static_cast<uchar>(ptrY[x*3+2]);
                m_pHostBuffer[index+1] = static_cast<uchar>(ptrY[x*3+1]);
                m_pHostBuffer[index+2] = static_cast<uchar>(ptrY[x*3]);
            }
        }
        emitDoProgress();
    }
}

void CVolumeRender::fillHostBuffer()
{
    if(m_nbBand == 1)
        fillGrayStack();
    else
        fillColorStack();
}

void CVolumeRender::transferHostToDevice()
{
    assert(m_pOclObj);

    cl::array<size_t, 3> origin = {0, 0, 0};
    cl::array<size_t, 3> region = {9, 1, 1};
    cl::array<size_t, 3> imageSize = {m_textureWidth, m_textureHeight, m_nbImages};

    // Transfer color map array
    m_pOclObj->getQueue().enqueueWriteImage(*m_bufTransfertFunction, CL_TRUE, origin, region, 9 * 4, 0, (m_nbBand>1) ? transferFuncClassic : m_transferFuncData);

    // Transfer main buffer
    cl::Buffer bufInput(m_pOclObj->getContext(), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, m_textureWidth * m_textureHeight * m_nbImages * 4, m_pHostBuffer);
    m_pOclObj->getQueue().enqueueCopyBufferToImage(bufInput, *m_bufInput, 0, origin, imageSize);

    if(m_bPinned)
        m_pOclObj->getQueue().enqueueUnmapMemObject(*m_bufPinnedInput, m_pHostBuffer);
    else
    {
        if(m_pHostBuffer)
        {
            ocl::utils::aligned_free(m_pHostBuffer);
            m_pHostBuffer = nullptr;
        }
    }
}

void CVolumeRender::convertTo8bits(CMat& src, CMat& dst)
{
    assert(src.data != nullptr);

    switch(src.type())
    {
        case CV_8UC1:
        case CV_8UC3:
        case CV_8UC4:
            dst = src;
            break;
        case CV_16SC1:
        case CV_16SC3:
        case CV_16SC4:
        case CV_16UC1:
        case CV_16UC3:
        case CV_16UC4:
        case CV_32SC1:
        case CV_32SC3:
        case CV_32SC4:
        case CV_32FC1:
        case CV_32FC3:
        case CV_32FC4:
        case CV_64FC1:
        case CV_64FC3:
        case CV_64FC4:
            cv::normalize(src, dst, 255, 0, cv::NORM_MINMAX, CV_8UC(src.channels()));
            break;
        default:
            dst = src;
            break;
    }
}

void CVolumeRender::setInputData(CMat inputData)
{
    m_inputData = inputData;
}

void CVolumeRender::setBinaryData(CMat binaryData)
{
    m_binaryData = binaryData;
}

void CVolumeRender::setCLGLInterop(bool bEnable)
{
    m_bInteropGLCL = bEnable;
}

void CVolumeRender::setMatrix(QMatrix4x4& model, QMatrix4x4& view, QMatrix4x4& projection)
{
    m_modelMatrix = model;
    m_viewMatrix = view;
    m_projMatrix = projection;

    m_modelViewProj = projection * view * model;

    QMatrix4x4 modelView = view * model;
    float* invV = m_invViewMatrix.data();
    float* MV = modelView.data();

    invV[0]  = MV[0];
    invV[1]  = MV[1];
    invV[2]  = MV[2];
    invV[3]  = -(MV[0]*MV[12] + MV[1]*MV[13] + MV[2]*MV[14]);
    invV[4]  = MV[4];
    invV[5]  = MV[5];
    invV[6]  = MV[6];
    invV[7]  = -(MV[4]*MV[12] + MV[5]*MV[13] + MV[6]*MV[14]);
    invV[8]  = MV[8];
    invV[9]  = MV[9];
    invV[10] = MV[10];
    invV[11] = -(MV[8]*MV[12] + MV[9]*MV[13] + MV[10]*MV[14]);
    invV[12] = MV[3];
    invV[13] = MV[7];
    invV[14] = MV[11];
    invV[15] = MV[15];
}

void CVolumeRender::setWndSize(size_t width, size_t height)
{
    m_wndWidth = width;
    m_wndHeight = height;

    // Update window parameters for kernels
    updateWndParameters();
    // Update local and global sizes for running kernels
    setLocalGlobalWorkSize();
}

void CVolumeRender::setLocalGlobalWorkSize()
{
    assert(m_pOclObj);

    if(m_pOclObj->getNbKernel()>0)
    {
        // Calculate local size depending on the kernel
        long maxLocalSize = (long)m_pOclObj->getKernel(m_kernelId).getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(m_pOclObj->getDevice());
        long warpSize = (long)m_pOclObj->getKernel(m_kernelId).getWorkGroupInfo<CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE>(m_pOclObj->getDevice());

        m_localSizeX = maxLocalSize/warpSize;
        m_localSizeY = warpSize;

        m_gridSize[0] = ocl::utils::shrRoundUp(m_localSizeX,m_wndWidth);
        m_gridSize[1] = ocl::utils::shrRoundUp(m_localSizeY,m_wndHeight);
    }
}

void CVolumeRender::setRenderMode(RenderMode mode)
{
    m_mode = mode;
    m_kernelId = static_cast<size_t>(mode);
    setLocalGlobalWorkSize();
}

void CVolumeRender::setColormap(RenderColormap colormap)
{
    assert(m_pOclObj);

    switch(colormap)
    {
        case RenderColormap::CLASSIC:
            m_transferFuncData = transferFuncClassic;
            break;
        case RenderColormap::SKIN:
            m_transferFuncData = transferFuncGraySkin;
            break;
        case RenderColormap::JET:
            m_transferFuncData = transferFuncGrayJet;
            break;
        case RenderColormap::GRAYCOLOR:
            m_transferFuncData = transferFuncGrayColor;
            break;
        default:
            return;

    }
    cl::array<size_t, 3> origin = {0, 0, 0};
    cl::array<size_t, 3> region = {9, 1, 1};

    // Transfer color map array
    m_pOclObj->getQueue().enqueueWriteImage(*m_bufTransfertFunction, CL_TRUE, origin, region, 9 * 4, 0, (m_nbBand>1) ? transferFuncClassic : m_transferFuncData);
}

void CVolumeRender::setRayParam(RenderParamType type, float value)
{
    assert(m_pRayParam);
    m_pRayParam->setParam(type, value);
}

void CVolumeRender::setProgressSignalHandler(CProgressSignalHandler *pHandler)
{
    m_pProgressSignalHandler = pHandler;
}

void CVolumeRender::execute()
{
    assert(m_pOclObj);

    if(m_bIsInit == false)
        return;

    m_pOclObj->getQueue().enqueueAcquireGLObjects(&m_pbo_cl);

    if(m_bPinned)
    {
        memcpy(m_pInvViewMatrix, m_invViewMatrix.data(), 16*sizeof(float));
        m_pOclObj->getQueue().enqueueWriteBuffer(*m_bufInvViewMatrix, CL_FALSE, 0, 16*sizeof(float), m_pInvViewMatrix, 0, 0);
    }
    else
        m_pOclObj->getQueue().enqueueWriteBuffer(*m_bufInvViewMatrix, CL_FALSE, 0, 16*sizeof(float), m_invViewMatrix.data(), 0, 0);

    if(m_bPinned)
    {
        memcpy(m_pModelViewProjMatrix, m_modelViewProj.data(), 16*sizeof(float));
        m_pOclObj->getQueue().enqueueWriteBuffer(*m_bufModelViewProjMatrix, CL_FALSE, 0, 16*sizeof(float), m_pModelViewProjMatrix, 0, 0);
    }
    else
        m_pOclObj->getQueue().enqueueWriteBuffer(*m_bufModelViewProjMatrix, CL_FALSE, 0, 16*sizeof(float), m_modelViewProj.data(), 0, 0);

    // execute OpenCL kernel, writing results to PBO
    m_pOclObj->getQueue().enqueueWriteBuffer(*m_bufRayParam, CL_TRUE, 0, sizeof(CRayParam), m_pRayParam.get(), 0, 0);
    m_pOclObj->getQueue().enqueueNDRangeKernel(m_pOclObj->getKernel(m_kernelId), cl::NullRange, cl::NDRange(m_gridSize[0], m_gridSize[1]), cl::NDRange(m_localSizeX, m_localSizeY), 0, 0);

    // Transfer ownership of buffer back from CL to GL
    m_pOclObj->getQueue().enqueueReleaseGLObjects(&m_pbo_cl);
    m_pOclObj->getQueue().finish();
}

void CVolumeRender::emitSetProgressSteps(size_t steps)
{
    if(m_pProgressSignalHandler)
        m_pProgressSignalHandler->emitSetTotalSteps(static_cast<int>(steps));
}

void CVolumeRender::emitSetProgressMsg(const std::string &msg)
{
    if(m_pProgressSignalHandler)
        m_pProgressSignalHandler->emitSetMessage(msg);
}

void CVolumeRender::emitDoProgress()
{
    if(m_pProgressSignalHandler)
        m_pProgressSignalHandler->emitProgress();
}








