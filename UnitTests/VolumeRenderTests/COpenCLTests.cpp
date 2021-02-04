#include <QTest>
#include <QPainter>
#include "COpenCLTests.h"
#include "UnitTestUtils.hpp"
#include "kernels.hpp"

using namespace Ikomia;

#define M_PI 3.1415926535897932

COpenCLTests::COpenCLTests(QObject* parent) : QObject(parent)
{

}

void COpenCLTests::onInitOpenCLOpenGL()
{
    QGLTestWidget clglWidget;
    clglWidget.show();
}

void COpenCLTests::onPerformHelloWorld()
{
    std::string kernelSrc = R"(
                            __kernel void helloworld(__global char* in, __global char* out)
                            {
                                int num = get_global_id(0);
                                out[num] = in[num];
                            }
                            )";

    cl::Program testProgram;
    ocl::buildProgramData buildData;
    buildData.source = kernelSrc;

    short err = SDK_SUCCESS;
    std::unique_ptr<ocl::COpenCL> pOclObj = std::make_unique<ocl::COpenCL>();
    err = pOclObj->init();
    QVERIFY(err == SDK_SUCCESS);

    err = pOclObj->buildProgram(testProgram, buildData);
    QVERIFY(err == SDK_SUCCESS);

    //Initial input,output for the host and create memory objects for the kernel
    std::string input = "Hello World!";
    size_t strlength = input.size();
    qDebug() << "input : " << QString::fromStdString(input);
    std::string output;
    output.resize(strlength);

    try
    {
        cl::Buffer inputBuffer(pOclObj->getContext(), CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, strlength * sizeof(char), &input[0], NULL);
        cl::Buffer outputBuffer(pOclObj->getContext(), CL_MEM_WRITE_ONLY , strlength * sizeof(char), NULL, NULL);

        //Create kernel object
        cl::Kernel kernel(testProgram, "helloworld");

        //Sets Kernel arguments.
        kernel.setArg(0, inputBuffer);
        kernel.setArg(1, outputBuffer);

        //Running the kernel.
        cl::NDRange global(strlength);
        pOclObj->getQueue().enqueueNDRangeKernel(kernel, cl::NullRange, global);

        //Read the cout put back to host memory.
        pOclObj->getQueue().enqueueReadBuffer(outputBuffer, CL_TRUE, 0, (strlength) * sizeof(char), &output[0]);
        pOclObj->getQueue().finish();
    }
    catch(cl::Error& e)
    {
        qDebug() << e.what() << "(" << e.err() << ")" ;
    }
    QVERIFY(output.size()>0);
    qDebug() << "output : " << QString::fromStdString(output);
    QVERIFY(input == output);
}

void COpenCLTests::onSmoothOpencvImg()
{
    cl::Program testProgram;
    ocl::buildProgramData buildData;
    buildData.source = kernelBoxFilter;

    auto inputImg = loadSampleImage();
    cv::cvtColor(inputImg, inputImg, cv::COLOR_RGB2RGBA);

    QVERIFY(!inputImg.empty());

    short err = SDK_SUCCESS;

    bool bInteropGLCL = false;
    std::unique_ptr<ocl::COpenCL> pOclObj = std::make_unique<ocl::COpenCL>(CL_DEVICE_TYPE_GPU, bInteropGLCL);
    err = pOclObj->init();
    QVERIFY(err == SDK_SUCCESS);

    err = pOclObj->buildProgram(testProgram, buildData);
    QVERIFY(err == SDK_SUCCESS);

    auto width = inputImg.cols;
    auto height = inputImg.rows;

    // Input and output Buffers (uchar4)
    auto inputBuf = std::make_unique<cl_uchar4[]>(width*height);
    auto outputBuf = std::make_unique<cl_uchar4[]>(width*height);

    // Init buffers
    memset(outputBuf.get(), 0, width * height * sizeof(cl_uchar4));
    memcpy(inputBuf.get(), inputImg.data, width * height * sizeof(cl_uchar4));

    try
    {
        // Create memory object for input Image
        cl::Buffer inputImageBuffer(pOclObj->getContext(), CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, width * height * sizeof(cl_uchar4), inputBuf.get());
        cl::Buffer tempImageBuffer(pOclObj->getContext(), CL_MEM_READ_WRITE, width * height * sizeof(cl_uchar4));
        cl::Buffer outputImageBuffer(pOclObj->getContext(), CL_MEM_WRITE_ONLY, width * height * sizeof(cl_uchar4));

        cl::Kernel verticalKernel(testProgram,"box_filter_vertical");
        cl::Kernel horizontalKernel(testProgram,"box_filter_horizontal");

        horizontalKernel.setArg(0, inputImageBuffer);
        horizontalKernel.setArg(1, tempImageBuffer);
        horizontalKernel.setArg(2, 8); // filter width

        cl::NDRange globalThreads(width, height);
        cl::NDRange localThreads(4, 4);

        std::vector<cl::Event> events;
        cl::Event ev1;
        pOclObj->getQueue().enqueueNDRangeKernel(horizontalKernel, cl::NullRange, globalThreads, localThreads, nullptr, &ev1);
        events.push_back(ev1);
        pOclObj->getQueue().flush();
        cl::WaitForEvents(events);

        verticalKernel.setArg(0, tempImageBuffer);
        verticalKernel.setArg(1, outputImageBuffer);
        verticalKernel.setArg(2, 8);

        cl::Event ev2;
        pOclObj->getQueue().enqueueNDRangeKernel(verticalKernel, cl::NullRange, globalThreads, localThreads, nullptr, &ev2);
        events.push_back(ev2);
        pOclObj->getQueue().flush();
        cl::WaitForEvents(events);

        cl::Event ev3;
        pOclObj->getQueue().enqueueReadBuffer(outputImageBuffer, CL_FALSE, 0, width * height * sizeof(cl_uchar4), outputBuf.get(), nullptr, &ev3);
        events.push_back(ev3);
        pOclObj->getQueue().flush();
        cl::WaitForEvents(events);
    }
    catch(cl::Error& e)
    {
        qDebug() << e.what() << "(" << QString::fromStdString(ocl::utils::getOpenCLErrorCodeStr(e.err()))<< ")" ;
    }

    CMat results = inputImg.clone();
    memcpy(results.data, outputBuf.get(), width * height * sizeof(cl_uchar4));

    cv::imshow("Original", inputImg);
    cv::imshow("onSmoothOpencvImg", results);
    cv::waitKey(0);
}

