#include "CSphereRender.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <math.h>
#include "CException.h"

CSphereRender::CSphereRender()
{
    initGL();
}

CSphereRender::~CSphereRender()
{
    if(m_pProgram)
        delete m_pProgram;

    if(m_pVShader)
        delete m_pVShader;

    if(m_pFShader)
        delete m_pFShader;

    m_vao.destroy();
    m_vbo.destroy();
}

void CSphereRender::setWindowInfo(int width, int height, float fov)
{
    m_wndWidth = width;
    m_wndHeight = height;
    m_fov = fov;
}

void CSphereRender::setMatrix(QMatrix4x4 *pModel, QMatrix4x4 *pView, QMatrix4x4 *pProjection)
{
    m_pProjectionMat = pProjection;
    m_pViewMat = pView;
    m_pModelMat = pModel;
}

void CSphereRender::setPosition(float x, float y, float z)
{
    if(m_vbo.isCreated())
        m_vbo.destroy();

    m_position << x << y << z;
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_position.constData(), m_position.count() * sizeof(float));
    m_vbo.release();
}

void CSphereRender::setRadius(float radius)
{
    m_radius = radius;
}

void CSphereRender::setColor(const QVector4D& color)
{
    m_color = color;
}

void CSphereRender::initGL()
{
    initializeOpenGLFunctions();
    m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

    m_pVShader = new QOpenGLShader(QOpenGLShader::Vertex);
    if(!m_pVShader->compileSourceFile(":/Shaders/spherevshader.vert"))
        throw CException(OpenGLExCode::SHADER_COMPILE_ERROR, m_pVShader->log().toStdString(), __func__, __FILE__, __LINE__);

    m_pFShader = new QOpenGLShader(QOpenGLShader::Fragment);
    if(!m_pFShader->compileSourceFile(":/Shaders/spherefshader.frag"))
        throw CException(OpenGLExCode::SHADER_COMPILE_ERROR, m_pFShader->log().toStdString(), __func__, __FILE__, __LINE__);

    m_pProgram = new QOpenGLShaderProgram;
    if(!m_pProgram->addShader(m_pVShader))
        throw CException(OpenGLExCode::SHADER_PROGRAM_ERROR, m_pProgram->log().toStdString(), __func__, __FILE__, __LINE__);
    if(!m_pProgram->addShader(m_pFShader))
        throw CException(OpenGLExCode::SHADER_PROGRAM_ERROR, m_pProgram->log().toStdString(), __func__, __FILE__, __LINE__);
    if(!m_pProgram->link())
        throw CException(OpenGLExCode::SHADER_LINK_ERROR, m_pProgram->log().toStdString(), __func__, __FILE__, __LINE__);

    m_modelViewMatId = m_pProgram->uniformLocation("modelViewMatrix");
    m_modelViewProjMatId = m_pProgram->uniformLocation("modelViewProjMatrix");
    m_projMatId = m_pProgram->uniformLocation("projMatrix");
    m_ptRadiusId = m_pProgram->uniformLocation("ptRadius");
    m_ptScaleId = m_pProgram->uniformLocation("ptScale");
    m_colorId = m_pProgram->uniformLocation("color");
    m_vertexId = m_pProgram->attributeLocation("vertex");

    //Create VAO
    m_vao.create();
}

void CSphereRender::render()
{
    const GLfloat halfViewRadianFactor = 0.5f * 3.1415926535f/180.0f;
    GLfloat ptScale = m_wndWidth / tanf(m_fov * halfViewRadianFactor);

    if(m_pProgram->bind() == false)
        throw CException(OpenGLExCode::SHADER_BIND_ERROR, m_pProgram->log().toStdString(), __func__, __FILE__, __LINE__);

    m_pProgram->setUniformValue(m_modelViewMatId, (*m_pViewMat) * (*m_pModelMat));
    m_pProgram->setUniformValue(m_modelViewProjMatId, (*m_pProjectionMat) * (*m_pViewMat) * (*m_pModelMat));
    m_pProgram->setUniformValue(m_projMatId, (*m_pProjectionMat));
    m_pProgram->setUniformValue(m_ptRadiusId, m_radius);
    m_pProgram->setUniformValue(m_ptScaleId, ptScale);
    m_pProgram->setUniformValue(m_colorId, m_color);

    m_vao.bind();

    if(m_vbo.bind())
    {
        m_pProgram->enableAttributeArray(m_vertexId);
        m_pProgram->setAttributeBuffer(m_vertexId, GL_FLOAT, 0, 3, 3*sizeof(GLfloat));
        m_vbo.release();
    }

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SPRITE_ARB);
    glDrawArrays(GL_POINTS, 0, 1);
    glDisable(GL_PROGRAM_POINT_SIZE);
    glDisable(GL_POINT_SPRITE_ARB);

    m_pProgram->disableAttributeArray(m_vertexId);
    m_vao.release();
    m_pProgram->release();
}
