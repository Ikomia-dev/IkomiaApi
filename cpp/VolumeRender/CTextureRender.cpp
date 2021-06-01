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

#include "CTextureRender.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLTexture>

CTextureRender::CTextureRender(bool bPboMode)
{
    m_bPboMode = bPboMode;
    initGL();
    initVBO();
}

CTextureRender::~CTextureRender()
{
    if(m_pProgram)
        delete m_pProgram;

    if(m_pVShader)
        delete m_pVShader;

    if(m_pFShader)
        delete m_pFShader;

    if(m_pTexture)
        delete m_pTexture;

    m_vao.destroy();
    m_vbo.destroy();
}

void CTextureRender::render()
{
    if(m_bShaderMode)
    {
        if(m_pProgram->bind())
         {
             m_pProgram->setUniformValue(m_modelViewProjMatId, m_modelViewProj);
             m_pProgram->setUniformValue(m_samplerId, 0);

             m_vao.bind();

             if(m_vbo.bind())
             {
                 m_pProgram->enableAttributeArray(m_vertexId);
                 m_pProgram->setAttributeBuffer(m_vertexId, GL_FLOAT, 0, 2, 4*sizeof(GLfloat));
                 m_pProgram->enableAttributeArray(m_textureId);
                 m_pProgram->setAttributeBuffer(m_textureId, GL_FLOAT, 2*sizeof(GLfloat), 2, 4*sizeof(GLfloat));
                 m_vbo.release();
             }

             glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo);
             m_pTexture->bind();
             glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
             glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

             m_pProgram->disableAttributeArray(m_vertexId);
             m_pProgram->disableAttributeArray(m_textureId);
             m_pTexture->release();
             glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
             m_vao.release();
             m_pProgram->release();
         }
         else
             qDebug() << m_pProgram->log();
    }
    else
    {
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo);
        glDrawPixels(m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    }
}

void CTextureRender::setSizes(int width, int height)
{
    if(m_width != width || m_height != height)
    {
        m_width = width;
        m_height = height;
        initTexture();
    }
}

void CTextureRender::setMatrix(QMatrix4x4 &model, QMatrix4x4 &view, QMatrix4x4 &projection)
{
    m_modelMatrix = model;
    m_viewMatrix = view;
    m_projMatrix = projection;

    if(m_bPboMode)
        m_modelViewProj.setToIdentity();
    else
        m_modelViewProj = projection * view * model;
}

void CTextureRender::setPBO(GLuint pbo)
{
    m_pbo = pbo;
}

void CTextureRender::setShaderMode(bool bShader)
{
    m_bShaderMode = bShader;
}

GLuint CTextureRender::getTextureId()
{
    return m_pTexture->textureId();
}

void CTextureRender::initGL()
{
    initializeOpenGLFunctions();
    m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

    m_pVShader = new QOpenGLShader(QOpenGLShader::Vertex);
    if(!m_pVShader->compileSourceFile(":/Shaders/texturevshader.vert"))
        qDebug() << m_pVShader->log();

    m_pFShader = new QOpenGLShader(QOpenGLShader::Fragment);
    if(!m_pFShader->compileSourceFile(":/Shaders/texturefshader.frag"))
        qDebug() << m_pFShader->log();

    m_pProgram = new QOpenGLShaderProgram;
    if(!m_pProgram->addShader(m_pVShader))
        qDebug() << m_pProgram->log();
    if(!m_pProgram->addShader(m_pFShader))
        qDebug() << m_pProgram->log();
    if(!m_pProgram->link())
        qDebug() << m_pProgram->log();

    m_modelViewProjMatId = m_pProgram->uniformLocation("modelViewProjMatrix");
    m_samplerId = m_pProgram->uniformLocation("texture");
    m_vertexId = m_pProgram->attributeLocation("vertex");
    m_textureId = m_pProgram->attributeLocation("texCoordIn");

    //Create VAO
    m_vao.create();
}

void CTextureRender::initVBO()
{
    m_vertexData = {    -1, -1, 0, 0 ,
                         1, -1, 1, 0,
                         1,  1, 1, 1,
                        -1,  1, 0, 1
                   };

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_vertexData.constData(), m_vertexData.count() * sizeof(GLfloat));
    m_vbo.release();
}

void CTextureRender::initTexture()
{
    if (m_pTexture != nullptr)
        delete m_pTexture;

    m_pTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    m_pTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_pTexture->setMagnificationFilter(QOpenGLTexture::Nearest);
    m_pTexture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);
    m_pTexture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);
    m_pTexture->setFormat(QOpenGLTexture::RGBA8_UNorm);
    m_pTexture->setSize(m_width, m_height);
    m_pTexture->allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8);
}