void COpenCLTests::onSmoothOpencvImgGL()
{
    m_image = loadSampleImage();
    CMat dst;
    cv::cvtColor(m_image, dst, cv::COLOR_RGB2BGRA);
    QVERIFY(!dst.empty());

    QGLTestWidget clglWidget;
    clglWidget.setImage(dst);
    clglWidget.show();
    cv::imshow("Original", m_image);
    cv::waitKey(0);
}

CMat COpenCLTests::loadSampleImage()
{
    std::string imagePath = UnitTest::getDataPath() + "/Images/Lena.png";
    CMat img = cv::imread(imagePath, cv::IMREAD_UNCHANGED);
    return img;
}

void COpenCLTests::onInitOpenCLOnly()
{
    short err = SDK_SUCCESS;

    bool bInteropGLCL = false;
    auto pOclObj = std::make_unique<ocl::COpenCL>(CL_DEVICE_TYPE_GPU, bInteropGLCL);
    err = pOclObj->init();
    QVERIFY(err == SDK_SUCCESS);
}

void QGLTestWidget::initCLGL()
{
    try
    {
        makeCurrent();
        m_pRender = std::make_unique<CRender>();
        m_pRender->setCLGLInterop(true);
        auto err = m_pRender->initCL();
        QVERIFY(err == RenderError::SUCCESS);
    }
    catch(cl::Error& e)
    {
        qDebug() << e.what();
    }
}

void QGLTestWidget::perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble xmin, xmax, ymin, ymax;

    ymax = zNear * tan( fovy * M_PI / 360.0 );
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustum( xmin, xmax, ymin, ymax, zNear, zFar );
}

void QGLTestWidget::smoothOpencvImg()
{
    if(m_image.empty())
        return;

    try
    {
        glGenTextures(1, &m_tex);
        glBindTexture(GL_TEXTURE_2D, m_tex);

        // Set parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA, m_width, m_height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        //Create pixel-buffer object
        glGenBuffers(1, &m_pbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_pbo);

        // initialize buffer object
        unsigned int size = m_width * m_height * sizeof(cl_uchar4);

        // buffer data
        glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Run CL kernels

        m_pRender->setInputData(m_image);
        m_pRender->execute(m_pbo);
    }
    catch(cl::Error& e)
    {
        qDebug() << e.what() << "(" << QString::fromStdString(ocl::utils::getOpenCLErrorCodeStr(e.err()))<< ")" ;
    }
}

void QGLTestWidget::setImage(CMat image)
{
    m_image = image;
    m_width = image.cols;
    m_height = image.rows;
    resize(m_width, m_height);
}

void QGLTestWidget::initializeGL()
{
    initializeOpenGLFunctions();
    initCLGL();

    //Init GL
    glClearColor(0.1f, 0.15f, 0.20f, 1.0f);
    glDisable(GL_DEPTH_TEST);
    try
    {
        smoothOpencvImg();
    }
    catch(std::exception& e)
    {
        qDebug() << e.what();
    }
}

void QGLTestWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    perspective(
        60.0,
        (GLfloat)w / (GLfloat)h,
        0.1,
        10.0);
}

void QGLTestWidget::paintGL()
{
    // Bind PBO and texture
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, m_pbo);
    glBindTexture(GL_TEXTURE_2D, m_tex);

    // Copy pixels from pbo to texture
    glTexSubImage2D(GL_TEXTURE_2D,
                    0,
                    0,
                    0,
                    m_width,
                    m_height,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    NULL);

    // Display image using texture
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode( GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, m_width, m_height);

    // Begin drawing texture
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1.0, 1.0, 0.5);

    glTexCoord2f(1.0, 0.0);
    glVertex3f(1.0, 1.0, 0.5);

    glTexCoord2f(1.0, 1.0);
    glVertex3f(1.0, -1.0, 0.5);

    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1.0, -1.0, 0.5);

    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

void CRender::execute(GLuint& pbo)
{
    cl::Program program;
    ocl::buildProgramData buildData;
    buildData.source = kernelBoxFilter;

    m_pOclObj->buildProgram(program, buildData);

    auto width = m_inputData.cols;
    auto height = m_inputData.rows;

    // Input and output Buffers (uchar4)
    auto inputBuf = std::make_unique<cl_uchar4[]>(width*height);
    auto outputBuf = std::make_unique<cl_uchar4[]>(width*height);

    // Init buffers
    memset(outputBuf.get(), 0, width * height * sizeof(cl_uchar4));
    memcpy(inputBuf.get(), m_inputData.data, width * height * sizeof(cl_uchar4));
    try
    {
        // Create memory object for input Image
        cl::Buffer inputImageBuffer(m_pOclObj->getContext(), CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, width * height * sizeof(cl_uchar4), inputBuf.get());
        cl::Buffer tempImageBuffer(m_pOclObj->getContext(), CL_MEM_READ_ONLY, width * height * sizeof(cl_uchar4));
        cl::BufferGL outputImageBuffer(m_pOclObj->getContext(), CL_MEM_WRITE_ONLY, pbo);

        cl::Kernel verticalKernel(program,"box_filter_vertical");
        cl::Kernel horizontalKernel(program,"box_filter_horizontal");

        horizontalKernel.setArg(0, inputImageBuffer);
        horizontalKernel.setArg(1, tempImageBuffer);
        horizontalKernel.setArg(2, 8); // filter width

        cl::NDRange globalThreads(width, height);
        cl::NDRange localThreads(4, 4);

        std::vector<cl::Memory> glBuffers = {outputImageBuffer};
        m_pOclObj->getQueue().enqueueAcquireGLObjects(&glBuffers);

        std::vector<cl::Event> events;
        cl::Event ev1;
        m_pOclObj->getQueue().enqueueNDRangeKernel(horizontalKernel, cl::NullRange, globalThreads, localThreads, nullptr, &ev1);
        events.push_back(ev1);
        m_pOclObj->getQueue().flush();
        cl::WaitForEvents(events);

        verticalKernel.setArg(0, tempImageBuffer);
        verticalKernel.setArg(1, outputImageBuffer);
        verticalKernel.setArg(2, 8);

        cl::Event ev2;
        m_pOclObj->getQueue().enqueueNDRangeKernel(verticalKernel, cl::NullRange, globalThreads, localThreads, nullptr, &ev2);
        events.push_back(ev2);
        m_pOclObj->getQueue().flush();
        cl::WaitForEvents(events);

        cl::Event ev3;
        m_pOclObj->getQueue().enqueueReadBuffer(outputImageBuffer, CL_FALSE, 0, width * height * sizeof(cl_uchar4), outputBuf.get(), nullptr, &ev3);
        events.push_back(ev3);
        m_pOclObj->getQueue().flush();
        cl::WaitForEvents(events);

        cl::Event ev4;
        std::vector<cl::Memory> vec = {outputImageBuffer};
        m_pOclObj->getQueue().enqueueReleaseGLObjects(&vec, nullptr, &ev4);
        events.push_back(ev4);
        m_pOclObj->getQueue().flush();
        cl::WaitForEvents(events);
    }
    catch(cl::Error& e)
    {
        qDebug() << e.what() << "(" << QString::fromStdString(ocl::utils::getOpenCLErrorCodeStr(e.err()))<< ")" ;
    }

    CMat results = m_inputData.clone();
    memcpy(results.data, outputBuf.get(), width * height * sizeof(cl_uchar4));
}

QTEST_MAIN(COpenCLTests)

