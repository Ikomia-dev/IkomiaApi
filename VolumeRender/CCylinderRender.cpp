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

#include "CCylinderRender.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include "CException.h"

CCylinderRender::CCylinderRender()
{
    initGL();
}

CCylinderRender::~CCylinderRender()
{
    if(m_pProgram)
        delete m_pProgram;

    if(m_pVShader)
        delete m_pVShader;

    if(m_pFShader)
        delete m_pFShader;

    m_vao.destroy();
    m_vbo.destroy();

    if(m_pBase)
        delete[] m_pBase;
}

void CCylinderRender::setMatrix(QMatrix4x4 *pModel, QMatrix4x4 *pView, QMatrix4x4 *pProjection)
{
    m_pProjectionMat = pProjection;
    m_pViewMat = pView;
    m_pModelMat = pModel;
}

void CCylinderRender::setPosition(QVector3D startPt, QVector3D endPt)
{
    m_startPt = startPt;
    m_axis = endPt - startPt;
}

void CCylinderRender::setRadius(float radius)
{
    m_radius = radius;
}

void CCylinderRender::setColor(const QVector4D& color)
{
    m_color = color;
}

void CCylinderRender::initGL()
{
    initializeOpenGLFunctions();
    initVbo();

    m_pVShader = new QOpenGLShader(QOpenGLShader::Vertex);
    if(!m_pVShader->compileSourceFile(":/Shaders/cylindervshader.vert"))
        throw CException(OpenGLExCode::SHADER_COMPILE_ERROR, m_pVShader->log().toStdString(), __func__, __FILE__, __LINE__);

    m_pFShader = new QOpenGLShader(QOpenGLShader::Fragment);
    if(!m_pFShader->compileSourceFile(":/Shaders/cylinderfshader.frag"))
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
    m_modelViewInverseMatId = m_pProgram->uniformLocation("modelViewMatrixInverse");
    m_startPtId = m_pProgram->uniformLocation("startPt");
    m_axisId = m_pProgram->uniformLocation("axis");
    m_radiusId = m_pProgram->uniformLocation("radius");
    m_colorId = m_pProgram->uniformLocation("color");
    m_localCoordId = m_pProgram->attributeLocation("localCoordIn");

    //Create VAO
    m_vao.create();
}

void CCylinderRender::initVbo()
{
    GLfloat localBase[] = {
                            // First face -- front
                            0.0, 0.0, 0.0,
                            0.0, 1.0, 0.0,
                            1.0, 1.0, 0.0,
                            0.0, 0.0, 0.0,
                            1.0, 1.0, 0.0,
                            1.0, 0.0, 0.0,
                            // Second face -- back
                            0.0, 0.0, 1.0,
                            0.0, 1.0, 1.0,
                            1.0, 1.0, 1.0,
                            0.0, 0.0, 1.0,
                            1.0, 1.0, 1.0,
                            1.0, 0.0, 1.0,
                            // Third face -- left
                            0.0, 0.0, 0.0,
                            0.0, 0.0, 1.0,
                            0.0, 1.0, 1.0,
                            0.0, 0.0, 0.0,
                            0.0, 1.0, 1.0,
                            0.0, 1.0, 0.0,
                            // Fourth face -- right
                            1.0, 0.0, 0.0,
                            1.0, 0.0, 1.0,
                            1.0, 1.0, 1.0,
                            1.0, 0.0, 0.0,
                            1.0, 1.0, 1.0,
                            1.0, 1.0, 0.0,
                            // Fifth face -- up
                            0.0, 1.0, 0.0,
                            0.0, 1.0, 1.0,
                            1.0, 1.0, 1.0,
                            0.0, 1.0, 0.0,
                            1.0, 1.0, 1.0,
                            1.0, 1.0, 0.0,
                            // Sixth face -- down
                            0.0, 0.0, 0.0,
                            0.0, 0.0, 1.0,
                            1.0, 0.0, 1.0,
                            0.0, 0.0, 0.0,
                            1.0, 0.0, 1.0,
                            1.0, 0.0, 0.0
                        };

    if(m_pBase)
        delete[] m_pBase;

    //108 = 3 x 6 vertex (2triangles) x 6 faces (cube)
    m_pBase = new GLfloat[108];
    memcpy(m_pBase, localBase, 108*sizeof(GLfloat));

    m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_pBase, 108 * sizeof(GLfloat));
    m_vbo.release();
}

void CCylinderRender::render()
{
    if(m_pProgram->bind() == false)
        throw CException(OpenGLExCode::SHADER_BIND_ERROR, m_pProgram->log().toStdString(), __func__, __FILE__, __LINE__);

    QMatrix4x4 modelViewMat = (*m_pViewMat) * (*m_pModelMat);
    m_pProgram->setUniformValue(m_modelViewMatId, modelViewMat);
    m_pProgram->setUniformValue(m_modelViewProjMatId, (*m_pProjectionMat) * modelViewMat);
    m_pProgram->setUniformValue(m_projMatId, (*m_pProjectionMat));
    m_pProgram->setUniformValue(m_modelViewInverseMatId, modelViewMat.inverted());
    m_pProgram->setUniformValue(m_startPtId, m_startPt);
    m_pProgram->setUniformValue(m_axisId, m_axis);
    m_pProgram->setUniformValue(m_radiusId, m_radius);
    m_pProgram->setUniformValue(m_colorId, m_color);

    m_vao.bind();

    if(m_vbo.bind())
    {
        m_pProgram->enableAttributeArray(m_localCoordId);
        m_pProgram->setAttributeBuffer(m_localCoordId, GL_FLOAT, 0, 3, 3*sizeof(GLfloat));
        m_vbo.release();
    }

    glDrawArrays(GL_TRIANGLES, 0, 36);

    m_pProgram->disableAttributeArray(m_localCoordId);
    m_vao.release();
    m_pProgram->release();
}
